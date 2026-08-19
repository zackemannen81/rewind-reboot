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
