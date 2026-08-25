#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RewindLoopBreakSignature.generated.h"

/**
 * Perceptible loop-break signature during a latched rewind prelude.
 *
 * Intensity is taken from `URewindLoopSubsystem::GetLoopBreakIntensity`, which
 * is a function of elapsed loop time. Frame delta is ignored.
 */
UCLASS()
class REWIND_API ARewindLoopBreakSignature : public AActor
{
	GENERATED_BODY()

public:
	ARewindLoopBreakSignature();

	virtual void Tick(float DeltaSeconds) override;

	double GetDisplayedIntensity() const { return DisplayedIntensity; }

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UPostProcessComponent> PostProcess;

	double DisplayedIntensity = 0.0;
	bool bLoggedVisible = false;
};
