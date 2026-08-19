#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RewindLoopParticipant.h"
#include "RewindTurnstile.generated.h"

UCLASS()
class REWIND_API ARewindTurnstile : public AActor, public IRewindLoopParticipant
{
	GENERATED_BODY()

public:
	ARewindTurnstile();
	virtual void Tick(float DeltaSeconds) override;
	virtual void RestoreFromBaseline() override;
	virtual void ApplyAnchorOverride(FName AnchorId) override;

private:
	void ApplyPhase(double ElapsedSeconds);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;
};
