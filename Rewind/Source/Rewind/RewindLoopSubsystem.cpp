#include "RewindLoopSubsystem.h"

#include "RewindLog.h"
#include "RewindDeveloperSettings.h"
#include "RewindFuse.h"
#include "RewindIds.h"
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
	if (Latch.IsLatched())
	{
		if (Clock.IsRunning())
		{
			Clock.Advance(static_cast<double>(DeltaTime));
		}
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				9001,
				0.2f,
				FColor::Green,
				FString::Printf(TEXT("t=%.1fs  REWIND %s"),
					Clock.GetElapsedSeconds(), ReasonName(LatchedReason)));
		}
		if (Latch.ShouldApply(Clock.GetElapsedSeconds()))
		{
			EndLoop(LatchedReason);
		}
		return;
	}

	if (!Clock.IsRunning())
	{
		return;
	}

	Clock.Advance(static_cast<double>(DeltaTime));
	if (GEngine)
	{
		if (UsesWholeSpaceDeadline())
		{
			GEngine->AddOnScreenDebugMessage(
				9001,
				0.2f,
				FColor::Green,
				FString::Printf(TEXT("t=%.1fs / %.0fs"),
					Clock.GetElapsedSeconds(), GetLoopDurationSeconds()));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(
				9001,
				0.2f,
				FColor::Green,
				FString::Printf(TEXT("t=%.1fs"), Clock.GetElapsedSeconds()));
		}
	}

	if (UsesWholeSpaceDeadline()
		&& Clock.GetElapsedSeconds() >= static_cast<double>(GetLoopDurationSeconds()))
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
	Latch.Reset();
	LatchedReason = ERewindLoopEndReason::Death;

	if (UWorld* World = GetWorld())
	{
		if (URewindWorldStateSubsystem* WorldState = World->GetSubsystem<URewindWorldStateSubsystem>())
		{
			WorldState->ApplyLoopStart();
		}
	}

	Clock.Start();

	// Logged after the clock restarts so the line reads t=0.00, and after the
	// apply order so the participants have already reported their baseline.
	FString Anchor = TEXT("None");
	int32 Knowledge = 0;
	if (UWorld* World = GetWorld())
	{
		if (UGameInstance* GameInstance = World->GetGameInstance())
		{
			if (const URewindSessionSubsystem* Session = GameInstance->GetSubsystem<URewindSessionSubsystem>())
			{
				Anchor = Session->GetActiveAnchor().ToString();
				Knowledge = Session->GetKnowledgeCount();
			}
		}
	}

	if (UsesWholeSpaceDeadline())
	{
		RewindLog::Event(this, FString::Printf(
			TEXT("LOOP START  duration=%.0fs  knowledge=%d  anchor=%s"),
			GetLoopDurationSeconds(), Knowledge, *Anchor));
	}
	else
	{
		RewindLog::Event(this, FString::Printf(
			TEXT("LOOP START  knowledge=%d  anchor=%s"), Knowledge, *Anchor));
	}
}

void URewindLoopSubsystem::EndLoop(ERewindLoopEndReason Reason)
{
	Latch.Reset();
	Clock.Stop();

	RewindLog::Event(this, FString::Printf(TEXT("LOOP END  reason=%s"), ReasonName(Reason)));

	if (UWorld* World = GetWorld())
	{
		if (UGameInstance* GameInstance = World->GetGameInstance())
		{
			if (URewindSessionSubsystem* Session = GameInstance->GetSubsystem<URewindSessionSubsystem>())
			{
				Session->WriteOnLoopEnd();

				RewindLog::Event(this, FString::Printf(
					TEXT("SESSION WRITE  knowledge=%d  anchor=%s"),
					Session->GetKnowledgeCount(), *Session->GetActiveAnchor().ToString()));
			}
		}
	}

	StartLoop();
}

void URewindLoopSubsystem::NotifyPlayerDied()
{
	if (Clock.IsRunning() || Latch.IsLatched())
	{
		EndLoop(ERewindLoopEndReason::Death);
	}
}

