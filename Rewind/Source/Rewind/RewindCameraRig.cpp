#include "RewindCameraRig.h"

#include "RewindCameraRegion.h"
#include "RewindLog.h"
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

bool ARewindCameraRig::ResolveTarget(FVector& OutLocation, FRotator& OutRotation)
{
	const APawn* Pawn = GetPlayer();
	if (!Pawn)
	{
		return false;
	}

	const FVector PlayerLocation = Pawn->GetActorLocation();

	// Regions are authored edge to edge, so a player standing exactly on a
	// boundary is inside both and the search returns whichever the actor
	// iterator reaches first. That flipped the frame back and forth eight times
	// in nine seconds during the first played test. Keeping the region the
	// player is already in until they genuinely leave it costs one check and
	// removes the whole class of thrash.
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

	OutLocation = ActiveRegion->GetCameraLocation(ActiveRegion->ClampTravel(TargetTravel));
	OutRotation = ActiveRegion->GetCameraRotation();
	return true;
}

void ARewindCameraRig::SnapToPlayer()
{
	FVector TargetLocation;
	FRotator TargetRotation;
	if (!ResolveTarget(TargetLocation, TargetRotation))
	{
		return;
	}

	CurrentTravel = ActiveRegion->ClampTravel(ActiveRegion->GetTravelCoord(GetPlayer()->GetActorLocation()));
	bHasTravel = true;
	SetActorLocation(ActiveRegion->GetCameraLocation(CurrentTravel));
	SetActorRotation(TargetRotation);
}

void ARewindCameraRig::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FVector TargetLocation;
	FRotator TargetRotation;
	if (!ResolveTarget(TargetLocation, TargetRotation))
	{
		return;
	}

	// Frame-rate independent smoothing, in the same form as the reference
	// implementation this borrows from: a fraction of the remaining distance
	// per 16.67 ms, scaled by the actual frame time.
	const double Scale = static_cast<double>(DeltaSeconds) * 1000.0 / 16.67;
	const double Alpha = FMath::Min(1.0, FollowSpeed * Scale);
	const double RotationAlpha = FMath::Min(1.0, RotationBlendSpeed * Scale);

	const double TargetTravel = ActiveRegion->GetTravelCoord(TargetLocation);
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
}
