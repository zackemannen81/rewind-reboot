#include "RewindTurnstile.h"

#include "RewindLog.h"
#include "RewindLoopSubsystem.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

ARewindTurnstile::ARewindTurnstile()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (Cube.Succeeded())
	{
		Mesh->SetStaticMesh(Cube.Object);
	}
	// The turnstile is the Transit Hub entrance, closed outside its 2.5 s window.
	// At 450 cm it left 155 cm open on each side of the 760 cm corridor, so the
	// player could pass at any `t` by walking around it and FL-12 had nothing to
	// gate.
	Mesh->SetRelativeScale3D(FVector(0.4f, 7.6f, 2.2f));
}

void ARewindTurnstile::Tick(float DeltaSeconds)
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

void ARewindTurnstile::RestoreFromBaseline()
{
	// Seed with the state phase 0 produces, so the apply prints no transition.
	// Phase 0 is inside the open window, since OpenWindow is positive.
	bOpenLogged = true;
	ApplyPhase(0.0);
	RewindLog::Baseline(TEXT("Turnstile: OPEN at phase 0"));
}

void ARewindTurnstile::ApplyAnchorOverride(FName AnchorId)
{
	(void)AnchorId;
}

void ARewindTurnstile::ApplyPhase(double ElapsedSeconds)
{
	constexpr double Cycle = 30.0;
	constexpr double OpenWindow = 2.5;
	const double Phase = FMath::Fmod(ElapsedSeconds, Cycle);
	const bool bOpen = Phase <= OpenWindow;
	Mesh->SetCollisionEnabled(bOpen ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryAndPhysics);
	Mesh->SetVisibility(!bOpen);

	// FL-12 states the turnstile is open when (t modulo 30) <= 2.5. Logging the
	// transitions makes that checkable against exact times. The tracker decides
	// only whether a line is printed; it is not world state.
	if (bOpenLogged != bOpen)
	{
		bOpenLogged = bOpen;
		RewindLog::Event(this, FString::Printf(
			TEXT("Turnstile: %s  (phase=%.2f of %.0fs)"),
			bOpen ? TEXT("OPEN") : TEXT("CLOSED"), Phase, Cycle));
	}
}
