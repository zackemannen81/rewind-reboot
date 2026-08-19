#include "RewindLoopSubsystem.h"

#include "RewindDeveloperSettings.h"
#include "RewindSessionSubsystem.h"
#include "RewindWorldStateSubsystem.h"
#include "Engine/World.h"
#include "Engine/GameInstance.h"
#include "HAL/IConsoleManager.h"

bool URewindLoopSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	const UWorld* World = Cast<UWorld>(Outer);
	return World && World->IsGameWorld();
}

void URewindLoopSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	RegisterConsoleCommands();
}

void URewindLoopSubsystem::Deinitialize()
{
	UnregisterConsoleCommands();
	Super::Deinitialize();
}

void URewindLoopSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	StartLoop();
}

void URewindLoopSubsystem::Tick(float DeltaTime)
{
	if (!Clock.IsRunning())
	{
		return;
	}

	Clock.Advance(static_cast<double>(DeltaTime));
	if (Clock.GetElapsedSeconds() >= static_cast<double>(GetLoopDurationSeconds()))
	{
		EndLoop(ERewindLoopEndReason::Timer);
	}
}

TStatId URewindLoopSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(URewindLoopSubsystem, STATGROUP_Tickables);
}

void URewindLoopSubsystem::StartLoop()
{
	if (URewindWorldStateSubsystem* WorldState = GetWorld()->GetSubsystem<URewindWorldStateSubsystem>())
	{
		WorldState->ApplyLoopStart();
	}

	Clock.Start();
}

void URewindLoopSubsystem::EndLoop(ERewindLoopEndReason Reason)
{
	Clock.Stop();

	if (UGameInstance* GameInstance = GetWorld()->GetGameInstance())
	{
		if (URewindSessionSubsystem* Session = GameInstance->GetSubsystem<URewindSessionSubsystem>())
		{
			Session->WriteOnLoopEnd();
		}
	}

	(void)Reason;
	StartLoop();
}

void URewindLoopSubsystem::NotifyPlayerDied()
{
	if (Clock.IsRunning())
	{
		EndLoop(ERewindLoopEndReason::Death);
	}
}

void URewindLoopSubsystem::CleanSaveAndRestart()
{
	if (UGameInstance* GameInstance = GetWorld()->GetGameInstance())
	{
		if (URewindSessionSubsystem* Session = GameInstance->GetSubsystem<URewindSessionSubsystem>())
		{
			Session->CleanSave();
		}
	}

	StartLoop();
}

float URewindLoopSubsystem::GetLoopDurationSeconds() const
{
	const URewindDeveloperSettings* Settings = GetDefault<URewindDeveloperSettings>();
	return Settings ? Settings->LoopDurationSeconds : 420.f;
}

void URewindLoopSubsystem::RegisterConsoleCommands()
{
	CleanSaveCommand = IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("Rewind.CleanSave"),
		TEXT("Empty knowledge and Anchors, then start a loop from baseline."),
		FConsoleCommandDelegate::CreateUObject(this, &URewindLoopSubsystem::CleanSaveAndRestart),
		ECVF_Default);

	EndLoopDeathCommand = IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("Rewind.EndLoopDeath"),
		TEXT("End the current loop as player death, write session, start the next loop."),
		FConsoleCommandDelegate::CreateLambda([this]()
		{
			NotifyPlayerDied();
		}),
		ECVF_Default);
}

void URewindLoopSubsystem::UnregisterConsoleCommands()
{
	if (CleanSaveCommand)
	{
		IConsoleManager::Get().UnregisterConsoleObject(CleanSaveCommand);
		CleanSaveCommand = nullptr;
	}
	if (EndLoopDeathCommand)
	{
		IConsoleManager::Get().UnregisterConsoleObject(EndLoopDeathCommand);
		EndLoopDeathCommand = nullptr;
	}
}
