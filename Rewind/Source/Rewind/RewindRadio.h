#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RewindInteractable.h"
#include "RewindLoopParticipant.h"
#include "RewindRadio.generated.h"

/**
 * The radio, per `docs/design/chapter-1-authored.md`.
 *
 * It broadcasts on the loop clock and never reads loop count, which ADR-0002
 * forbids. The code sequence takes 20 seconds and repeats every 50, so the
 * fact is obtained by being present for a whole sequence rather than by
 * pressing a key. That cost is the loop's first lesson: information is
 * expensive once and free afterwards.
 *
 * Interacting selects a channel. Exactly one carries the sequence and the
 * others carry static. Which channel that is, is never stored: the player
 * remembers it, exactly as they remember the patrol's timing.
 */
UCLASS()
class REWIND_API ARewindRadio : public AActor, public IRewindInteractable, public IRewindLoopParticipant
{
	GENERATED_BODY()

public:
	ARewindRadio();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	/** Selects the next channel. Tuning is one interaction per press. */
	virtual bool TryInteract(APawn* InstigatorPawn) override;

	virtual void RestoreFromBaseline() override;
	virtual void ApplyAnchorOverride(FName AnchorId) override;

	/** Pure selection used by the schedule and its automation coverage. */
	static int32 GetDigitSoundIndexForPhase(double Phase);

	/** Runtime observability for the radio's LoopWorld audio selection. */
	int32 GetChannel() const { return Channel; }
	int32 GetCurrentBedSoundIndex() const;
	int32 GetAppliedBedSoundIndex() const;

private:
	/** Whether a pawn is close enough to hear, and on the right channel. */
	bool IsBeingHeard() const;

	/** Fragments spoken as the sequence advances, so the wait is legible. */
	void ReportFragment(double Phase);

	/** Keeps the spatial radio bed in lockstep with the current channel. */
	void UpdateBedAudio();

	/** Interaction sounds are intentionally dry, unlike the radio broadcast. */
	void PlayInteractionSound(class USoundBase* Sound) const;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<class UAudioComponent> RadioBed;

	UPROPERTY()
	TObjectPtr<class USoundAttenuation> DigitAttenuation;

	UPROPERTY()
	TObjectPtr<class USoundBase> DigitSounds[4];

	UPROPERTY()
	TObjectPtr<class USoundBase> StationLoop;

	UPROPERTY()
	TObjectPtr<class USoundBase> StaticLoop;

	UPROPERTY()
	TObjectPtr<class USoundBase> ClickOnSound;

	UPROPERTY()
	TObjectPtr<class USoundBase> ClickOffSound;

	UPROPERTY()
	TObjectPtr<class USoundBase> TuneSound;

	/**
	 * Selected channel. Channel zero is off; one through four are the authored
	 * receiver positions. LoopWorld: it returns to the authored default at loop start.
	 */
	int32 Channel = 1;

	/** Elapsed loop time when unbroken listening began, or -1 when not listening. */
	double ListeningSince = -1.0;

	/** Whether the previous tick was inside the sequence, for edge detection. */
	bool bWasInSequence = false;

	/** Highest fragment index already reported this sequence. */
	int32 FragmentsReported = 0;
};
