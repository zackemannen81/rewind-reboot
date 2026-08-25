#pragma once

#include "CoreMinimal.h"
#include "RewindLatchedRewind.h"
#include "RewindLoopClock.h"
#include "Subsystems/WorldSubsystem.h"
#include "RewindLoopSubsystem.generated.h"

UENUM()
enum class ERewindLoopEndReason : uint8
{
	CausalContract,
	Death,
	AnchorCommit,
	Timer
};

UCLASS()
class REWIND_API URewindLoopSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;

	void StartLoop();
	void EndLoop(ERewindLoopEndReason Reason);
	void NotifyPlayerDied();
	void NotifyCausalContractFailed(FName Checkpoint);
	void NotifyAnchorCommitted();
	void CleanSaveAndRestart();

	double GetElapsedLoopTime() const { return Clock.GetElapsedSeconds(); }
	float GetLoopDurationSeconds() const;
	bool UsesWholeSpaceDeadline() const;
	bool IsLoopActive() const { return Clock.IsRunning(); }

	bool IsRewindLatched() const { return Latch.IsLatched(); }
	ERewindLoopEndReason GetLatchedReason() const { return LatchedReason; }
	FName GetLatchedCheckpoint() const { return Latch.Checkpoint; }
	double GetLatchedRequestElapsed() const { return Latch.RequestElapsedSeconds; }
	double GetLatchedPreludeDuration() const { return Latch.PreludeDurationSeconds; }

	/** Rising interference in [0, 1] during a latched prelude, else 0. Reads the loop clock. */
	double GetLoopBreakIntensity() const;

	static const TCHAR* ReasonName(ERewindLoopEndReason Reason);

private:
	void RegisterConsoleCommands();
	void UnregisterConsoleCommands();
	void TryLatchRewind(ERewindLoopEndReason Reason, FName Checkpoint);
	float GetConfiguredPreludeSeconds() const;

	FRewindLoopClock Clock;
	FRewindLatchedRewind Latch;
	ERewindLoopEndReason LatchedReason = ERewindLoopEndReason::Death;
	IConsoleCommand* CleanSaveCommand = nullptr;
	IConsoleCommand* EndLoopDeathCommand = nullptr;
};
