#include "RewindGenerator.h"

#include "RewindLog.h"
#include "RewindFuse.h"
#include "RewindCourtyardGate.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"

ARewindGenerator::ARewindGenerator()
{
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Mesh->SetCollisionResponseToAllChannels(ECR_Overlap);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (Cube.Succeeded())
	{
		Mesh->SetStaticMesh(Cube.Object);
	}
	Mesh->SetRelativeScale3D(FVector(1.2f, 0.8f, 1.f));
}

void ARewindGenerator::SetGate(ARewindCourtyardGate* InGate)
{
	Gate = InGate;
}

bool ARewindGenerator::TryInteract(APawn* InstigatorPawn)
{
	(void)InstigatorPawn;
	if (bOnline)
	{
		return false;
	}
	// `chapter-1-authored.md`: the generator starts only while the fuse is in
	// the courtyard socket. That is the whole cost of opening the gate, and it
	// is paid in the building's lift.
	const ARewindFuse* Fuse = ARewindFuse::Find(GetWorld());
	if (!Fuse || !Fuse->IsSeatedIn(ERewindFuseSocket::Courtyard))
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow, TEXT("Generator: no courtyard power"));
		}
		RewindLog::Event(this, TEXT("Generator: refused, no courtyard power"));
		return false;
	}
	bOnline = true;
	if (Gate)
	{
		Gate->OpenFromThisLoopPlay();
	}
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Generator: online, gate open"));
	}
	RewindLog::Event(this, TEXT("Generator: online, gate opened by this-loop play"));
	return true;
}

void ARewindGenerator::RestoreFromBaseline()
{
	bOnline = false;
	RewindLog::Baseline(TEXT("Generator: offline"));
}

void ARewindGenerator::ApplyAnchorOverride(FName AnchorId)
{
	(void)AnchorId;
}
