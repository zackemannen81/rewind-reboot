#include "RewindLoopSubsystem.h"

#include "RewindLog.h"
#include "RewindDeveloperSettings.h"
#include "RewindSessionSubsystem.h"
#include "RewindWorldStateSubsystem.h"
#include "Engine/Engine.h"
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
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			9001,
			0.2f,
			FColor::Green,
			FString::Printf(TEXT("t=%.1fs / %.0fs"), Clock.GetElapsedSeconds(), GetLoopDurationSeconds()));
	}
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

	// Logged after the clock restarts so the line reads t=0.00, and after the
	// apply order so the participants have already reported their baseline.
	FString Anchor = TEXT("None");
	int32 Knowledge = 0;
	if (UGameInstance* GameInstance = GetWorld()->GetGameInstance())
	{
		if (const URewindSessionSubsystem* Session = GameInstance->GetSubsystem<URewindSessionSubsystem>())
		{
			Anchor = Session->GetActiveAnchor().ToString();
			Knowledge = Session->GetKnowledgeCount();
		}
	}
	RewindLog::Event(this, FString::Printf(
		TEXT("LOOP START  duration=%.0fs  knowledge=%d  anchor=%s"),
		GetLoopDurationSeconds(), Knowledge, *Anchor));
}

void URewindLoopSubsystem::EndLoop(ERewindLoopEndReason Reason)
{
	Clock.Stop();

	// FL-02 asks that a loop end only by timer or by death. Logged before the
	// session write and before the next loop starts, so the reason and the
	// elapsed time it ended at are one line.
	RewindLog::Event(this, FString::Printf(TEXT("LOOP END  reason=%s"),
		Reason == ERewindLoopEndReason::Timer ? TEXT("Timer") : TEXT("Death")));

	if (UGameInstance* GameInstance = GetWorld()->GetGameInstance())
	{
		if (URewindSessionSubsystem* Session = GameInstance->GetSubsystem<URewindSessionSubsystem>())
		{
			Session->WriteOnLoopEnd();

			RewindLog::Event(this, FString::Printf(
				TEXT("SESSION WRITE  knowledge=%d  anchor=%s"),
				Session->GetKnowledgeCount(), *Session->GetActiveAnchor().ToString()));
		}
	}

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

			// FL-15 asks that a clean save be reachable. Without a report the
			// only proof is indirect, so state the resulting session on screen
			// and let the run record the observation rather than infer it.
			const FString Report = FString::Printf(
				TEXT("CLEAN SAVE  knowledge=%d  anchor=%s  -> %s"),
				Session->GetKnowledgeCount(),
				*Session->GetActiveAnchor().ToString(),
				Session->IsClean() ? TEXT("CLEAN") : TEXT("NOT CLEAN"));

			RewindLog::Event(this, Report);
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 6.f,
					Session->IsClean() ? FColor::Green : FColor::Red, Report);
			}
		}
	}

	StartLoop();
}

float URewindLoopSubsystem::GetLoopDurationSeconds() const
{
	// GetDefault never returns null, so the settings object is the only place
	// this number lives in code. It had a second copy here as a fallback, and a
	// number in two places drifts: the design said 240 while this said 420.
	// `chapter-1-authored.md` owns the value.
	return GetDefault<URewindDeveloperSettings>()->LoopDurationSeconds;
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
