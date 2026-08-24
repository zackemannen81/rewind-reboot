#include "RewindPIEInputToolset.h"

#include "Camera/CameraComponent.h"
#include "Editor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "HAL/FileManager.h"
#include "InputKeyEventArgs.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/Paths.h"
#include "TimerManager.h"
#include "UObject/UnrealType.h"
#include "UnrealClient.h"

namespace
{
	TSet<FKey> GRewindTrackedKeys;
	uint64 GRewindSequenceGeneration = 0;

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

			if (AActor* ViewTarget = PlayerController->GetViewTarget())
			{
				Result.ViewTargetName = ViewTarget->GetName();
				Result.CameraLocation = ViewTarget->GetActorLocation();
				Result.CameraRotation = ViewTarget->GetActorRotation();

				if (const UCameraComponent* Camera = ViewTarget->FindComponentByClass<UCameraComponent>())
				{
					Result.CameraFieldOfView = Camera->FieldOfView;
				}

				// Keep the editor bridge independent of the runtime module's private
				// headers. These are evidence-only reads of reflected authored state.
				if (const FObjectProperty* ActiveRegionProperty =
					FindFProperty<FObjectProperty>(ViewTarget->GetClass(), TEXT("ActiveRegion")))
				{
					if (const UObject* Region = ActiveRegionProperty->GetObjectPropertyValue_InContainer(ViewTarget))
					{
						if (const FNameProperty* RegionNameProperty =
							FindFProperty<FNameProperty>(Region->GetClass(), TEXT("RegionName")))
						{
							Result.ActiveRegionName =
								RegionNameProperty->GetPropertyValue_InContainer(Region).ToString();
						}
						if (const FEnumProperty* TravelAxisProperty =
							FindFProperty<FEnumProperty>(Region->GetClass(), TEXT("TravelAxis")))
						{
							const void* AxisAddress = TravelAxisProperty->ContainerPtrToValuePtr<void>(Region);
							const int64 AxisValue = TravelAxisProperty->GetUnderlyingProperty()
								->GetSignedIntPropertyValue(AxisAddress);
							Result.ActiveTravelAxis = TravelAxisProperty->GetEnum()
								->GetNameStringByValue(AxisValue);
						}
					}
				}
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

FRewindPIEInputResult URewindPIEInputToolset::HoldKeyForSeconds(
	const FString& KeyName,
	float DurationSeconds)
{
	FRewindPIEInputResult Result;
	APlayerController* PlayerController = GetPIEPlayerController(Result);
	if (!PlayerController)
	{
		return Result;
	}

	const FKey Key = ResolveKey(KeyName, Result);
	if (!Key.IsValid() || !FMath::IsFinite(DurationSeconds) || DurationSeconds <= 0.f)
	{
		if (Key.IsValid())
		{
			Result.Key = Key.GetFName().ToString();
			Result.Message = TEXT("DurationSeconds must be finite and greater than zero.");
		}
		AddObservedState(Result, PlayerController);
		return Result;
	}

	DeliverKey(PlayerController, Key, IE_Pressed);
	GRewindTrackedKeys.Add(Key);
	TWeakObjectPtr<APlayerController> WeakController(PlayerController);
	FTimerHandle ReleaseTimer;
	PlayerController->GetWorldTimerManager().SetTimer(
		ReleaseTimer,
		FTimerDelegate::CreateLambda([WeakController, Key]()
		{
			if (APlayerController* Controller = WeakController.Get())
			{
				DeliverKey(Controller, Key, IE_Released);
			}
			GRewindTrackedKeys.Remove(Key);
		}),
		DurationSeconds,
		false);

	Result.bSuccess = true;
	Result.Key = Key.GetFName().ToString();
	Result.Message = FString::Printf(
		TEXT("Holding %s for %.3f seconds of PIE game time."), *Result.Key, DurationSeconds);
	AddObservedState(Result, PlayerController);
	return Result;
}

FRewindPIEInputResult URewindPIEInputToolset::QueueInputSequence(const FString& Sequence)
{
	FRewindPIEInputResult Result;
	APlayerController* PlayerController = GetPIEPlayerController(Result);
	if (!PlayerController)
	{
		return Result;
	}

	TArray<FString> Tokens;
	Sequence.ParseIntoArray(Tokens, TEXT(","), true);
	if (Tokens.IsEmpty())
	{
		Result.Message = TEXT("Sequence must contain at least one comma-separated action.");
		AddObservedState(Result, PlayerController);
		return Result;
	}

	struct FQueuedAction
	{
		FKey Key;
		float StartSeconds = 0.f;
		float DurationSeconds = 0.f;
		bool bTap = false;
	};
	TArray<FQueuedAction> Actions;
	float CursorSeconds = 0.f;
	for (FString Token : Tokens)
	{
		Token.TrimStartAndEndInline();
		FString Name;
		FString DurationText;
		const bool bHasDuration = Token.Split(TEXT(":"), &Name, &DurationText);
		if (!bHasDuration)
		{
			Name = Token;
		}
		Name.TrimStartAndEndInline();
		DurationText.TrimStartAndEndInline();

		if (Name.Equals(TEXT("Wait"), ESearchCase::IgnoreCase))
		{
			const float WaitSeconds = FCString::Atof(*DurationText);
			if (!bHasDuration || !FMath::IsFinite(WaitSeconds) || WaitSeconds <= 0.f)
			{
				Result.Message = FString::Printf(TEXT("Invalid wait token '%s'. Use Wait:seconds."), *Token);
				AddObservedState(Result, PlayerController);
				return Result;
			}
			CursorSeconds += WaitSeconds;
			continue;
		}

		const FKey Key = ResolveKey(Name, Result);
		if (!Key.IsValid())
		{
			AddObservedState(Result, PlayerController);
			return Result;
		}

		if (bHasDuration)
		{
			const float HoldSeconds = FCString::Atof(*DurationText);
			if (!FMath::IsFinite(HoldSeconds) || HoldSeconds <= 0.f)
			{
				Result.Message = FString::Printf(TEXT("Invalid hold token '%s'. Duration must be greater than zero."), *Token);
				AddObservedState(Result, PlayerController);
				return Result;
			}
			Actions.Add({Key, CursorSeconds, HoldSeconds, false});
			CursorSeconds += HoldSeconds;
		}
		else
		{
			Actions.Add({Key, CursorSeconds, 0.f, true});
			CursorSeconds += 0.1f;
		}
	}

	const uint64 Generation = ++GRewindSequenceGeneration;
	TWeakObjectPtr<APlayerController> WeakController(PlayerController);
	for (const FQueuedAction& Action : Actions)
	{
		auto Schedule = [PlayerController](float DelaySeconds, FTimerDelegate&& Delegate)
		{
			if (DelaySeconds <= KINDA_SMALL_NUMBER)
			{
				Delegate.ExecuteIfBound();
				return;
			}
			FTimerHandle Timer;
			PlayerController->GetWorldTimerManager().SetTimer(Timer, MoveTemp(Delegate), DelaySeconds, false);
		};

		Schedule(Action.StartSeconds, FTimerDelegate::CreateLambda(
			[WeakController, Action, Generation]()
			{
				if (Generation != GRewindSequenceGeneration)
				{
					return;
				}
				if (APlayerController* Controller = WeakController.Get())
				{
					DeliverKey(Controller, Action.Key, IE_Pressed);
					if (Action.bTap)
					{
						DeliverKey(Controller, Action.Key, IE_Released);
					}
					else
					{
						GRewindTrackedKeys.Add(Action.Key);
					}
				}
			}));

		if (!Action.bTap)
		{
			Schedule(Action.StartSeconds + Action.DurationSeconds,
				FTimerDelegate::CreateLambda([WeakController, Action, Generation]()
				{
					if (Generation != GRewindSequenceGeneration)
					{
						return;
					}
					if (APlayerController* Controller = WeakController.Get())
					{
						DeliverKey(Controller, Action.Key, IE_Released);
					}
					GRewindTrackedKeys.Remove(Action.Key);
				}));
		}
	}

	Result.bSuccess = true;
	Result.Message = FString::Printf(
		TEXT("Queued %d input action(s) across %.3f seconds of PIE game time."),
		Actions.Num(), CursorSeconds);
	AddObservedState(Result, PlayerController);
	return Result;
}

FRewindPIEInputResult URewindPIEInputToolset::ExecuteProjectConsoleCommand(const FString& Command)
{
	FRewindPIEInputResult Result;
	APlayerController* PlayerController = GetPIEPlayerController(Result);
	if (!PlayerController)
	{
		return Result;
	}

	const FString Trimmed = Command.TrimStartAndEnd();
	if (!Trimmed.StartsWith(TEXT("Rewind."), ESearchCase::CaseSensitive))
	{
		Result.Message = TEXT("Only project-owned Rewind.* console commands are allowed.");
		AddObservedState(Result, PlayerController);
		return Result;
	}

	PlayerController->ConsoleCommand(Trimmed, true);
	Result.bSuccess = true;
	Result.Message = FString::Printf(TEXT("Executed PIE console command: %s"), *Trimmed);
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

	++GRewindSequenceGeneration;
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

FRewindPIEInputResult URewindPIEInputToolset::CapturePIEScreenshot()
{
	FRewindPIEInputResult Result;
	APlayerController* PlayerController = GetPIEPlayerController(Result);
	if (!PlayerController)
	{
		return Result;
	}

	Result.ScreenshotPath = FPaths::ConvertRelativePathToFull(FPaths::Combine(
		FPaths::ProjectSavedDir(),
		TEXT("Screenshots"),
		TEXT("WindowsEditor"),
		TEXT("RewindPIE.png")));
	IFileManager::Get().MakeDirectory(*FPaths::GetPath(Result.ScreenshotPath), true);
	FScreenshotRequest::RequestScreenshot(Result.ScreenshotPath, false, false);

	Result.bSuccess = true;
	Result.Message = TEXT("Requested a clean PIE game-viewport screenshot on the next rendered frame.");
	AddObservedState(Result, PlayerController);
	return Result;
}

void URewindPIEInputToolset::ClearTrackedKeys()
{
	++GRewindSequenceGeneration;
	GRewindTrackedKeys.Reset();
}
