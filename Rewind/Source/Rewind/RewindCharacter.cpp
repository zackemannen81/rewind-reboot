#include "RewindCharacter.h"

#include "RewindInteractable.h"
#include "RewindCameraRegion.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "WorldCollision.h"
#include "Engine/OverlapResult.h"

ARewindCharacter::ARewindCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	// ADR-0007: the player does not aim the camera, so controller rotation
	// drives nothing. The body turns to face where it walks instead, which is
	// what reads from an authored angle.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);

	// `chapter-1-authored.md`: 200 cm/s. It was 500, which is a sprint and was
	// never a decision. Travel time is a design resource here.
	GetCharacterMovement()->MaxWalkSpeed = 200.f;

	// The camera is ARewindCameraRig. This pawn owns none.
	AutoPossessPlayer = EAutoReceiveInput::Disabled;

	// A body to see. The capsule is 42 by 96, so the cylinder matches it and
	// the small block reads which way the character faces, which matters now
	// that the body turns toward movement and the camera does not.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cylinder(
		TEXT("/Engine/BasicShapes/Cylinder.Cylinder"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(
		TEXT("/Engine/BasicShapes/Cube.Cube"));

	BodyPlaceholder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyPlaceholder"));
	BodyPlaceholder->SetupAttachment(RootComponent);
	BodyPlaceholder->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BodyPlaceholder->SetRelativeScale3D(FVector(0.84f, 0.84f, 1.92f));
	if (Cylinder.Succeeded())
	{
		BodyPlaceholder->SetStaticMesh(Cylinder.Object);
	}

	FacingPlaceholder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FacingPlaceholder"));
	FacingPlaceholder->SetupAttachment(RootComponent);
	FacingPlaceholder->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FacingPlaceholder->SetRelativeLocation(FVector(34.f, 0.f, 62.f));
	FacingPlaceholder->SetRelativeScale3D(FVector(0.22f, 0.34f, 0.34f));
	if (Cube.Succeeded())
	{
		FacingPlaceholder->SetStaticMesh(Cube.Object);
	}
}

void ARewindCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// The player volume may be narrower than the region's collision geometry,
	// and where it is, that is authored rather than accidental. Clamping the
	// body is a write to PlayerBody, which `world-state-model.md` discards at
	// every loop start, so it is not world state.
	if (const ARewindCameraRegion* Region =
			ARewindCameraRegion::FindContaining(GetWorld(), GetActorLocation()))
	{
		const FVector Clamped = Region->ClampToPlayerVolume(GetActorLocation());
		if (!Clamped.Equals(GetActorLocation(), 0.01))
		{
			SetActorLocation(Clamped);
		}
	}
}

void ARewindCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::White,
			TEXT("RE:WIND 4C  |  WASD move  E interact  digits at lock"));
	}
}

void ARewindCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ARewindCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ARewindCharacter::MoveRight);
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

bool ARewindCharacter::GetScreenAxes(FVector& OutRight, FVector& OutDepth) const
{
	// Input is expressed against the frame the player is looking at, not
	// against world axes. Otherwise "right" stops meaning right on screen the
	// moment a region is angled differently, and an authored camera would make
	// the game harder to control rather than easier to read.
	const ARewindCameraRegion* Region =
		ARewindCameraRegion::FindContaining(GetWorld(), GetActorLocation());
	if (!Region)
	{
		return false;
	}
	OutRight = Region->GetScreenRight();
	OutDepth = Region->GetScreenDepth();
	return true;
}

void ARewindCharacter::MoveForward(float Value)
{
	if (!Controller || Value == 0.f)
	{
		return;
	}
	FVector Right, Depth;
	if (GetScreenAxes(Right, Depth))
	{
		// Forward moves into the frame, which is the depth the player volume
		// allows. `camera-and-movement.md` keeps that band narrow on purpose.
		AddMovementInput(Depth, Value);
	}
}

void ARewindCharacter::MoveRight(float Value)
{
	if (!Controller || Value == 0.f)
	{
		return;
	}
	FVector Right, Depth;
	if (GetScreenAxes(Right, Depth))
	{
		AddMovementInput(Right, Value);
	}
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
