#include "RewindRadio.h"

#include "RewindLog.h"
#include "RewindIds.h"
#include "RewindLoopSubsystem.h"
#include "RewindSessionSubsystem.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"

namespace
{
	/**
	 * `chapter-1-authored.md`: a 20 second sequence repeating every 50, with the
	 * four digits of 7312 spoken at phases 4, 9, 14 and 19. The cycle is 50 so
	 * that it does not lock in step with the patrol's 40 or the turnstile's 30.
	 */
	constexpr double RadioCycle = 50.0;
	constexpr double RadioSequence = 20.0;
	constexpr double DigitPhases[] = { 4.0, 9.0, 14.0, 19.0 };
	const TCHAR* DigitWords[] = { TEXT("seven"), TEXT("three"), TEXT("one"), TEXT("two") };
	constexpr int32 DigitCount = UE_ARRAY_COUNT(DigitPhases);

	/** Not a rule. The document says there are channels and one carries the code. */
	constexpr int32 RadioChannelCount = 4;
	constexpr int32 RadioCodeChannel = 3;

	/** How close the player must stand to hear it. */
	constexpr double RadioRange = 320.0;

	/** One tick's grace, so the grant does not turn on floating point. */
	constexpr double HeardTolerance = 0.25;
}

ARewindRadio::ARewindRadio()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Mesh->SetCollisionResponseToAllChannels(ECR_Overlap);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (Cube.Succeeded())
	{
		Mesh->SetStaticMesh(Cube.Object);
	}
	Mesh->SetRelativeScale3D(FVector(0.4f, 0.6f, 0.3f));
}

void ARewindRadio::RestoreFromBaseline()
{
	Channel = 1;
	ListeningSince = -1.0;
	bWasInSequence = false;
	FragmentsReported = 0;
	RewindLog::Baseline(TEXT("Radio: channel 1, static"));
}

void ARewindRadio::ApplyAnchorOverride(FName AnchorId)
{
	(void)AnchorId;
}

bool ARewindRadio::TryInteract(APawn* InstigatorPawn)
{
	(void)InstigatorPawn;

	Channel = Channel % RadioChannelCount + 1;

	// Changing channel breaks the listen. A sequence heard in two halves is
	// not a sequence heard.
	ListeningSince = -1.0;

	// FragmentsReported is deliberately not reset here. It counts what the
	// broadcast has already said, which is a property of the sequence and not
	// of whoever is listening. Resetting it let a late arrival replay digits
	// spoken before they tuned in: the first played test showed digit 1, at
	// phase 4.0, being announced at t=5.95 on the tick the channel changed.

	const bool bOnCode = Channel == RadioCodeChannel;
	RewindLog::Event(this, FString::Printf(TEXT("Radio: channel %d (%s)"),
		Channel, bOnCode ? TEXT("voice") : TEXT("static")));
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, bOnCode ? FColor::Cyan : FColor::Silver,
			FString::Printf(TEXT("Radio: channel %d  %s"),
				Channel, bOnCode ? TEXT("...a voice, under the static") : TEXT("static")));
	}
	return true;
}

bool ARewindRadio::IsBeingHeard() const
{
	if (Channel != RadioCodeChannel)
	{
		return false;
	}
	const UWorld* World = GetWorld();
	const APlayerController* Controller = World ? World->GetFirstPlayerController() : nullptr;
	const APawn* Pawn = Controller ? Controller->GetPawn() : nullptr;
	if (!Pawn)
	{
		return false;
	}
	return FVector::Dist(Pawn->GetActorLocation(), GetActorLocation()) <= RadioRange;
}

void ARewindRadio::ReportFragment(double Phase)
{
	// A digit heard is a digit kept. Each one is spoken at its authored phase,
	// and a player standing here for only part of a sequence still leaves with
	// what was said while they stood. Nothing about this is stored: the digits
	// live in the player's memory, like the patrol's timing.
	while (FragmentsReported < DigitCount && Phase >= DigitPhases[FragmentsReported])
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
				FString::Printf(TEXT("Radio:  ...%s..."), DigitWords[FragmentsReported]));
		}
		RewindLog::Event(this, FString::Printf(TEXT("Radio: digit %d spoken (%s) at phase %.1f"),
			FragmentsReported + 1, DigitWords[FragmentsReported], DigitPhases[FragmentsReported]));
		++FragmentsReported;
	}
}

void ARewindRadio::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	(void)DeltaSeconds;

	const UWorld* World = GetWorld();
	const URewindLoopSubsystem* Loop = World ? World->GetSubsystem<URewindLoopSubsystem>() : nullptr;
	if (!Loop)
	{
		return;
	}

	const double Elapsed = Loop->GetElapsedLoopTime();
	const double Phase = FMath::Fmod(Elapsed, RadioCycle);
	const bool bInSequence = Phase < RadioSequence;

	// The count of digits already spoken this sequence is carried whether or
	// not anyone is listening, so that arriving late reports only what is still
	// to come rather than replaying what was missed.
	if (bInSequence)
	{
		if (IsBeingHeard())
		{
			ReportFragment(Phase);
		}
		else
		{
			while (FragmentsReported < DigitCount && Phase >= DigitPhases[FragmentsReported])
			{
				++FragmentsReported;
			}
		}
	}

	if (IsBeingHeard())
	{
		if (ListeningSince < 0.0)
		{
			ListeningSince = Elapsed;
		}
	}
	else
	{
		ListeningSince = -1.0;
	}

	// The broadcast is a world clock like the patrol and the turnstile, so its
	// transitions are logged whether or not anyone is listening. Without this
	// the sequence cannot be checked at a stated `t`, and FL-04's evidence
	// would rest on the player's report.
	if (bInSequence != bWasInSequence)
	{
		RewindLog::Event(this, FString::Printf(
			TEXT("Radio: broadcast %s  (phase=%.2f of %.0fs)"),
			bInSequence ? TEXT("SEQUENCE") : TEXT("STATIC"), Phase, RadioCycle));
	}

	// The sequence just ended. Listening for at least its full length, ending
	// here, is the same thing as having been present from its start, and it
	// does not depend on catching the exact frame the sequence began.
	if (bWasInSequence && !bInSequence)
	{
		if (ListeningSince >= 0.0 && (Elapsed - ListeningSince) >= RadioSequence - HeardTolerance)
		{
			bool bAlreadyKnown = false;
			if (const APlayerController* Controller = World->GetFirstPlayerController())
			{
				if (const UGameInstance* GI = Controller->GetGameInstance())
				{
					if (URewindSessionSubsystem* Session = GI->GetSubsystem<URewindSessionSubsystem>())
					{
						bAlreadyKnown = Session->HasKnowledge(RewindIds::KnowledgeRadioCode7312);
						Session->ObtainKnowledge(RewindIds::KnowledgeRadioCode7312);
					}
				}
			}
			if (!bAlreadyKnown)
			{
				RewindLog::Event(this, FString::Printf(
					TEXT("Radio: 7312 obtained after a full %.0fs sequence"), RadioSequence));
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 6.f, FColor::Green, TEXT("Radio: 7312"));
				}
			}
		}
		else if (ListeningSince >= 0.0)
		{
			RewindLog::Event(this, FString::Printf(
				TEXT("Radio: sequence missed, heard only %.1fs of %.0f"),
				Elapsed - ListeningSince, RadioSequence));
		}
		FragmentsReported = 0;
	}

	bWasInSequence = bInSequence;
}
