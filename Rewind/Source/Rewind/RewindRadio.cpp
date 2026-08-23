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
	/** `chapter-1-authored.md`: a 45 second sequence repeating every 60. */
	constexpr double RadioCycle = 60.0;
	constexpr double RadioSequence = 45.0;

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
	FragmentsReported = 0;

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
	// Four fragments across the sequence, so a 45 second wait reads as
	// something arriving rather than as nothing happening.
	static const TCHAR* Fragments[] = {
		TEXT("...blue shift..."),
		TEXT("...seven..."),
		TEXT("...three..."),
		TEXT("...one... two..."),
	};
	constexpr int32 FragmentCount = UE_ARRAY_COUNT(Fragments);

	const int32 Due = FMath::Clamp(
		static_cast<int32>((Phase / RadioSequence) * FragmentCount), 0, FragmentCount);

	while (FragmentsReported < Due)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Cyan, Fragments[FragmentsReported]);
		}
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

	if (IsBeingHeard())
	{
		if (ListeningSince < 0.0)
		{
			ListeningSince = Elapsed;
		}
		if (bInSequence)
		{
			ReportFragment(Phase);
		}
	}
	else
	{
		ListeningSince = -1.0;
		FragmentsReported = 0;
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
