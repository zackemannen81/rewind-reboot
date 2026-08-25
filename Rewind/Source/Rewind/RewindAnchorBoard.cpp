#include "RewindAnchorBoard.h"

#include "RewindLog.h"
#include "RewindCourtyardGate.h"
#include "RewindMessageIds.h"
#include "RewindMessageSubsystem.h"
#include "RewindSessionSubsystem.h"
#include "Components/StaticMeshComponent.h"
#include "EngineUtils.h"
#include "Engine/GameInstance.h"
#include "UObject/ConstructorHelpers.h"

ARewindAnchorBoard::ARewindAnchorBoard()
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
	Mesh->SetRelativeScale3D(FVector(0.1f, 1.2f, 1.f));
}

bool ARewindAnchorBoard::TryInteract(APawn* InstigatorPawn)
{
	if (!InstigatorPawn)
	{
		return false;
	}

	bool bOpenFromPlay = false;
	if (UWorld* World = GetWorld())
	{
		for (TActorIterator<ARewindCourtyardGate> It(World); It; ++It)
		{
			bOpenFromPlay = It->WasOpenedByThisLoopPlay();
			break;
		}
	}

	URewindSessionSubsystem* Session = nullptr;
	if (UGameInstance* GI = InstigatorPawn->GetGameInstance())
	{
		Session = GI->GetSubsystem<URewindSessionSubsystem>();
	}
	if (!Session)
	{
		return false;
	}

	const bool bOk = Session->TryCommitCourtyardGateAnchor(bOpenFromPlay);
	RewindLog::Event(this, FString::Printf(
		TEXT("Board: commit courtyard_gate_open %s (gate open from this-loop play=%s)"),
		bOk ? TEXT("ACCEPTED") : TEXT("REFUSED"),
		bOpenFromPlay ? TEXT("yes") : TEXT("no")));
	if (URewindMessageSubsystem* Messages = URewindMessageSubsystem::Get(this))
	{
		Messages->Show(bOk ? RewindMessageIds::AnchorAccepted : RewindMessageIds::AnchorRefused);
	}
	return bOk;
}
