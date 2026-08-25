#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RewindCausalCheckpoint.generated.h"

UENUM()
enum class ERewindCheckpointPredicate : uint8
{
	GroundFuseGate
};

/**
 * A named no-return causal checkpoint, per ADR-0009 and
 * `docs/design/chapter-1-authored.md`.
 *
 * The contract is evaluated when the player crosses the volume, not
 * continuously while its prerequisite is still solvable. A failed crossing
 * latches rewind. Leaving the volume does not cancel it.
 */
UCLASS()
class REWIND_API ARewindCausalCheckpoint : public AActor
{
	GENERATED_BODY()

public:
	ARewindCausalCheckpoint();

	void Configure(FName InCheckpointId, ERewindCheckpointPredicate InPredicate, const FVector& BoxExtent);

	FName GetCheckpointId() const { return CheckpointId; }
	ERewindCheckpointPredicate GetPredicate() const { return Predicate; }

	/** The accepted GroundFuseGate predicate, or true for an unknown id. */
	bool EvaluatePredicate() const;

	/** Crossing. Tests call this; overlap also calls it. */
	void NotifyCrossing();

	/** Stepping back. Must not cancel a latched rewind. */
	void NotifyExited();

private:
	UFUNCTION()
	void OnBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	bool IsPlayerPawn(const AActor* Actor) const;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UBoxComponent> Volume;

	UPROPERTY(EditAnywhere, Category = "Checkpoint")
	FName CheckpointId = NAME_None;

	UPROPERTY(EditAnywhere, Category = "Checkpoint")
	ERewindCheckpointPredicate Predicate = ERewindCheckpointPredicate::GroundFuseGate;
};
