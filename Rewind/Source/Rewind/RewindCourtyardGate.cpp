#include "RewindCourtyardGate.h"

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
	Mesh->SetRelativeScale3D(FVector(0.4f, 4.f, 3.f));
	SetOpen(false);
}

void ARewindCourtyardGate::RestoreFromBaseline()
{
	bOpenedByThisLoopPlay = false;
	SetOpen(false);
}

void ARewindCourtyardGate::ApplyAnchorOverride(FName AnchorId)
{
	if (AnchorId == RewindIds::AnchorCourtyardGateOpen)
	{
		SetOpen(true);
		bOpenedByThisLoopPlay = false;
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
