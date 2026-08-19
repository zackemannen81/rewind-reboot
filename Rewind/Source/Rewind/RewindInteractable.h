#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RewindInteractable.generated.h"

UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class URewindInteractable : public UInterface
{
	GENERATED_BODY()
};

class REWIND_API IRewindInteractable
{
	GENERATED_BODY()

public:
	virtual bool TryInteract(APawn* InstigatorPawn) = 0;
	virtual void ReceiveDigit(int32 Digit) {}
};
