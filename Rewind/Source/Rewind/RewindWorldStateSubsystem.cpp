#include "RewindWorldStateSubsystem.h"

#include "RewindLog.h"
#include "RewindCameraRig.h"
#include "RewindFourCBlockout.h"
#include "RewindProofLayout.h"
#include "RewindLoopParticipant.h"
#include "RewindSessionSubsystem.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"

bool URewindWorldStateSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	const UWorld* World = Cast<UWorld>(Outer);
	return World && World->IsGameWorld();
}

void URewindWorldStateSubsystem::EnsureAuthoredSpace()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	ARewindProofLayout* Layout = nullptr;
	for (TActorIterator<ARewindProofLayout> It(World); It; ++It)
	{
		Layout = *It;
		break;
	}
	if (!Layout)
	{
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Layout = World->SpawnActor<ARewindProofLayout>(FVector::ZeroVector, FRotator::ZeroRotator, Params);
	}
	if (Layout)
	{
		Layout->EnsureContents();
	}
}

void URewindWorldStateSubsystem::ApplyLoopStart()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	EnsureAuthoredSpace();

	TArray<IRewindLoopParticipant*> WorldParts;
	for (FActorIterator It(World); It; ++It)
	{
		AActor* Actor = *It;
		if (IRewindLoopParticipant* Participant = Cast<IRewindLoopParticipant>(Actor))
		{
			if (!Participant->IsPlayerBody())
			{
				WorldParts.Add(Participant);
			}
		}
	}

	for (IRewindLoopParticipant* Participant : WorldParts)
	{
		Participant->RestoreFromBaseline();
	}

	FName ActiveAnchor = NAME_None;
	if (const UGameInstance* GameInstance = World->GetGameInstance())
	{
		if (const URewindSessionSubsystem* Session = GameInstance->GetSubsystem<URewindSessionSubsystem>())
		{
			ActiveAnchor = Session->GetActiveAnchor();
		}
	}

	if (!ActiveAnchor.IsNone())
	{
		for (IRewindLoopParticipant* Participant : WorldParts)
		{
			Participant->ApplyAnchorOverride(ActiveAnchor);
		}
	}

	PlacePlayerBody();
}

void URewindWorldStateSubsystem::PlacePlayerBody()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const FTransform Start = GetLoopStartPose();
	APlayerController* Controller = World->GetFirstPlayerController();
	APawn* Pawn = Controller ? Controller->GetPawn() : nullptr;
	if (!Pawn)
	{
		return;
	}

	Pawn->SetActorTransform(Start, false, nullptr, ETeleportType::ResetPhysics);

	// Step 4 of the apply order. Logged so FL-01's "the player is in Apartment
	// 4C at t = 0" is readable from the log rather than needing a live query.
	const FVector P = Start.GetLocation();
	RewindLog::Baseline(FString::Printf(
		TEXT("Player: placed at loop-start pose (%.0f, %.0f, %.0f)"), P.X, P.Y, P.Z));

	// The camera takes its region's pose immediately at loop start, per
	// `camera-and-movement.md`. This is world state notifying presentation; it
	// never reads anything back, so the camera stays outside the model.
	for (TActorIterator<ARewindCameraRig> It(World); It; ++It)
	{
		It->SnapToPlayer();
	}
}

FTransform URewindWorldStateSubsystem::GetLoopStartPose() const
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		return FTransform::Identity;
	}

	APlayerStart* Tagged = nullptr;
	APlayerStart* First = nullptr;
	for (TActorIterator<APlayerStart> It(World); It; ++It)
	{
		APlayerStart* Start = *It;
		if (!First)
		{
			First = Start;
		}
		if (Start->PlayerStartTag == FName(TEXT("LoopStart")))
		{
			Tagged = Start;
			break;
		}
	}

	if (const APlayerStart* Chosen = Tagged ? Tagged : First)
	{
		return Chosen->GetActorTransform();
	}

	return FTransform::Identity;
}
