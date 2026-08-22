#include "RewindPatrol.h"

#include "RewindLog.h"
#include "RewindLoopSubsystem.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

ARewindPatrol::ARewindPatrol()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube.Cube"));
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Root);
	Body->SetRelativeScale3D(FVector(0.8f, 0.8f, 1.8f));
	// The barrier has to span the walkable corridor, which is 760 cm between the
	// edge walls, and stand on the floor. At its previous 200 cm width it left
	// 280 cm open on each side, so the path was never covered and FL-09 had
	// nothing to demonstrate.
	PathBlock = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PathBlock"));
	PathBlock->SetupAttachment(Root);
	PathBlock->SetRelativeLocation(FVector(0.f, 0.f, 125.f));
	PathBlock->SetRelativeScale3D(FVector(1.f, 7.6f, 2.5f));
	if (Cube.Succeeded())
	{
		Body->SetStaticMesh(Cube.Object);
		PathBlock->SetStaticMesh(Cube.Object);
	}
}

void ARewindPatrol::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	(void)DeltaSeconds;
	double Elapsed = 0.0;
	if (UWorld* World = GetWorld())
	{
		if (const URewindLoopSubsystem* Loop = World->GetSubsystem<URewindLoopSubsystem>())
		{
			Elapsed = Loop->GetElapsedLoopTime();
		}
	}
	ApplyPhase(Elapsed);
}

void ARewindPatrol::RestoreFromBaseline()
{
	// Seed the log tracker with the state phase 0 produces, so the apply itself
	// does not print a transition. Phase 0 is inside the covering period by
	// construction, since PatrolCovering is positive.
	bCoveringLogged = true;
	ApplyPhase(0.0);
	RewindLog::Baseline(TEXT("Patrol: path COVERED at phase 0"));
}

void ARewindPatrol::ApplyAnchorOverride(FName AnchorId)
{
	(void)AnchorId;
}

namespace
{
	constexpr double PatrolCycle = 40.0;
	constexpr double PatrolWindow = 20.0;
	constexpr double PatrolCovering = PatrolCycle - PatrolWindow;

	constexpr double PaceHalfWidth = 300.0;
	constexpr double StandAsideY = -320.0;
	constexpr double StepAsideSeconds = 4.0;

	// Where the body stands at a given phase. Continuous, periodic, and a pure
	// function of the phase: the same `t` gives the same pose on every loop, as
	// `loop-and-determinism.md` requires. It reaches Y=0 at both ends of the
	// cycle so the walk does not jump at a boundary.
	double PatrolBodyY(double Phase)
	{
		if (Phase < PatrolCovering)
		{
			// Pacing the barrier: one sweep across and back per covering period.
			return PaceHalfWidth * FMath::Sin((Phase / PatrolCovering) * 2.0 * PI);
		}

		const double Out = Phase - PatrolCovering;
		if (Out < StepAsideSeconds)
		{
			return FMath::Lerp(0.0, StandAsideY, Out / StepAsideSeconds);
		}
		if (Out > PatrolWindow - StepAsideSeconds)
		{
			const double Back = (Out - (PatrolWindow - StepAsideSeconds)) / StepAsideSeconds;
			return FMath::Lerp(StandAsideY, 0.0, Back);
		}
		return StandAsideY;
	}
}

void ARewindPatrol::ApplyPhase(double ElapsedSeconds)
{
	const double Phase = FMath::Fmod(ElapsedSeconds, PatrolCycle);
	const bool bCovering = Phase < PatrolCovering;

	// Coverage is unchanged: covered for the first 20 s of every 40 s cycle,
	// clear for the last 20 s. FL-08 and FL-09 read this and nothing else.
	PathBlock->SetCollisionEnabled(bCovering ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
	PathBlock->SetVisibility(bCovering);

	// FL-08 compares patrol phase at a stated `t` and FL-09 asks for a 20 s
	// uncovered window. Logging the transitions gives both an exact time
	// instead of a judgement from a paused frame. The tracker below decides
	// only whether a line is printed; it is not world state and nothing reads
	// it.
	if (bCoveringLogged != bCovering)
	{
		bCoveringLogged = bCovering;
		RewindLog::Event(this, FString::Printf(
			TEXT("Patrol: path %s  (phase=%.2f of %.0fs)"),
			bCovering ? TEXT("COVERED") : TEXT("CLEAR"), Phase, PatrolCycle));
	}

	// The body walks its beat instead of teleporting between two poses. The
	// authored space gives it a route, and a route the player can watch is what
	// makes the window learnable rather than surprising.
	Body->SetRelativeLocation(FVector(0.f, PatrolBodyY(Phase), 90.f));
}
