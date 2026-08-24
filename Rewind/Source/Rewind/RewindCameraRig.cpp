#include "RewindCameraRig.h"

#include "RewindCameraRegion.h"
#include "RewindLog.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

ARewindCameraRig::ARewindCameraRig()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARewindCameraRig::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* Controller = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr)
	{
		Controller->SetViewTarget(this);
	}
	SnapToPlayer();
}

const APawn* ARewindCameraRig::GetPlayer() const
{
	const UWorld* World = GetWorld();
	const APlayerController* Controller = World ? World->GetFirstPlayerController() : nullptr;
	return Controller ? Controller->GetPawn() : nullptr;
}

bool ARewindCameraRig::ResolveTarget(
	FRotator& OutRotation,
	float& OutFieldOfView,
	double& OutTravel)
{
	const APawn* Pawn = GetPlayer();
	if (!Pawn)
	{
		return false;
	}

	const FVector PlayerLocation = Pawn->GetActorLocation();

	// Regions are authored edge to edge using half-open volumes. Keeping the
	// current region until the player genuinely leaves it still avoids needless
	// searches and makes the handoff state explicit.
	ARewindCameraRegion* Region = (ActiveRegion && ActiveRegion->Contains(PlayerLocation))
		? ActiveRegion.Get()
		: ARewindCameraRegion::FindContaining(GetWorld(), PlayerLocation);

	// Outside every region, the last region keeps the frame. A gap in the
	// region set is an authoring defect, and holding the last good frame makes
	// it visible without dropping the player into an undefined camera.
	if (!Region)
	{
		Region = ActiveRegion;
	}
	if (!Region)
	{
		return false;
	}

	if (Region != ActiveRegion)
	{
		const bool bCut = Region->CutsOnEntry();
		RewindLog::Event(this, FString::Printf(TEXT("Camera: region %s -> %s (%s)"),
			ActiveRegion ? *ActiveRegion->GetRegionName().ToString() : TEXT("none"),
			*Region->GetRegionName().ToString(),
			bCut ? TEXT("cut") : TEXT("blend")));

		ActiveRegion = Region;
		if (bCut || !bHasTravel)
		{
			CurrentTravel = Region->ClampTravel(Region->GetTravelCoord(PlayerLocation));
			bHasTravel = true;
			SetActorRotation(Region->GetCameraRotation());
			GetCameraComponent()->SetFieldOfView(Region->GetFieldOfView());
		}
	}

	// Dead zone: the camera holds still while the player is inside a band
	// centred on where the camera already looks, then follows so the player
	// stays at the band's edge. The player is never re-centred, because a frame
	// that always holds the player in the middle never holds anything else on
	// purpose.
	const double PlayerTravel = ActiveRegion->GetTravelCoord(PlayerLocation);
	const double Zone = ActiveRegion->GetDeadZone();

	double TargetTravel = CurrentTravel;
	if (PlayerTravel < CurrentTravel - Zone)
	{
		TargetTravel = PlayerTravel + Zone;
	}
	else if (PlayerTravel > CurrentTravel + Zone)
	{
		TargetTravel = PlayerTravel - Zone;
	}

	OutTravel = ActiveRegion->ClampTravel(TargetTravel);
	OutRotation = ActiveRegion->GetCameraRotation();
	OutFieldOfView = ActiveRegion->GetFieldOfView();
	return true;
}

void ARewindCameraRig::SnapToPlayer()
{
	FRotator TargetRotation;
	float TargetFieldOfView = 50.f;
	double TargetTravel = 0.0;
	if (!ResolveTarget(TargetRotation, TargetFieldOfView, TargetTravel))
	{
		return;
	}

	CurrentTravel = ActiveRegion->ClampTravel(ActiveRegion->GetTravelCoord(GetPlayer()->GetActorLocation()));
	bHasTravel = true;
	SetActorLocation(ActiveRegion->GetCameraLocation(CurrentTravel));
	SetActorRotation(TargetRotation);
	GetCameraComponent()->SetFieldOfView(TargetFieldOfView);
}

void ARewindCameraRig::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FRotator TargetRotation;
	float TargetFieldOfView = 50.f;
	double TargetTravel = 0.0;
	if (!ResolveTarget(TargetRotation, TargetFieldOfView, TargetTravel))
	{
		return;
	}

	// Frame-rate independent smoothing, in the same form as the reference
	// implementation this borrows from: a fraction of the remaining distance
	// per 16.67 ms, scaled by the actual frame time.
	const double Scale = static_cast<double>(DeltaSeconds) * 1000.0 / 16.67;
	const double Alpha = FMath::Min(1.0, FollowSpeed * Scale);
	const double RotationAlpha = FMath::Min(1.0, RotationBlendSpeed * Scale);

	CurrentTravel += (TargetTravel - CurrentTravel) * Alpha;
	if (FMath::Abs(TargetTravel - CurrentTravel) < 0.05)
	{
		CurrentTravel = TargetTravel;
	}

	SetActorLocation(ActiveRegion->GetCameraLocation(CurrentTravel));

	// Slerp rather than a rotator lerp, so a blend between two regions never
	// takes the long way round.
	SetActorRotation(FQuat::Slerp(GetActorQuat(), TargetRotation.Quaternion(),
		static_cast<float>(RotationAlpha)));
	GetCameraComponent()->SetFieldOfView(FMath::Lerp(
		GetCameraComponent()->FieldOfView, TargetFieldOfView,
		static_cast<float>(RotationAlpha)));
}
