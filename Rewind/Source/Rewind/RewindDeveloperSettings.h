#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "RewindDeveloperSettings.generated.h"

UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Rewind"))
class REWIND_API URewindDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	/** Authored whole-space deadline. Off by default; ADR-0009 forbids it as the automatic end. */
	UPROPERTY(Config, EditAnywhere, Category = "Loop")
	bool bUseWholeSpaceDeadline = false;

	/** Seconds used only when bUseWholeSpaceDeadline is true. */
	UPROPERTY(Config, EditAnywhere, Category = "Loop")
	float LoopDurationSeconds = 240.f;

	/** Latched rewind prelude, clamped to [1, 3] at latch time. */
	UPROPERTY(Config, EditAnywhere, Category = "Loop", meta = (ClampMin = "1.0", ClampMax = "3.0"))
	float RewindPreludeSeconds = 2.f;

	UPROPERTY(Config, EditAnywhere, Category = "Save")
	FString SaveSlotName = TEXT("RewindSession");
};
