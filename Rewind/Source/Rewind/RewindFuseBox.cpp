#include "RewindFuseBox.h"

#include "RewindLog.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"

ARewindFuseBox::ARewindFuseBox()
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
	Mesh->SetRelativeScale3D(FVector(0.4f, 0.2f, 0.6f));
}

bool ARewindFuseBox::TryInteract(APawn* InstigatorPawn)
{
	(void)InstigatorPawn;
	if (!bHasFuse)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("Fuse: already used this loop"));
		}
		RewindLog::Event(this, TEXT("Fuse: refused, already used this loop"));
		return false;
	}
	bHasFuse = false;
	bCourtyardPowerOn = true;
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Fuse: courtyard has power"));
	}
	RewindLog::Event(this, TEXT("Fuse: routed, courtyard power ON"));
	return true;
}

void ARewindFuseBox::RestoreFromBaseline()
{
	bHasFuse = true;
	bCourtyardPowerOn = false;
	RewindLog::Baseline(TEXT("Fuse: available, courtyard power OFF"));
}

void ARewindFuseBox::ApplyAnchorOverride(FName AnchorId)
{
	(void)AnchorId;
}
