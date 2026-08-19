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

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
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

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UCameraComponent> FollowCamera;
};
