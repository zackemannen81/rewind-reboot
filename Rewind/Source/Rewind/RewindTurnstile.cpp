#include "RewindTurnstile.h"

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
	Mesh->SetRelativeScale3D(FVector(0.4f, 4.5f, 2.2f));
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
	ApplyPhase(0.0);
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
}
