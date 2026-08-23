#include "RewindLift.h"

#include "RewindChapter1Metrics.h"
#include "RewindFuse.h"
#include "RewindLog.h"
#include "RewindLoopSubsystem.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/ConstructorHelpers.h"

ARewindLift::ARewindLift()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Cabin = CreateDefaultSubobject<USceneComponent>(TEXT("Cabin"));
	Cabin->SetupAttachment(Root);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeFinder(TEXT("/Engine/BasicShapes/Cube.Cube"));
	UStaticMesh* Cube = CubeFinder.Succeeded() ? CubeFinder.Object : nullptr;
	auto AddCabinPart = [this, Cube](FName Name, FVector Location, FVector Scale)
	{
		UStaticMeshComponent* Part = CreateDefaultSubobject<UStaticMeshComponent>(Name);
		Part->SetupAttachment(Cabin);
		Part->SetRelativeLocation(Location);
		Part->SetRelativeScale3D(Scale);
		Part->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (Cube)
		{
			Part->SetStaticMesh(Cube);
		}
		return Part;
	};

	Platform = AddCabinPart(TEXT("Platform"), FVector(0.f, 0.f, -5.f), FVector(2.2f, 2.4f, 0.1f));
	// The platform is the interaction target. It overlaps rather than blocks so
	// the passenger can stand in the cage while it moves.
	Platform->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Platform->SetCollisionResponseToAllChannels(ECR_Overlap);
	CabinBack = AddCabinPart(TEXT("CabinBack"), FVector(0.f, 115.f, 110.f), FVector(2.2f, 0.1f, 2.2f));
	CabinRoof = AddCabinPart(TEXT("CabinRoof"), FVector(0.f, 0.f, 220.f), FVector(2.2f, 2.4f, 0.1f));

	auto AddShaftRail = [this, Cube](FName Name, float X)
	{
		UStaticMeshComponent* Rail = CreateDefaultSubobject<UStaticMeshComponent>(Name);
		Rail->SetupAttachment(Root);
		Rail->SetRelativeLocation(FVector(X, 125.f, 600.f));
		Rail->SetRelativeScale3D(FVector(0.12f, 0.12f, 12.f));
		Rail->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (Cube)
		{
			Rail->SetStaticMesh(Cube);
		}
		return Rail;
	};
	ShaftLeft = AddShaftRail(TEXT("ShaftLeft"), -105.f);
	ShaftRight = AddShaftRail(TEXT("ShaftRight"), 105.f);
}

void ARewindLift::Configure(double InTopZ, double InBottomZ)
{
	TopZ = InTopZ;
	BottomZ = InBottomZ;
	ShaftLeft->SetRelativeLocation(FVector(-105.f, 125.f, static_cast<float>((TopZ + BottomZ) * 0.5)));
	ShaftRight->SetRelativeLocation(FVector(105.f, 125.f, static_cast<float>((TopZ + BottomZ) * 0.5)));
	const FVector RailScale = FVector(0.12f, 0.12f, static_cast<float>((TopZ - BottomZ) / 100.0));
	ShaftLeft->SetRelativeScale3D(RailScale);
	ShaftRight->SetRelativeScale3D(RailScale);
	SetCabinZ(TopZ);
}

void ARewindLift::SetCabinZ(double Z)
{
	Cabin->SetRelativeLocation(FVector(0.f, 0.f, static_cast<float>(Z)));
}

bool ARewindLift::TryInteract(APawn* InstigatorPawn)
{
	if (!InstigatorPawn || bDescending)
	{
		return false;
	}
	if (bAtEntranceFloor)
	{
		RewindLog::Event(this, TEXT("Lift: refused, cage already at entrance floor"));
		return false;
	}

	const ARewindFuse* Fuse = ARewindFuse::Find(GetWorld());
	if (!Fuse || !Fuse->IsSeatedIn(ERewindFuseSocket::Building))
	{
		RewindLog::Event(this, TEXT("Lift: refused, fuse not in the building socket"));
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow,
				TEXT("Lift: no power. Seat the fuse in the building socket"));
		}
		return false;
	}

	const URewindLoopSubsystem* Loop = GetWorld()->GetSubsystem<URewindLoopSubsystem>();
	if (!Loop)
	{
		return false;
	}

	Passenger = InstigatorPawn;
	PassengerOffset = Passenger->GetActorLocation()
		- (GetActorLocation() + FVector(0.f, 0.f, static_cast<float>(TopZ)));
	DescentStartedAt = Loop->GetElapsedLoopTime();
	bDescending = true;
	if (ACharacter* Character = Cast<ACharacter>(Passenger))
	{
		Character->GetCharacterMovement()->DisableMovement();
	}
	RewindLog::Event(this, FString::Printf(
		TEXT("Lift: descent started, building fuse present (authored %.1fs)"),
		RewindChapter1Metrics::LiftTravelSeconds));
	return true;
}

void ARewindLift::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	(void)DeltaSeconds;

	const ARewindFuse* Fuse = ARewindFuse::Find(GetWorld());
	const bool bPowered = Fuse && Fuse->IsSeatedIn(ERewindFuseSocket::Building);
	if (bPowerLogged != bPowered)
	{
		bPowerLogged = bPowered;
		RewindLog::Event(this, bPowered
			? TEXT("Lift: POWERED by the building socket")
			: TEXT("Lift: UNPOWERED, building socket empty"));
	}

	if (!bDescending || !Passenger)
	{
		return;
	}
	if (!bPowered)
	{
		RewindLog::Event(this, TEXT("Lift: stopped between floors, building fuse removed"));
		ReleasePassenger();
		return;
	}

	const URewindLoopSubsystem* Loop = GetWorld()->GetSubsystem<URewindLoopSubsystem>();
	if (!Loop)
	{
		return;
	}
	const double Travelled = Loop->GetElapsedLoopTime() - DescentStartedAt;
	const double Alpha = FMath::Clamp(Travelled / RewindChapter1Metrics::LiftTravelSeconds, 0.0, 1.0);
	const double CabinZ = FMath::Lerp(TopZ, BottomZ, Alpha);
	SetCabinZ(CabinZ);
	Passenger->SetActorLocation(
		GetActorLocation() + FVector(0.f, 0.f, static_cast<float>(CabinZ)) + PassengerOffset,
		false, nullptr, ETeleportType::TeleportPhysics);

	if (Alpha >= 1.0)
	{
		bAtEntranceFloor = true;
		RewindLog::Event(this, FString::Printf(
			TEXT("Lift: entrance floor reached, measured descent %.2fs"), Travelled));
		ReleasePassenger();
	}
}

void ARewindLift::ReleasePassenger()
{
	if (ACharacter* Character = Cast<ACharacter>(Passenger))
	{
		Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
	Passenger = nullptr;
	bDescending = false;
}

void ARewindLift::RestoreFromBaseline()
{
	ReleasePassenger();
	bAtEntranceFloor = false;
	SetCabinZ(TopZ);
	bPowerLogged = false;
	RewindLog::Baseline(TEXT("Lift: at 4C, unpowered; building socket empty"));
}

void ARewindLift::ApplyAnchorOverride(FName AnchorId)
{
	(void)AnchorId;
}
