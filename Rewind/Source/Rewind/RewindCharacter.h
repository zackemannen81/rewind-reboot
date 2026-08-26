#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RewindInteractable.h"
#include "RewindCharacter.generated.h"

UCLASS()
class REWIND_API ARewindCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ARewindCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Interact();
	void UpdateFootsteps();
	void PlayFootstep();
	void EnterDigit(int32 Digit);
	void Digit0() { EnterDigit(0); }
	void Digit1() { EnterDigit(1); }
	void Digit2() { EnterDigit(2); }
	void Digit3() { EnterDigit(3); }
	void Digit4() { EnterDigit(4); }
	void Digit5() { EnterDigit(5); }
	void Digit6() { EnterDigit(6); }
	void Digit7() { EnterDigit(7); }
	void Digit8() { EnterDigit(8); }
	void Digit9() { EnterDigit(9); }

	IRewindInteractable* FindInteractable() const;

	/**
	 * Screen axes for input, taken from the region the player is standing in,
	 * or from the last region that did when the player is outside every volume.
	 *
	 * It never fails. It used to return false outside every region, and
	 * `MoveForward` and `MoveRight` then dropped the input, so a player who
	 * left the authored volumes kept the picture and lost the controller.
	 */
	bool GetScreenAxes(FVector& OutRight, FVector& OutDepth);

	/** The last region that contained the player. The fallback frame for input. */
	UPROPERTY()
	TObjectPtr<class ARewindCameraRegion> LastKnownRegion;

	/** Latched so the authoring gap is logged on entry and exit, not every frame. */
	bool bOutsideEveryRegion = false;

	/**
	 * Debug body visible only if the runtime skeletal mesh cannot load.
	 */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> BodyPlaceholder;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> FacingPlaceholder;

	/** The Returner skeletal body bound by REW-0029. */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USkeletalMeshComponent> ReturnerBody;

	UPROPERTY()
	TObjectPtr<class UAnimSequence> IdleAnimation;

	UPROPERTY()
	TObjectPtr<class UAnimSequence> WalkAnimation;

	bool bWasMoving = false;

	UPROPERTY()
	TObjectPtr<class USoundBase> FootstepSounds[6];

	FVector LastFootstepLocation = FVector::ZeroVector;
	float FootstepDistance = 0.0f;
	int32 PreviousFootstepIndex = INDEX_NONE;
	bool bHasFootstepLocation = false;

	// A held axis keeps the world direction it acquired when pressed. Without
	// this latch, an opposing authored camera cut reverses the same held key at
	// the threshold and traps the player on the seam.
	FVector LatchedForwardDirection = FVector::ZeroVector;
	FVector LatchedRightDirection = FVector::ZeroVector;
	float LastForwardValue = 0.f;
	float LastRightValue = 0.f;
};
