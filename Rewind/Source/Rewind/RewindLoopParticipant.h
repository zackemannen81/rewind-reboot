#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RewindLoopParticipant.generated.h"

UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class URewindLoopParticipant : public UInterface
{
	GENERATED_BODY()
};

class REWIND_API IRewindLoopParticipant
{
	GENERATED_BODY()

public:
	virtual void RestoreFromBaseline() = 0;
	virtual void ApplyAnchorOverride(FName AnchorId) = 0;
	virtual bool IsPlayerBody() const { return false; }
};
