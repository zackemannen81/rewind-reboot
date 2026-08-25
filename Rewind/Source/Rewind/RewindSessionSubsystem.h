#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "RewindSessionSubsystem.generated.h"

UCLASS()
class REWIND_API URewindSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void ObtainKnowledge(FName Fact);
	bool HasKnowledge(FName Fact) const;

	bool TryCommitCourtyardGateAnchor(bool bGateOpenFromThisLoopPlay);
	FName GetActiveAnchor() const { return ActiveAnchor; }

	/**
	 * Developer verification only. Accepts `courtyard_gate_open` or none,
	 * matching `LoadSession`. Not a player action and not a new Anchor rule.
	 */
	bool TrySetActiveAnchorForVerification(FName AnchorId);
	int32 GetKnowledgeCount() const { return KnowledgeFacts.Num(); }
	bool HasPendingAnchorCommit() const { return bPendingCourtyardGateCommit; }

	void WriteOnLoopEnd();
	void LoadSession();
	void CleanSave();

	bool IsClean() const;

private:
	void Persist() const;
	FString GetSlotName() const;

	TSet<FName> KnowledgeFacts;
	FName ActiveAnchor;
	bool bPendingCourtyardGateCommit = false;
};
