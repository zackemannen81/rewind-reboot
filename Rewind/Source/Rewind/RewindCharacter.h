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

	/** Screen axes for input, taken from the region the player is standing in. */
	bool GetScreenAxes(FVector& OutRight, FVector& OutDepth) const;

	/**
	 * Placeholder body. The character has never had a mesh, which was invisible
	 * while the camera sat behind its head and is not now. This is scaffolding
	 * until the mannequin in `docs/backlog/character-animation-source.md` is
	 * imported, and it exists so the chain can be played before then.
	 */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> BodyPlaceholder;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UStaticMeshComponent> FacingPlaceholder;
};
