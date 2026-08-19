#pragma once

#include "CoreMinimal.h"
#include "RewindLoopClock.h"
#include "Subsystems/WorldSubsystem.h"
#include "RewindLoopSubsystem.generated.h"

UENUM()
enum class ERewindLoopEndReason : uint8
{
	Timer,
	Death
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
	void CleanSaveAndRestart();

	double GetElapsedLoopTime() const { return Clock.GetElapsedSeconds(); }
	float GetLoopDurationSeconds() const;
	bool IsLoopActive() const { return Clock.IsRunning(); }

private:
	void RegisterConsoleCommands();
	void UnregisterConsoleCommands();

	FRewindLoopClock Clock;
	IConsoleCommand* CleanSaveCommand = nullptr;
	IConsoleCommand* EndLoopDeathCommand = nullptr;
};
