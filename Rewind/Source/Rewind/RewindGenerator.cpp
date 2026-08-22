#include "RewindGenerator.h"

#include "RewindLog.h"
#include "RewindFuseBox.h"
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

void ARewindGenerator::SetLinks(ARewindFuseBox* InFuse, ARewindCourtyardGate* InGate)
{
	FuseBox = InFuse;
	Gate = InGate;
}

bool ARewindGenerator::TryInteract(APawn* InstigatorPawn)
{
	(void)InstigatorPawn;
	if (bOnline)
	{
		return false;
	}
	if (!FuseBox || !FuseBox->IsCourtyardPowerOn())
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
