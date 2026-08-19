#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RewindSaveGame.generated.h"

UCLASS()
class REWIND_API URewindSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<FName> KnowledgeFacts;

	UPROPERTY()
	FName ActiveAnchor;
};
