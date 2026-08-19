#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "RewindWorldStateSubsystem.generated.h"

UCLASS()
class REWIND_API URewindWorldStateSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	void ApplyLoopStart();
	void PlacePlayerBody();
	FTransform GetLoopStartPose() const;
};