void URewindLoopSubsystem::NotifyCausalContractFailed(FName Checkpoint)
{
	TryLatchRewind(ERewindLoopEndReason::CausalContract, Checkpoint);
}

void URewindLoopSubsystem::NotifyAnchorCommitted()
{
	TryLatchRewind(ERewindLoopEndReason::AnchorCommit, NAME_None);
}

void URewindLoopSubsystem::TryLatchRewind(ERewindLoopEndReason Reason, FName Checkpoint)
{
	if (!Clock.IsRunning() && !Latch.IsLatched())
	{
		Clock.Start();
	}

	if (!Latch.TryLatch(Checkpoint, Clock.GetElapsedSeconds(), GetConfiguredPreludeSeconds()))
	{
		return;
	}

	LatchedReason = Reason;
	RewindLog::Event(this, FString::Printf(
		TEXT("REWIND LATCH  reason=%s  checkpoint=%s  prelude=%.2fs  t=%.2f"),
		ReasonName(Reason),
		Checkpoint.IsNone() ? TEXT("None") : *Checkpoint.ToString(),
		Latch.PreludeDurationSeconds,
		Latch.RequestElapsedSeconds));
}

void URewindLoopSubsystem::CleanSaveAndRestart()
{
	Latch.Reset();

	if (UWorld* World = GetWorld())
	{
		if (UGameInstance* GameInstance = World->GetGameInstance())
		{
			if (URewindSessionSubsystem* Session = GameInstance->GetSubsystem<URewindSessionSubsystem>())
			{
				Session->CleanSave();

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
	}

	StartLoop();
}

float URewindLoopSubsystem::GetLoopDurationSeconds() const
{
	return GetDefault<URewindDeveloperSettings>()->LoopDurationSeconds;
}

bool URewindLoopSubsystem::UsesWholeSpaceDeadline() const
{
	const URewindDeveloperSettings* Settings = GetDefault<URewindDeveloperSettings>();
	return Settings && Settings->bUseWholeSpaceDeadline && Settings->LoopDurationSeconds > 0.f;
}

double URewindLoopSubsystem::GetLoopBreakIntensity() const
{
	if (!Latch.IsLatched())
	{
		return 0.0;
	}
	return FRewindLoopBreakMath::Intensity(
		Clock.GetElapsedSeconds(),
		Latch.RequestElapsedSeconds,
		Latch.PreludeDurationSeconds);
}

float URewindLoopSubsystem::GetConfiguredPreludeSeconds() const
{
	return GetDefault<URewindDeveloperSettings>()->RewindPreludeSeconds;
}

const TCHAR* URewindLoopSubsystem::ReasonName(ERewindLoopEndReason Reason)
{
	switch (Reason)
	{
	case ERewindLoopEndReason::CausalContract:
		return TEXT("CausalContract");
	case ERewindLoopEndReason::Death:
		return TEXT("Death");
	case ERewindLoopEndReason::AnchorCommit:
		return TEXT("AnchorCommit");
	case ERewindLoopEndReason::Timer:
		return TEXT("Timer");
	default:
		return TEXT("Unknown");
	}
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

	SetActiveAnchorCommand = IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("Rewind.SetActiveAnchor"),
		TEXT("Developer verification: set courtyard_gate_open or none, then restart the loop."),
		FConsoleCommandWithArgsDelegate::CreateUObject(this, &URewindLoopSubsystem::HandleSetActiveAnchor),
		ECVF_Default);

	PlacePlayerCommand = IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("Rewind.PlacePlayerForVerification"),
		TEXT("Developer verification: threshold, fuse or courtyard_socket."),
		FConsoleCommandWithArgsDelegate::CreateUObject(this, &URewindLoopSubsystem::HandlePlacePlayerForVerification),
		ECVF_Default);

	SeatFuseCommand = IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("Rewind.SeatFuseForVerification"),
		TEXT("Developer verification: seat the one fuse in courtyard or building."),
		FConsoleCommandWithArgsDelegate::CreateUObject(this, &URewindLoopSubsystem::HandleSeatFuseForVerification),
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
	if (SetActiveAnchorCommand)
	{
		IConsoleManager::Get().UnregisterConsoleObject(SetActiveAnchorCommand);
		SetActiveAnchorCommand = nullptr;
	}
	if (PlacePlayerCommand)
	{
		IConsoleManager::Get().UnregisterConsoleObject(PlacePlayerCommand);
		PlacePlayerCommand = nullptr;
	}
	if (SeatFuseCommand)
	{
		IConsoleManager::Get().UnregisterConsoleObject(SeatFuseCommand);
		SeatFuseCommand = nullptr;
	}
}

