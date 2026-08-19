#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RewindProofLayout.generated.h"

UCLASS()
class REWIND_API ARewindProofLayout : public AActor
{
	GENERATED_BODY()

public:
	ARewindProofLayout();
	void EnsureContents();
	void EnsureLights();

private:
	template <typename T>
	T* EnsureActor(TObjectPtr<T>& Cache, const FVector& Location);

	UPROPERTY()
	TObjectPtr<class ARewindFourCBlockout> Blockout;
	UPROPERTY()
	TObjectPtr<class ARewindRadio> Radio;
	UPROPERTY()
	TObjectPtr<class ARewindCodeLock> CodeLock;
	UPROPERTY()
	TObjectPtr<class ARewindFuseBox> FuseBox;
	UPROPERTY()
	TObjectPtr<class ARewindGenerator> Generator;
	UPROPERTY()
	TObjectPtr<class ARewindCourtyardGate> Gate;
	UPROPERTY()
	TObjectPtr<class ARewindAnchorBoard> Board;
	UPROPERTY()
	TObjectPtr<class ARewindPatrol> Patrol;
	UPROPERTY()
	TObjectPtr<class ARewindTurnstile> Turnstile;
};
