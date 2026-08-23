#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "RewindDeveloperSettings.generated.h"

UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Rewind"))
class REWIND_API URewindDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = "Loop")
	float LoopDurationSeconds = 240.f;

	UPROPERTY(Config, EditAnywhere, Category = "Save")
	FString SaveSlotName = TEXT("RewindSession");
};
