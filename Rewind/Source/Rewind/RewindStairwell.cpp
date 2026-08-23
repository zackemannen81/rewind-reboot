#include "RewindStairwell.h"

#include "RewindChapter1Metrics.h"
#include "RewindLog.h"
#include "RewindLoopSubsystem.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"

ARewindStairwell::ARewindStairwell()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (Cube.Succeeded())
	{
		CubeMesh = Cube.Object;
	}

	// 4C -> floor 3, floor 3 -> floor 2, floor 2 -> entrance. The alternating
	// direction makes a legible switchback stairwell beside the lift shaft.
	AddFlight(0, FVector(0.f, 200.f, 1200.f), 1.f, 200.f);
	AddBox(TEXT("Floor3Landing"), FVector(3360.f, 0.f, 795.f), FVector(500.f, 800.f, 10.f));
	AddFlight(1, FVector(3360.f, -200.f, 800.f), -1.f, -200.f);
	AddBox(TEXT("Floor2Landing"), FVector(0.f, 0.f, 395.f), FVector(500.f, 800.f, 10.f));
	AddFlight(2, FVector(0.f, 200.f, 400.f), 1.f, 200.f);
}

void ARewindStairwell::AddFlight(int32 FlightIndex, FVector Start, float DirectionX, float CentreY)
{
	const float Rise = static_cast<float>(
		RewindChapter1Metrics::FloorHeightCm / RewindChapter1Metrics::StepsPerFlight);
	const float Run = static_cast<float>(RewindChapter1Metrics::StepRunCm);
	for (int32 Step = 0; Step < RewindChapter1Metrics::StepsPerFlight; ++Step)
	{
		const float TopZ = Start.Z - (Step + 1) * Rise;
		const float X = Start.X + DirectionX * (Step + 0.5f) * Run;
		const bool bLandingOpening = Step < 2
			|| Step >= RewindChapter1Metrics::StepsPerFlight - 2;
		AddBox(
			FName(*FString::Printf(TEXT("Flight%d_Step%02d"), FlightIndex + 1, Step + 1)),
			FVector(X, CentreY, TopZ - 8.f), FVector(Run, 360.f, 16.f));
		// Leave both rail sides open over the two endpoint steps. Which side is
		// the landing/hall side changes when the authored stairwell is rotated;
		// keeping one side by local sign made the rotated entrance rail a hard
		// wall. The middle twenty steps retain both rails.
		if (!bLandingOpening)
		{
			AddBox(
				FName(*FString::Printf(TEXT("Flight%d_RailNear%02d"), FlightIndex + 1, Step + 1)),
				FVector(X, CentreY - 190.f, TopZ + 45.f), FVector(Run, 12.f, 90.f));
		}
		if (!bLandingOpening)
		{
			AddBox(
				FName(*FString::Printf(TEXT("Flight%d_RailFar%02d"), FlightIndex + 1, Step + 1)),
				FVector(X, CentreY + 190.f, TopZ + 45.f), FVector(Run, 12.f, 90.f));
		}
	}
}

void ARewindStairwell::AddBox(FName Name, FVector RelativeLocation, FVector SizeCm, bool bVisible)
{
	UStaticMeshComponent* Mesh = CreateDefaultSubobject<UStaticMeshComponent>(Name);
	Mesh->SetupAttachment(Root);
	Mesh->SetRelativeLocation(RelativeLocation);
	Mesh->SetRelativeScale3D(SizeCm / 100.f);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	Mesh->SetVisibility(bVisible);
	if (CubeMesh)
	{
		Mesh->SetStaticMesh(CubeMesh);
	}
}

bool ARewindStairwell::IsPawnNear(const APawn* Pawn, const FVector& RelativePoint) const
{
	if (!Pawn)
	{
		return false;
	}
	const FVector Delta = Pawn->GetActorLocation()
		- GetActorTransform().TransformPosition(RelativePoint);
	return FMath::Abs(Delta.Z) <= 130.f && FVector2D(Delta.X, Delta.Y).Size() <= 260.f;
}

void ARewindStairwell::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	(void)DeltaSeconds;
	const UWorld* World = GetWorld();
	const APlayerController* Controller = World ? World->GetFirstPlayerController() : nullptr;
	const APawn* Pawn = Controller ? Controller->GetPawn() : nullptr;
	const URewindLoopSubsystem* Loop = World ? World->GetSubsystem<URewindLoopSubsystem>() : nullptr;
	if (!Pawn || !Loop)
	{
		return;
	}

	const FVector Checkpoints[] = {
		FVector(0.f, 200.f, 1296.f),
		FVector(3360.f, 0.f, 896.f),
		FVector(0.f, 0.f, 496.f),
		FVector(3360.f, 200.f, 96.f),
	};
	if (NextCheckpoint >= UE_ARRAY_COUNT(Checkpoints)
		|| !IsPawnNear(Pawn, Checkpoints[NextCheckpoint]))
	{
		return;
	}

	if (NextCheckpoint == 0)
	{
		StartedAt = Loop->GetElapsedLoopTime();
		RewindLog::Event(this, TEXT("Stairs: entered at 4C; three flights to entrance"));
	}
	else if (NextCheckpoint < RewindChapter1Metrics::FloorTransitions)
	{
		const int32 Floor = 4 - NextCheckpoint;
		RewindLog::Event(this, FString::Printf(
			TEXT("Stairs: floor %d landing reached (%d/3 flights)"), Floor, NextCheckpoint));
	}
	else
	{
		const double Measured = Loop->GetElapsedLoopTime() - StartedAt;
		RewindLog::Event(this, FString::Printf(
			TEXT("Stairs: entrance floor reached, measured descent %.2fs (minimum geometry %.2fs)"),
			Measured, RewindChapter1Metrics::StairMinimumSeconds()));
	}
	++NextCheckpoint;
}

void ARewindStairwell::RestoreFromBaseline()
{
	NextCheckpoint = 0;
	StartedAt = 0.0;
	RewindLog::Baseline(FString::Printf(
		TEXT("Stairs: available; 3 flights, %.0fcm authored run, %.2fs minimum at 200cm/s"),
		RewindChapter1Metrics::StairRunLengthCm(),
		RewindChapter1Metrics::StairMinimumSeconds()));
}

void ARewindStairwell::ApplyAnchorOverride(FName AnchorId)
{
	(void)AnchorId;
}
