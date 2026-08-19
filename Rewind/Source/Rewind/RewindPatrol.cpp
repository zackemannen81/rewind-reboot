#include "RewindPatrol.h"

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
	PathBlock = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PathBlock"));
	PathBlock->SetupAttachment(Root);
	PathBlock->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	PathBlock->SetRelativeScale3D(FVector(4.f, 2.f, 2.5f));
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
	ApplyPhase(0.0);
}

void ARewindPatrol::ApplyAnchorOverride(FName AnchorId)
{
	(void)AnchorId;
}

void ARewindPatrol::ApplyPhase(double ElapsedSeconds)
{
	constexpr double Cycle = 40.0;
	constexpr double Window = 20.0;
	const double Phase = FMath::Fmod(ElapsedSeconds, Cycle);
	const bool bCovering = Phase < (Cycle - Window);

	PathBlock->SetCollisionEnabled(bCovering ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
	PathBlock->SetVisibility(bCovering);

	const FVector Away(0.f, -400.f, 90.f);
	const FVector OnPath(0.f, 0.f, 90.f);
	Body->SetRelativeLocation(bCovering ? OnPath : Away);
}
