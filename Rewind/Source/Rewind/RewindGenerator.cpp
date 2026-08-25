#include "RewindGenerator.h"

#include "RewindLog.h"
#include "RewindFuse.h"
#include "RewindCourtyardGate.h"
#include "RewindMessageIds.h"
#include "RewindMessageSubsystem.h"
#include "Components/StaticMeshComponent.h"
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
		if (URewindMessageSubsystem* Messages = URewindMessageSubsystem::Get(this))
		{
			Messages->Show(RewindMessageIds::GeneratorDead);
		}
		RewindLog::Event(this, TEXT("Generator: refused, fuse not in the courtyard socket"));
		return false;
	}
	bOnline = true;
	if (Gate)
	{
		Gate->OpenFromThisLoopPlay();
	}
	if (URewindMessageSubsystem* Messages = URewindMessageSubsystem::Get(this))
	{
		Messages->Show(RewindMessageIds::GeneratorOnline);
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
