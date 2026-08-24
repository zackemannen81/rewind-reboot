#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "RewindCameraRig.generated.h"

class ARewindCameraRegion;

/**
 * The one camera. It reads the player's position, asks which region contains
 * it, and places itself by that region's rules.
 *
 * It writes nothing but its own transform. `docs/design/camera-and-movement.md`
 * requires that, and the reason is not tidiness: a camera that can write world
 * state is a camera that has entered the determinism model. Nothing here may
 * ever touch a loop clock, a puzzle flag, or a participant.
 *
 * Per-frame delta is used for smoothing, which `loop-and-determinism.md`
 * permits: frame delta may move a presentation, not a phase.
 */
UCLASS()
class REWIND_API ARewindCameraRig : public ACameraActor
{
	GENERATED_BODY()

public:
	ARewindCameraRig();

	virtual void Tick(float DeltaSeconds) override;

	/** Take the active region's pose immediately. Called at loop start and on an authored cut. */
	void SnapToPlayer();

	/** The region the player is currently in, or null. Used by movement to align input to the frame. */
	ARewindCameraRegion* GetActiveRegion() const { return ActiveRegion; }

protected:
	virtual void BeginPlay() override;

	/** Fraction of the remaining distance covered per 16.67 ms. */
	UPROPERTY(EditAnywhere, Category = "Rig")
	double FollowSpeed = 0.12;

	/** Rotation blend on a handoff, per 16.67 ms. Faster than the follow, so angle settles first. */
	UPROPERTY(EditAnywhere, Category = "Rig")
	double RotationBlendSpeed = 0.09;

private:
	const APawn* GetPlayer() const;
	bool ResolveTarget(
		FRotator& OutRotation,
		float& OutFieldOfView,
		double& OutTravel);

	UPROPERTY()
	TObjectPtr<ARewindCameraRegion> ActiveRegion;

	/** Camera coordinate along the active region's travel axis. */
	double CurrentTravel = 0.0;

	bool bHasTravel = false;
};
