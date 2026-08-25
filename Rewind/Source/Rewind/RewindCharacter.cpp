#include "RewindCharacter.h"

#include "RewindInteractable.h"
#include "RewindCameraRegion.h"
#include "RewindLog.h"
#include "Animation/AnimSequence.h"
#include "GameFramework/PlayerController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "Materials/MaterialInterface.h"
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

	// REW-0007 Tier 1 import. The source is the CC0 Quaternius mannequin and
	// in-place UAL1 animations; movement remains CharacterMovement-owned.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Mannequin(
		TEXT("/Game/Characters/Tier1/UAL1/Tier1_UAL1/SkeletalMeshes/Tier1_UAL1.Tier1_UAL1"));
	static ConstructorHelpers::FObjectFinder<UAnimSequence> Idle(
		TEXT("/Game/Characters/Tier1/UAL1/Tier1_UAL1/SkeletalMeshes/Tier1_UAL1Idle_Loop.Tier1_UAL1Idle_Loop"));
	static ConstructorHelpers::FObjectFinder<UAnimSequence> Walk(
		TEXT("/Game/Characters/Tier1/UAL1/Tier1_UAL1/SkeletalMeshes/Tier1_UAL1Walk_Loop.Tier1_UAL1Walk_Loop"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Silhouette(
		TEXT("/Game/Art/Materials/Stairwell/MI_CharacterSilhouette.MI_CharacterSilhouette"));

	MannequinBody = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MannequinBody"));
	MannequinBody->SetupAttachment(RootComponent);
	MannequinBody->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// The imported mannequin is authored with its origin at the feet and faces
	// across the character's +X axis. ACharacter's root is the centre of its
	// capsule, so place the feet at the capsule bottom and align the mesh's
	// animation-forward direction with CharacterMovement.
	MannequinBody->SetRelativeLocation(FVector(
		0.f, 0.f, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
	MannequinBody->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	if (Mannequin.Succeeded())
	{
		MannequinBody->SetSkeletalMeshAsset(Mannequin.Object);
		IdleAnimation = Idle.Succeeded() ? Idle.Object : nullptr;
		WalkAnimation = Walk.Succeeded() ? Walk.Object : nullptr;
		MannequinBody->SetAnimationMode(EAnimationMode::AnimationSingleNode);
		if (Silhouette.Succeeded())
		{
			for (int32 MaterialIndex = 0; MaterialIndex < MannequinBody->GetNumMaterials(); ++MaterialIndex)
			{
				MannequinBody->SetMaterial(MaterialIndex, Silhouette.Object);
			}
		}
		if (IdleAnimation)
		{
			MannequinBody->PlayAnimation(IdleAnimation, true);
		}
		BodyPlaceholder->SetVisibility(false);
		FacingPlaceholder->SetVisibility(false);
	}
}

void ARewindCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const bool bMoving = GetVelocity().SizeSquared2D() > FMath::Square(5.f);
	if (MannequinBody && bMoving != bWasMoving)
	{
		if (UAnimSequence* Next = bMoving ? WalkAnimation.Get() : IdleAnimation.Get())
		{
			MannequinBody->PlayAnimation(Next, true);
		}
		bWasMoving = bMoving;
	}

	// The player volume may be narrower than the region's collision geometry,
	// and where it is, that is authored rather than accidental. Clamping the
	// body is a write to PlayerBody, which `world-state-model.md` discards at
	// every loop start, so it is not world state.
	if (ARewindCameraRegion* Region =
			ARewindCameraRegion::FindContaining(GetWorld(), GetActorLocation()))
	{
		// Recorded here as well as in GetScreenAxes, so the fallback frame is
		// current even when the player is standing still when the volume runs
		// out.
		LastKnownRegion = Region;

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

bool ARewindCharacter::GetScreenAxes(FVector& OutRight, FVector& OutDepth)
{
	// Input is expressed against the frame the player is looking at, not
	// against world axes. Otherwise "right" stops meaning right on screen the
	// moment a region is angled differently, and an authored camera would make
	// the game harder to control rather than easier to read.
	if (ARewindCameraRegion* Region =
			ARewindCameraRegion::FindContaining(GetWorld(), GetActorLocation()))
	{
		if (bOutsideEveryRegion)
		{
			bOutsideEveryRegion = false;
			RewindLog::Event(this, FString::Printf(
				TEXT("Movement: region containment regained (%s)"),
				*Region->GetRegionName().ToString()));
		}
		LastKnownRegion = Region;
		OutRight = Region->GetScreenRight();
		OutDepth = Region->GetScreenDepth();
		return true;
	}

	// Outside every region, control is not dropped. `camera-and-movement.md`
	// says a gap in the region set is an authoring defect, and ARewindCameraRig
	// already holds the last good frame rather than dropping the player into an
	// undefined camera. Movement did not do the same, so the player kept the
	// picture and lost the controller: the frame still moved, `AddMovementInput`
	// was never called, and the three interactions at the edges of 4C read as
	// the cause because they are where the volume runs out.
	//
	// Hold the last region's axes for the same reason the camera holds its
	// frame, and log the transition so the gap is visible instead of silent.
	if (const ARewindCameraRegion* Last = LastKnownRegion.Get())
	{
		if (!bOutsideEveryRegion)
		{
			bOutsideEveryRegion = true;
			RewindLog::Event(this, FString::Printf(
				TEXT("Movement: outside every region at %s, holding %s axes"),
				*GetActorLocation().ToCompactString(),
				*Last->GetRegionName().ToString()));
		}
		OutRight = Last->GetScreenRight();
		OutDepth = Last->GetScreenDepth();
		return true;
	}

	// No region has ever contained the player, so there is no authored frame to
	// hold. Take the axes from whatever is actually being viewed, so input
	// still matches the screen rather than the world grid.
	// Not named Controller: APawn already has a member by that name.
	const UWorld* World = GetWorld();
	const APlayerController* ViewingController =
		World ? World->GetFirstPlayerController() : nullptr;
	const AActor* ViewTarget = ViewingController ? ViewingController->GetViewTarget() : nullptr;
	const FRotator ViewRotation = ViewTarget ? ViewTarget->GetActorRotation() : FRotator::ZeroRotator;

	FVector Right = FRotationMatrix(ViewRotation).GetUnitAxis(EAxis::Y);
	FVector Depth = FRotationMatrix(ViewRotation).GetUnitAxis(EAxis::X);
	Right.Z = 0.0;
	Depth.Z = 0.0;
	OutRight = Right.GetSafeNormal();
	OutDepth = Depth.GetSafeNormal();
	return !OutRight.IsNearlyZero() && !OutDepth.IsNearlyZero();
}

void ARewindCharacter::MoveForward(float Value)
{
	if (!Controller)
	{
		return;
	}
	if (FMath::IsNearlyZero(Value))
	{
		LastForwardValue = 0.f;
		LatchedForwardDirection = FVector::ZeroVector;
		return;
	}
	FVector Right, Depth;
	if (GetScreenAxes(Right, Depth))
	{
		if (FMath::IsNearlyZero(LastForwardValue)
			|| FMath::Sign(Value) != FMath::Sign(LastForwardValue))
		{
			LatchedForwardDirection = Depth;
		}
		// Forward moves into the frame, which is the depth the player volume
		// allows. Preserve the press-time direction through a camera handoff;
		// the next press samples the new region's screen axes as usual.
		AddMovementInput(LatchedForwardDirection, Value);
		LastForwardValue = Value;
	}
}

void ARewindCharacter::MoveRight(float Value)
{
	if (!Controller)
	{
		return;
	}
	if (FMath::IsNearlyZero(Value))
	{
		LastRightValue = 0.f;
		LatchedRightDirection = FVector::ZeroVector;
		return;
	}
	FVector Right, Depth;
	if (GetScreenAxes(Right, Depth))
	{
		if (FMath::IsNearlyZero(LastRightValue)
			|| FMath::Sign(Value) != FMath::Sign(LastRightValue))
		{
			LatchedRightDirection = Right;
		}
		AddMovementInput(LatchedRightDirection, Value);
		LastRightValue = Value;
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
