#include "RewindCharacter.h"

#include "RewindInteractable.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "WorldCollision.h"
#include "Engine/OverlapResult.h"

ARewindCharacter::ARewindCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void ARewindCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ARewindCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ARewindCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ARewindCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ARewindCharacter::LookUp);
	PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &ARewindCharacter::Interact);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindKey(EKeys::Zero, IE_Pressed, this, &ARewindCharacter::Digit0);
	PlayerInputComponent->BindKey(EKeys::One, IE_Pressed, this, &ARewindCharacter::Digit1);
	PlayerInputComponent->BindKey(EKeys::Two, IE_Pressed, this, &ARewindCharacter::Digit2);
	PlayerInputComponent->BindKey(EKeys::Three, IE_Pressed, this, &ARewindCharacter::Digit3);
	PlayerInputComponent->BindKey(EKeys::Four, IE_Pressed, this, &ARewindCharacter::Digit4);
	PlayerInputComponent->BindKey(EKeys::Five, IE_Pressed, this, &ARewindCharacter::Digit5);
	PlayerInputComponent->BindKey(EKeys::Six, IE_Pressed, this, &ARewindCharacter::Digit6);
	PlayerInputComponent->BindKey(EKeys::Seven, IE_Pressed, this, &ARewindCharacter::Digit7);
	PlayerInputComponent->BindKey(EKeys::Eight, IE_Pressed, this, &ARewindCharacter::Digit8);
	PlayerInputComponent->BindKey(EKeys::Nine, IE_Pressed, this, &ARewindCharacter::Digit9);
}

void ARewindCharacter::MoveForward(float Value)
{
	if (Controller && Value != 0.f)
	{
		AddMovementInput(FRotationMatrix(FRotator(0.f, GetControlRotation().Yaw, 0.f)).GetUnitAxis(EAxis::X), Value);
	}
}

void ARewindCharacter::MoveRight(float Value)
{
	if (Controller && Value != 0.f)
	{
		AddMovementInput(FRotationMatrix(FRotator(0.f, GetControlRotation().Yaw, 0.f)).GetUnitAxis(EAxis::Y), Value);
	}
}

void ARewindCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ARewindCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void ARewindCharacter::Interact()
{
	if (IRewindInteractable* Target = FindInteractable())
	{
		Target->TryInteract(this);
	}
}

void ARewindCharacter::EnterDigit(int32 Digit)
{
	if (IRewindInteractable* Target = FindInteractable())
	{
		Target->ReceiveDigit(Digit);
	}
}

IRewindInteractable* ARewindCharacter::FindInteractable() const
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return nullptr;
	}

	TArray<FOverlapResult> Hits;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(RewindInteract), false, this);
	World->OverlapMultiByChannel(
		Hits,
		GetActorLocation(),
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeSphere(160.f),
		Params);

	for (const FOverlapResult& Hit : Hits)
	{
		if (IRewindInteractable* Interactable = Cast<IRewindInteractable>(Hit.GetActor()))
		{
			return Interactable;
		}
	}
	return nullptr;
}
