#include "RewindCausalCheckpoint.h"

#include "RewindFuse.h"
#include "RewindIds.h"
#include "RewindLog.h"
#include "RewindLoopSubsystem.h"
#include "RewindSessionSubsystem.h"
#include "Components/BoxComponent.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"

ARewindCausalCheckpoint::ARewindCausalCheckpoint()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Volume = CreateDefaultSubobject<UBoxComponent>(TEXT("Volume"));
	Volume->SetupAttachment(Root);
	Volume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Volume->SetCollisionObjectType(ECC_WorldDynamic);
	Volume->SetCollisionResponseToAllChannels(ECR_Ignore);
	Volume->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Volume->SetGenerateOverlapEvents(true);
	Volume->SetBoxExtent(FVector(80.f, 300.f, 180.f));
	Volume->OnComponentBeginOverlap.AddDynamic(this, &ARewindCausalCheckpoint::OnBeginOverlap);
	Volume->OnComponentEndOverlap.AddDynamic(this, &ARewindCausalCheckpoint::OnEndOverlap);

	CheckpointId = RewindIds::CheckpointGroundFuseGate;
	Predicate = ERewindCheckpointPredicate::GroundFuseGate;
}

void ARewindCausalCheckpoint::Configure(
	FName InCheckpointId,
	ERewindCheckpointPredicate InPredicate,
	const FVector& BoxExtent)
{
	CheckpointId = InCheckpointId;
	Predicate = InPredicate;
	if (Volume)
	{
		Volume->SetBoxExtent(BoxExtent);
	}
}

bool ARewindCausalCheckpoint::EvaluatePredicate() const
{
	if (Predicate != ERewindCheckpointPredicate::GroundFuseGate)
	{
		return true;
	}

	bool bGroundFusePowered = false;
	if (const ARewindFuse* Fuse = ARewindFuse::Find(GetWorld()))
	{
		bGroundFusePowered = Fuse->IsSeatedIn(ERewindFuseSocket::Courtyard);
	}

	bool bGateAnchorActive = false;
	if (const UWorld* World = GetWorld())
	{
		if (const UGameInstance* GameInstance = World->GetGameInstance())
		{
			if (const URewindSessionSubsystem* Session =
					GameInstance->GetSubsystem<URewindSessionSubsystem>())
			{
				bGateAnchorActive =
					Session->GetActiveAnchor() == RewindIds::AnchorCourtyardGateOpen;
			}
		}
	}

	return bGroundFusePowered || bGateAnchorActive;
}

void ARewindCausalCheckpoint::NotifyCrossing()
{
	if (EvaluatePredicate())
	{
		RewindLog::Event(this, FString::Printf(
			TEXT("Checkpoint %s: pass"), *CheckpointId.ToString()));
		return;
	}

	RewindLog::Event(this, FString::Printf(
		TEXT("Checkpoint %s: FAIL  latching rewind"), *CheckpointId.ToString()));

	if (UWorld* World = GetWorld())
	{
		if (URewindLoopSubsystem* Loop = World->GetSubsystem<URewindLoopSubsystem>())
		{
			Loop->NotifyCausalContractFailed(CheckpointId);
		}
	}
}

void ARewindCausalCheckpoint::NotifyExited()
{
	// ADR-0009: a latched rewind cannot be cancelled by stepping back.
}

void ARewindCausalCheckpoint::OnBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	(void)OverlappedComponent;
	(void)OtherComp;
	(void)OtherBodyIndex;
	(void)bFromSweep;
	(void)SweepResult;

	if (IsPlayerPawn(OtherActor))
	{
		NotifyCrossing();
	}
}

void ARewindCausalCheckpoint::OnEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	(void)OverlappedComponent;
	(void)OtherComp;
	(void)OtherBodyIndex;

	if (IsPlayerPawn(OtherActor))
	{
		NotifyExited();
	}
}

bool ARewindCausalCheckpoint::IsPlayerPawn(const AActor* Actor) const
{
	const APawn* Pawn = Cast<APawn>(Actor);
	return Pawn && Pawn->IsPlayerControlled();
}
