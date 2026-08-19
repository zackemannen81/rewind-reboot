#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RewindInteractable.h"
#include "RewindAnchorBoard.generated.h"

UCLASS()
class REWIND_API ARewindAnchorBoard : public AActor, public IRewindInteractable
{
	GENERATED_BODY()

public:
	ARewindAnchorBoard();
	virtual bool TryInteract(APawn* InstigatorPawn) override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;
};
