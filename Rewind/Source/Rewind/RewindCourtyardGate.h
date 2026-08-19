#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RewindLoopParticipant.h"
#include "RewindCourtyardGate.generated.h"

UCLASS()
class REWIND_API ARewindCourtyardGate : public AActor, public IRewindLoopParticipant
{
	GENERATED_BODY()

public:
	ARewindCourtyardGate();
	virtual void RestoreFromBaseline() override;
	virtual void ApplyAnchorOverride(FName AnchorId) override;

	void OpenFromThisLoopPlay();
	bool IsOpen() const { return bOpen; }
	bool WasOpenedByThisLoopPlay() const { return bOpenedByThisLoopPlay; }

private:
	void SetOpen(bool bInOpen);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	bool bOpen = false;
	bool bOpenedByThisLoopPlay = false;
};