void URewindLoopSubsystem::HandleSetActiveAnchor(const TArray<FString>& Args)
{
	FName Wanted = NAME_None;
	if (Args.Num() >= 1)
	{
		const FString& Value = Args[0];
		if (Value.Equals(TEXT("none"), ESearchCase::IgnoreCase))
		{
			Wanted = NAME_None;
		}
		else if (Value.Equals(RewindIds::AnchorCourtyardGateOpen.ToString(), ESearchCase::IgnoreCase))
		{
			Wanted = RewindIds::AnchorCourtyardGateOpen;
		}
		else
		{
			RewindLog::Event(this, TEXT("SetActiveAnchor: refused, only courtyard_gate_open or none"));
			return;
		}
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	UGameInstance* GameInstance = World->GetGameInstance();
	URewindSessionSubsystem* Session = GameInstance
		? GameInstance->GetSubsystem<URewindSessionSubsystem>()
		: nullptr;
	if (!Session)
	{
		RewindLog::Event(this, TEXT("SetActiveAnchor: no session"));
		return;
	}

	if (!Session->TrySetActiveAnchorForVerification(Wanted))
	{
		RewindLog::Event(this, TEXT("SetActiveAnchor: refused, only courtyard_gate_open or none"));
		return;
	}

	RewindLog::Event(this, FString::Printf(
		TEXT("SetActiveAnchor: %s"),
		Wanted.IsNone() ? TEXT("none") : *Wanted.ToString()));
	StartLoop();
}

void URewindLoopSubsystem::HandlePlacePlayerForVerification(const TArray<FString>& Args)
{
	if (Args.Num() < 1)
	{
		RewindLog::Event(this, TEXT("PlacePlayerForVerification: refused, use threshold, fuse or courtyard_socket"));
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	URewindWorldStateSubsystem* WorldState = World->GetSubsystem<URewindWorldStateSubsystem>();
	if (!WorldState || !WorldState->PlacePlayerForVerification(FName(*Args[0])))
	{
		RewindLog::Event(this, TEXT("PlacePlayerForVerification: failed"));
	}
}

void URewindLoopSubsystem::HandleSeatFuseForVerification(const TArray<FString>& Args)
{
	ERewindFuseSocket Which = ERewindFuseSocket::Courtyard;
	if (Args.Num() >= 1 && Args[0].Equals(TEXT("building"), ESearchCase::IgnoreCase))
	{
		Which = ERewindFuseSocket::Building;
	}
	else if (Args.Num() >= 1 && !Args[0].Equals(TEXT("courtyard"), ESearchCase::IgnoreCase))
	{
		RewindLog::Event(this, TEXT("SeatFuseForVerification: refused, use courtyard or building"));
		return;
	}

	UWorld* World = GetWorld();
	ARewindFuse* Fuse = ARewindFuse::Find(World);
	if (!Fuse || !Fuse->SeatForVerification(Which))
	{
		RewindLog::Event(this, TEXT("SeatFuseForVerification: failed"));
		return;
	}
	RewindLog::Event(this, Which == ERewindFuseSocket::Courtyard
		? TEXT("SeatFuseForVerification: courtyard")
		: TEXT("SeatFuseForVerification: building"));
}
