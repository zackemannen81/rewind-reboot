#include "RewindCourtyardGate.h"

#include "RewindLog.h"
#include "RewindIds.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

ARewindCourtyardGate::ARewindCourtyardGate()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (Cube.Succeeded())
	{
		Mesh->SetStaticMesh(Cube.Object);
	}
	// `chapter-1-authored.md`: the closed gate "blocks the way to the street".
	// At 400 cm it left 180 cm open on each side of the 760 cm corridor, which a
	// 42 cm capsule walks straight through, so a closed gate blocked nothing.
	Mesh->SetRelativeScale3D(FVector(0.4f, 7.6f, 3.f));
	SetOpen(false);
}

void ARewindCourtyardGate::RestoreFromBaseline()
{
	bOpenedByThisLoopPlay = false;
	SetOpen(false);
	RewindLog::Baseline(TEXT("Gate: CLOSED"));
}

void ARewindCourtyardGate::ApplyAnchorOverride(FName AnchorId)
{
	if (AnchorId == RewindIds::AnchorCourtyardGateOpen)
	{
		SetOpen(true);
		bOpenedByThisLoopPlay = false;
		RewindLog::Baseline(TEXT("Gate: OPEN, held by anchor courtyard_gate_open"));
	}
}

void ARewindCourtyardGate::OpenFromThisLoopPlay()
{
	SetOpen(true);
	bOpenedByThisLoopPlay = true;
}

void ARewindCourtyardGate::SetOpen(bool bInOpen)
{
	bOpen = bInOpen;
	Mesh->SetCollisionEnabled(bOpen ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryAndPhysics);
	Mesh->SetVisibility(!bOpen);
}
