#include "RewindRadio.h"

#include "RewindLog.h"
#include "RewindIds.h"
#include "RewindSessionSubsystem.h"
#include "GameFramework/Pawn.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/GameInstance.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"

ARewindRadio::ARewindRadio()
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
	Mesh->SetRelativeScale3D(FVector(0.4f, 0.6f, 0.3f));
}

bool ARewindRadio::TryInteract(APawn* InstigatorPawn)
{
	if (!InstigatorPawn)
	{
		return false;
	}
	if (UGameInstance* GI = InstigatorPawn->GetGameInstance())
	{
		if (URewindSessionSubsystem* Session = GI->GetSubsystem<URewindSessionSubsystem>())
		{
			Session->ObtainKnowledge(RewindIds::KnowledgeRadioCode7312);
		}
	}
	RewindLog::Event(this, TEXT("Radio: 7312 obtained"));
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, TEXT("Radio: 7312"));
	}
	return true;
}
