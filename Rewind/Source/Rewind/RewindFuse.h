#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RewindInteractable.h"
#include "RewindLoopParticipant.h"
#include "RewindFuse.generated.h"

UENUM(meta = (ScriptName = "RewindFuseSocketKind"))
enum class ERewindFuseSocket : uint8
{
	Building,
	Courtyard
};

/**
 * The one fuse, per `docs/design/chapter-1-authored.md`.
 *
 * It is LoopWorld, and being held is simply one of the positions it can be in,
 * alongside its rest position and either socket. `world-state-model.md` says
 * that explicitly: there is no inventory bucket and none is needed, because
 * LoopWorld is discarded at loop start and the fuse returns home for free.
 *
 * It is the resource ADR-0008 requires an Anchor to release. It fits the
 * building socket or the courtyard socket, never both, which is what makes it
 * a choice rather than a key.
 */
UCLASS()
class REWIND_API ARewindFuse : public AActor, public IRewindInteractable, public IRewindLoopParticipant
{
	GENERATED_BODY()

public:
	ARewindFuse();

	virtual void Tick(float DeltaSeconds) override;

	/** Picks the fuse up when it is at rest. Sockets handle seating and removal. */
	virtual bool TryInteract(APawn* InstigatorPawn) override;

	virtual void RestoreFromBaseline() override;
	virtual void ApplyAnchorOverride(FName AnchorId) override;

	static ARewindFuse* Find(const UWorld* World);

	bool IsCarried() const { return State == EState::Carried; }
	bool IsSeatedIn(ERewindFuseSocket Which) const { return State == EState::Seated && Seated == Which; }
	bool IsSeated() const { return State == EState::Seated; }

	/** Called by a socket. Returns false when the fuse is not in hand. */
	bool SeatInto(ERewindFuseSocket Which);

	/** Developer verification: pick up if needed and seat. Not a player action. */
	bool SeatForVerification(ERewindFuseSocket Which);

	/** Called by a socket. Returns false when the fuse is not seated there. */
	bool TakeFrom(ERewindFuseSocket Which);

private:
	enum class EState : uint8
	{
		AtRest,
		Carried,
		Seated
	};

	void SetCarryCollision(bool bCarried);
	static const TCHAR* SocketName(ERewindFuseSocket Which);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	/** Where the fuse sits at Baseline. Captured once, restored every loop. */
	FVector HomeLocation = FVector::ZeroVector;
	bool bHomeCaptured = false;

	EState State = EState::AtRest;
	ERewindFuseSocket Seated = ERewindFuseSocket::Building;
};
