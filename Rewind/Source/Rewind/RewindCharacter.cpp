#include "RewindCharacter.h"

#include "RewindInteractable.h"
#include "RewindCameraRegion.h"
#include "RewindFirstRun.h"
#include "RewindLog.h"
#include "RewindMessageIds.h"
#include "Animation/AnimSequence.h"
#include "GameFramework/PlayerController.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInterface.h"
#include "Sound/SoundBase.h"
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

	// REW-0029 Returner import. Its own in-place clips keep movement owned by
	// CharacterMovement rather than animation root travel.
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Returner(
		TEXT("/Game/Characters/Returner/Returner.Returner"));
	static ConstructorHelpers::FObjectFinder<UAnimSequence> Idle(
		TEXT("/Game/Characters/Returner/Animations/A_Returner_Alert.A_Returner_Alert"));
	static ConstructorHelpers::FObjectFinder<UAnimSequence> Walk(
		TEXT("/Game/Characters/Returner/Animations/A_Returner_Walk.A_Returner_Walk"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Silhouette(
		TEXT("/Game/Art/Materials/Stairwell/MI_CharacterSilhouette.MI_CharacterSilhouette"));
	static ConstructorHelpers::FObjectFinder<USoundBase> Step01(
		TEXT("/Game/Audio/Chapter1/A_REW_Step_Interior_01.A_REW_Step_Interior_01"));
	static ConstructorHelpers::FObjectFinder<USoundBase> Step02(
		TEXT("/Game/Audio/Chapter1/A_REW_Step_Interior_02.A_REW_Step_Interior_02"));
	static ConstructorHelpers::FObjectFinder<USoundBase> Step03(
		TEXT("/Game/Audio/Chapter1/A_REW_Step_Interior_03.A_REW_Step_Interior_03"));
	static ConstructorHelpers::FObjectFinder<USoundBase> Step04(
		TEXT("/Game/Audio/Chapter1/A_REW_Step_Interior_04.A_REW_Step_Interior_04"));
	static ConstructorHelpers::FObjectFinder<USoundBase> Step05(
		TEXT("/Game/Audio/Chapter1/A_REW_Step_Interior_05.A_REW_Step_Interior_05"));
	static ConstructorHelpers::FObjectFinder<USoundBase> Step06(
		TEXT("/Game/Audio/Chapter1/A_REW_Step_Interior_06.A_REW_Step_Interior_06"));
	FootstepSounds[0] = Step01.Succeeded() ? Step01.Object : nullptr;
	FootstepSounds[1] = Step02.Succeeded() ? Step02.Object : nullptr;
	FootstepSounds[2] = Step03.Succeeded() ? Step03.Object : nullptr;
	FootstepSounds[3] = Step04.Succeeded() ? Step04.Object : nullptr;
	FootstepSounds[4] = Step05.Succeeded() ? Step05.Object : nullptr;
	FootstepSounds[5] = Step06.Succeeded() ? Step06.Object : nullptr;

	ReturnerBody = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ReturnerBody"));
	ReturnerBody->SetupAttachment(RootComponent);
	ReturnerBody->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// REW-0029 editor evidence: bounds are centred at Z=85 with Z extent=85,
	// so the imported origin is at the feet. The reference pose's `headfront`
	// point is +Y from the head; yaw -90 maps that facing vector to the
	// character root's +X movement-forward direction. ACharacter's root is at
	// capsule centre, so the 96 cm half-height places the feet at its bottom.
	ReturnerBody->SetRelativeLocation(FVector(
		0.f, 0.f, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
	ReturnerBody->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	if (Returner.Succeeded())
	{
		ReturnerBody->SetSkeletalMeshAsset(Returner.Object);
		IdleAnimation = Idle.Succeeded() ? Idle.Object : nullptr;
		WalkAnimation = Walk.Succeeded() ? Walk.Object : nullptr;
		ReturnerBody->SetAnimationMode(EAnimationMode::AnimationSingleNode);
		if (Silhouette.Succeeded())
		{
			for (int32 MaterialIndex = 0; MaterialIndex < ReturnerBody->GetNumMaterials(); ++MaterialIndex)
			{
				ReturnerBody->SetMaterial(MaterialIndex, Silhouette.Object);
			}
		}
		if (IdleAnimation)
		{
			ReturnerBody->PlayAnimation(IdleAnimation, true);
		}
		BodyPlaceholder->SetVisibility(false);
		FacingPlaceholder->SetVisibility(false);
	}
}

void ARewindCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const bool bMoving = GetVelocity().SizeSquared2D() > FMath::Square(5.f);
	if (ReturnerBody && bMoving != bWasMoving)
	{
		if (UAnimSequence* Next = bMoving ? WalkAnimation.Get() : IdleAnimation.Get())
		{
			ReturnerBody->PlayAnimation(Next, true);
		}
		bWasMoving = bMoving;
	}
	UpdateFootsteps();

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
		if (RewindFirstRun::IsStairRegion(Region->GetRegionName()))
		{
			RewindFirstRun::ShowOnce(this, RewindMessageIds::StairsRemain);
		}

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
	LastFootstepLocation = GetActorLocation();
	bHasFootstepLocation = true;
	if (RewindFirstRun::ShowOnce(this, RewindMessageIds::ApartmentWaking))
	{
		RewindFirstRun::ShowOnce(this, RewindMessageIds::CharacterControls);
	}
}

void ARewindCharacter::UpdateFootsteps()
{
	constexpr float StepDistanceCm = 80.0f;
	constexpr float TeleportDistanceCm = 1000.0f;
	const FVector CurrentLocation = GetActorLocation();
	if (!bHasFootstepLocation)
	{
		LastFootstepLocation = CurrentLocation;
		bHasFootstepLocation = true;
		return;
	}

	const float Travelled = FVector::Dist2D(CurrentLocation, LastFootstepLocation);
	LastFootstepLocation = CurrentLocation;
	if (Travelled > TeleportDistanceCm)
	{
		// A rewind or scripted placement is not a footfall.
		FootstepDistance = 0.0f;
		return;
	}

	FootstepDistance += Travelled;
	while (FootstepDistance >= StepDistanceCm)
	{
		FootstepDistance -= StepDistanceCm;
		PlayFootstep();
	}
}

void ARewindCharacter::PlayFootstep()
{
	constexpr int32 FootstepCount = UE_ARRAY_COUNT(FootstepSounds);
	int32 Index = FMath::RandRange(0, FootstepCount - 1);
	if (PreviousFootstepIndex != INDEX_NONE && Index >= PreviousFootstepIndex)
	{
		// Pick uniformly from the other five indices, never the prior sound.
		Index = (Index + 1) % FootstepCount;
	}
	PreviousFootstepIndex = Index;
	if (USoundBase* Sound = FootstepSounds[Index])
	{
		UGameplayStatics::PlaySoundAtLocation(this, Sound, GetActorLocation());
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
