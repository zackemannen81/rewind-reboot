#pragma once

#include "CoreMinimal.h"
#include "ToolsetRegistry/ToolsetDefinition.h"

#include "RewindPIEInputToolset.generated.h"

/** Result returned by every RE:WIND PIE input tool. */
USTRUCT(BlueprintType)
struct FRewindPIEInputResult
{
	GENERATED_BODY()

	/** True when the requested event was delivered or the requested state was read. */
	UPROPERTY(BlueprintReadOnly, Category = "Rewind PIE Input")
	bool bSuccess = false;

	/** Human-readable outcome or the reason the request could not run. */
	UPROPERTY(BlueprintReadOnly, Category = "Rewind PIE Input")
	FString Message;

	/** Whether an in-process PIE world currently exists. */
	UPROPERTY(BlueprintReadOnly, Category = "Rewind PIE Input")
	bool bPIERunning = false;

	/** The canonical Unreal key name involved in this request, when applicable. */
	UPROPERTY(BlueprintReadOnly, Category = "Rewind PIE Input")
	FString Key;

	/** Name of player zero's possessed pawn, when available. */
	UPROPERTY(BlueprintReadOnly, Category = "Rewind PIE Input")
	FString PawnName;

	/** Player zero's pawn location in centimetres, when available. */
	UPROPERTY(BlueprintReadOnly, Category = "Rewind PIE Input")
	FVector PlayerLocation = FVector::ZeroVector;

	/** Keys pressed through this toolset and not yet released through it. */
	UPROPERTY(BlueprintReadOnly, Category = "Rewind PIE Input")
	TArray<FString> HeldKeys;
};

/**
 * Project-owned input bridge for in-process PIE.
 *
 * Events enter through APlayerController::InputKey as simulated Unreal input,
 * so normal project mappings and InputComponent bindings handle them. This is
 * intentionally independent of desktop focus and Windows SendInput.
 */
UCLASS(BlueprintType, Hidden)
class URewindPIEInputToolset : public UToolsetDefinition
{
	GENERATED_BODY()

public:
	/** Press a named Unreal key and keep it held until ReleaseKey or ReleaseAllKeys. */
	UFUNCTION(meta = (AICallable), Category = "Rewind PIE Input")
	static FRewindPIEInputResult PressKey(const FString& KeyName);

	/** Release a named Unreal key. Releasing an untracked key is safe. */
	UFUNCTION(meta = (AICallable), Category = "Rewind PIE Input")
	static FRewindPIEInputResult ReleaseKey(const FString& KeyName);

	/** Deliver a press immediately followed by a release for one named Unreal key. */
	UFUNCTION(meta = (AICallable), Category = "Rewind PIE Input")
	static FRewindPIEInputResult TapKey(const FString& KeyName);

	/** Release every key currently held through this toolset. */
	UFUNCTION(meta = (AICallable), Category = "Rewind PIE Input")
	static FRewindPIEInputResult ReleaseAllKeys();

	/** Read PIE and player-zero state without changing input. */
	UFUNCTION(meta = (AICallable), Category = "Rewind PIE Input")
	static FRewindPIEInputResult GetPlayerState();

	/** Editor lifecycle hook; clears bookkeeping after PIE destroys its input stack. */
	static void ClearTrackedKeys();
};
