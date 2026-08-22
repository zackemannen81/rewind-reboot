#include "RewindTurnstile.h"

#include "RewindLog.h"
#include "RewindLoopSubsystem.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
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
	CheckHubEntry();
}

void ARewindTurnstile::CheckHubEntry()
{
	// FL-13 asks that the player pass the turnstile into the Transit Hub, and
	// FL-14 compares the elapsed time of that crossing between two runs.
	// Neither is readable without an event, so record the first crossing of
	// each loop. This observes; it changes nothing.
	if (bHubEntryLogged)
	{
		return;
	}

	const UWorld* World = GetWorld();
	const APlayerController* Controller = World ? World->GetFirstPlayerController() : nullptr;
	const APawn* Pawn = Controller ? Controller->GetPawn() : nullptr;
	if (!Pawn)
	{
		return;
	}

	// The turnstile spans the walkable corridor, so being past it on X is the
	// only way to stand in the hub.
	if (Pawn->GetActorLocation().X <= GetActorLocation().X)
	{
		return;
	}

	bHubEntryLogged = true;
	RewindLog::Event(this, FString::Printf(
		TEXT("Hub: entered past the turnstile (turnstile was %s)"),
		bOpenNow ? TEXT("OPEN") : TEXT("CLOSED")));
}

void ARewindTurnstile::RestoreFromBaseline()
{
	// Seed with the state phase 0 produces, so the apply prints no transition.
	// Phase 0 is inside the open window, since OpenWindow is positive.
	bOpenLogged = true;
	bHubEntryLogged = false;
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
	bOpenNow = bOpen;
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
