#pragma once

#include "CoreMinimal.h"

struct FRewindMessageDef
{
	FString Text;
	float DefaultDurationSeconds = 0.f;
};

/**
 * Player-facing copy, held as data. Changing a line is a catalog edit, not an
 * actor edit. `docs/design/player-messages.md` owns the channel.
 */
class FRewindMessageCatalog
{
public:
	static const FRewindMessageDef* Find(FName Id);
	static FString Format(FName Id, const TArray<FString>& Args);
	static float DefaultDuration(FName Id);
	static const TMap<FName, FRewindMessageDef>& GetAll();
};
