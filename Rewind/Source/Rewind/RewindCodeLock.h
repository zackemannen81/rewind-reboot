#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RewindInteractable.h"
#include "RewindLoopParticipant.h"
#include "RewindCodeLock.generated.h"

UCLASS()
class REWIND_API ARewindCodeLock : public AActor, public IRewindInteractable, public IRewindLoopParticipant
{
	GENERATED_BODY()

public:
	ARewindCodeLock();
	virtual bool TryInteract(APawn* InstigatorPawn) override;
	virtual void ReceiveDigit(int32 Digit) override;
	virtual void RestoreFromBaseline() override;
	virtual void ApplyAnchorOverride(FName AnchorId) override;

private:
	void Submit(const FString& Code);
	void SetLocked(bool bLocked);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	FString Buffer;
	bool bLocked = true;
};
