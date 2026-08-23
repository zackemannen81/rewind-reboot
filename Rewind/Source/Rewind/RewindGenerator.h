#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RewindInteractable.h"
#include "RewindLoopParticipant.h"
#include "RewindGenerator.generated.h"

class ARewindCourtyardGate;

UCLASS()
class REWIND_API ARewindGenerator : public AActor, public IRewindInteractable, public IRewindLoopParticipant
{
	GENERATED_BODY()

public:
	ARewindGenerator();
	virtual bool TryInteract(APawn* InstigatorPawn) override;
	virtual void RestoreFromBaseline() override;
	virtual void ApplyAnchorOverride(FName AnchorId) override;
	void SetGate(ARewindCourtyardGate* InGate);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY()
	TObjectPtr<ARewindCourtyardGate> Gate;

	bool bOnline = false;
};
