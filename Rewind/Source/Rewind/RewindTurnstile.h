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
	void CheckHubEntry();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	/** Last open state written to the log. Instrumentation, not world state. */
	bool bOpenLogged = true;

	/** Current open state, read by the hub-entry line. Instrumentation. */
	bool bOpenNow = true;

	/** Whether this loop's hub entry is already logged. Instrumentation. */
	bool bHubEntryLogged = false;
};
