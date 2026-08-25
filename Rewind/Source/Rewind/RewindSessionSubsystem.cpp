#include "RewindSessionSubsystem.h"

#include "RewindDeveloperSettings.h"
#include "RewindIds.h"
#include "RewindSaveGame.h"
#include "Kismet/GameplayStatics.h"

void URewindSessionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LoadSession();
}

void URewindSessionSubsystem::ObtainKnowledge(FName Fact)
{
	if (!Fact.IsNone())
	{
		KnowledgeFacts.Add(Fact);
	}
}

bool URewindSessionSubsystem::HasKnowledge(FName Fact) const
{
	return KnowledgeFacts.Contains(Fact);
}

bool URewindSessionSubsystem::TryCommitCourtyardGateAnchor(bool bGateOpenFromThisLoopPlay)
{
	if (ActiveAnchor == RewindIds::AnchorCourtyardGateOpen)
	{
		return true;
	}

	if (!bGateOpenFromThisLoopPlay)
	{
		return false;
	}

	bPendingCourtyardGateCommit = true;
	return true;
}

void URewindSessionSubsystem::WriteOnLoopEnd()
{
	if (bPendingCourtyardGateCommit)
	{
		ActiveAnchor = RewindIds::AnchorCourtyardGateOpen;
		bPendingCourtyardGateCommit = false;
	}
	Persist();
}

void URewindSessionSubsystem::LoadSession()
{
	KnowledgeFacts.Reset();
	ActiveAnchor = NAME_None;
	bPendingCourtyardGateCommit = false;

	if (!UGameplayStatics::DoesSaveGameExist(GetSlotName(), 0))
	{
		return;
	}

	const URewindSaveGame* Save = Cast<URewindSaveGame>(
		UGameplayStatics::LoadGameFromSlot(GetSlotName(), 0));
	if (!Save)
	{
		return;
	}

	for (const FName Fact : Save->KnowledgeFacts)
	{
		KnowledgeFacts.Add(Fact);
	}

	if (Save->ActiveAnchor == RewindIds::AnchorCourtyardGateOpen)
	{
		ActiveAnchor = Save->ActiveAnchor;
	}
}

bool URewindSessionSubsystem::TrySetActiveAnchorForVerification(FName AnchorId)
{
	if (!AnchorId.IsNone() && AnchorId != RewindIds::AnchorCourtyardGateOpen)
	{
		return false;
	}

	ActiveAnchor = AnchorId;
	bPendingCourtyardGateCommit = false;
	if (GetGameInstance())
	{
		Persist();
	}
	return true;
}

void URewindSessionSubsystem::CleanSave()
{
	KnowledgeFacts.Reset();
	ActiveAnchor = NAME_None;
	bPendingCourtyardGateCommit = false;
	Persist();
}

bool URewindSessionSubsystem::IsClean() const
{
	return KnowledgeFacts.Num() == 0 && ActiveAnchor.IsNone() && !bPendingCourtyardGateCommit;
}

void URewindSessionSubsystem::Persist() const
{
	URewindSaveGame* Save = Cast<URewindSaveGame>(
		UGameplayStatics::CreateSaveGameObject(URewindSaveGame::StaticClass()));
	if (!Save)
	{
		return;
	}

	Save->KnowledgeFacts = KnowledgeFacts.Array();
	Save->ActiveAnchor = ActiveAnchor;
	UGameplayStatics::SaveGameToSlot(Save, GetSlotName(), 0);
}

FString URewindSessionSubsystem::GetSlotName() const
{
	const URewindDeveloperSettings* Settings = GetDefault<URewindDeveloperSettings>();
	return Settings ? Settings->SaveSlotName : FString(TEXT("RewindSession"));
}
