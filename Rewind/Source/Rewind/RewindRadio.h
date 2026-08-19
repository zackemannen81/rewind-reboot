#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RewindInteractable.h"
#include "RewindRadio.generated.h"

UCLASS()
class REWIND_API ARewindRadio : public AActor, public IRewindInteractable
{
	GENERATED_BODY()

public:
	ARewindRadio();
	virtual bool TryInteract(APawn* InstigatorPawn) override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;
};
