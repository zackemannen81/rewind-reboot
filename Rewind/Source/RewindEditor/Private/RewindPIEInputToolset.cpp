#include "RewindPIEInputToolset.h"

#include "Editor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "InputKeyEventArgs.h"
#include "Kismet/GameplayStatics.h"

namespace
{
	TSet<FKey> GRewindTrackedKeys;

	void AddObservedState(FRewindPIEInputResult& Result, APlayerController* PlayerController)
	{
		Result.bPIERunning = GEditor && GEditor->PlayWorld != nullptr;

		if (PlayerController)
		{
			if (const APawn* Pawn = PlayerController->GetPawn())
			{
				Result.PawnName = Pawn->GetName();
				Result.PlayerLocation = Pawn->GetActorLocation();
			}
		}

		for (const FKey& HeldKey : GRewindTrackedKeys)
		{
			Result.HeldKeys.Add(HeldKey.GetFName().ToString());
		}
		Result.HeldKeys.Sort();
	}

	APlayerController* GetPIEPlayerController(FRewindPIEInputResult& Result)
	{
		if (!GEditor || !GEditor->PlayWorld)
		{
			Result.Message = TEXT("No in-process PIE world is running. Start standard PIE, not Simulate In Editor.");
			AddObservedState(Result, nullptr);
			return nullptr;
		}

		APlayerController* PlayerController =
			UGameplayStatics::GetPlayerController(GEditor->PlayWorld, 0);
		if (!PlayerController)
		{
			Result.Message = TEXT("PIE is running, but player zero has no player controller. Do not use Simulate In Editor.");
			AddObservedState(Result, nullptr);
			return nullptr;
		}

		if (!PlayerController->GetPawn())
		{
			Result.Message = TEXT("PIE player zero has no possessed pawn yet. Wait for StartPIE warmup and retry.");
			AddObservedState(Result, PlayerController);
			return nullptr;
		}

		return PlayerController;
	}

	FKey ResolveKey(const FString& KeyName, FRewindPIEInputResult& Result)
	{
		const FString TrimmedName = KeyName.TrimStartAndEnd();
		const FKey Key{FName(*TrimmedName)};
		if (!Key.IsValid())
		{
			Result.Key = TrimmedName;
			Result.Message = FString::Printf(
				TEXT("'%s' is not a valid Unreal key name. Examples: W, A, S, D, E, SpaceBar, Zero."),
				*TrimmedName);
		}
		return Key;
	}

	void DeliverKey(APlayerController* PlayerController, const FKey& Key, EInputEvent Event)
	{
		const float AmountDepressed = Event == IE_Released ? 0.0f : 1.0f;
		PlayerController->InputKey(
			FInputKeyEventArgs::CreateSimulated(Key, Event, AmountDepressed));
	}

	FRewindPIEInputResult MakeKeyResult(
		const FString& KeyName,
		EInputEvent Event,
		bool bTrackAsHeld,
		const TCHAR* Verb)
	{
		FRewindPIEInputResult Result;
		APlayerController* PlayerController = GetPIEPlayerController(Result);
		if (!PlayerController)
		{
			return Result;
		}

		const FKey Key = ResolveKey(KeyName, Result);
		if (!Key.IsValid())
		{
			AddObservedState(Result, PlayerController);
			return Result;
		}

		DeliverKey(PlayerController, Key, Event);
		if (bTrackAsHeld)
		{
			GRewindTrackedKeys.Add(Key);
		}
		else
		{
			GRewindTrackedKeys.Remove(Key);
		}

		Result.bSuccess = true;
		Result.Key = Key.GetFName().ToString();
		Result.Message = FString::Printf(TEXT("%s %s through PIE player zero."), Verb, *Result.Key);
		AddObservedState(Result, PlayerController);
		return Result;
	}
}

FRewindPIEInputResult URewindPIEInputToolset::PressKey(const FString& KeyName)
{
	return MakeKeyResult(KeyName, IE_Pressed, true, TEXT("Pressed"));
}

FRewindPIEInputResult URewindPIEInputToolset::ReleaseKey(const FString& KeyName)
{
	return MakeKeyResult(KeyName, IE_Released, false, TEXT("Released"));
}

FRewindPIEInputResult URewindPIEInputToolset::TapKey(const FString& KeyName)
{
	FRewindPIEInputResult Result;
	APlayerController* PlayerController = GetPIEPlayerController(Result);
	if (!PlayerController)
	{
		return Result;
	}

	const FKey Key = ResolveKey(KeyName, Result);
	if (!Key.IsValid())
	{
		AddObservedState(Result, PlayerController);
		return Result;
	}

	DeliverKey(PlayerController, Key, IE_Pressed);
	DeliverKey(PlayerController, Key, IE_Released);
	GRewindTrackedKeys.Remove(Key);

	Result.bSuccess = true;
	Result.Key = Key.GetFName().ToString();
	Result.Message = FString::Printf(TEXT("Tapped %s through PIE player zero."), *Result.Key);
	AddObservedState(Result, PlayerController);
	return Result;
}

FRewindPIEInputResult URewindPIEInputToolset::ReleaseAllKeys()
{
	FRewindPIEInputResult Result;
	APlayerController* PlayerController = GetPIEPlayerController(Result);
	if (!PlayerController)
	{
		return Result;
	}

	TArray<FKey> KeysToRelease = GRewindTrackedKeys.Array();
	for (const FKey& Key : KeysToRelease)
	{
		DeliverKey(PlayerController, Key, IE_Released);
	}
	GRewindTrackedKeys.Reset();

	Result.bSuccess = true;
	Result.Message = FString::Printf(TEXT("Released %d project-injected held key(s)."), KeysToRelease.Num());
	AddObservedState(Result, PlayerController);
	return Result;
}

FRewindPIEInputResult URewindPIEInputToolset::GetPlayerState()
{
	FRewindPIEInputResult Result;
	APlayerController* PlayerController = GetPIEPlayerController(Result);
	if (!PlayerController)
	{
		return Result;
	}

	Result.bSuccess = true;
	Result.Message = TEXT("Read PIE player-zero state.");
	AddObservedState(Result, PlayerController);
	return Result;
}

void URewindPIEInputToolset::ClearTrackedKeys()
{
	GRewindTrackedKeys.Reset();
}
