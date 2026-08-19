#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RewindInteractable.h"
#include "RewindLoopParticipant.h"
#include "RewindFuseBox.generated.h"

UCLASS()
class REWIND_API ARewindFuseBox : public AActor, public IRewindInteractable, public IRewindLoopParticipant
{
	GENERATED_BODY()

public:
	ARewindFuseBox();
	virtual bool TryInteract(APawn* InstigatorPawn) override;
	virtual void RestoreFromBaseline() override;
	virtual void ApplyAnchorOverride(FName AnchorId) override;
	bool IsCourtyardPowerOn() const { return bCourtyardPowerOn; }

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	bool bHasFuse = true;
	bool bCourtyardPowerOn = false;
};
