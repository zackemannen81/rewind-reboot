#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RewindLoopParticipant.h"
#include "RewindPatrol.generated.h"

UCLASS()
class REWIND_API ARewindPatrol : public AActor, public IRewindLoopParticipant
{
	GENERATED_BODY()

public:
	ARewindPatrol();
	virtual void Tick(float DeltaSeconds) override;
	virtual void RestoreFromBaseline() override;
	virtual void ApplyAnchorOverride(FName AnchorId) override;

private:
	void ApplyPhase(double ElapsedSeconds);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Body;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> PathBlock;
};
