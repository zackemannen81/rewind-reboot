#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RewindFuse.h"
#include "RewindInteractable.h"
#include "RewindFuseSocket.generated.h"

/**
 * One of the two places the fuse fits, per `chapter-1-authored.md`.
 *
 * The socket holds no state of its own. The fuse knows where it is, and the
 * socket asks. That keeps the "never both" rule true by construction rather
 * than by two objects agreeing with each other.
 */
UCLASS()
class REWIND_API ARewindFuseSocket : public AActor, public IRewindInteractable
{
	GENERATED_BODY()

public:
	ARewindFuseSocket();

	virtual void Tick(float DeltaSeconds) override;

	/** Seats the carried fuse, or takes back the one seated here. */
	virtual bool TryInteract(APawn* InstigatorPawn) override;

	void Configure(ERewindFuseSocket InWhich) { Which = InWhich; }
	ERewindFuseSocket GetSocket() const { return Which; }

	/** True while this socket holds the fuse. */
	bool IsOccupied() const;

	/** The socket of this kind, or null. */
	static ARewindFuseSocket* Find(const UWorld* World, ERewindFuseSocket Which);

private:
	UPROPERTY(EditAnywhere, Category = "Socket")
	ERewindFuseSocket Which = ERewindFuseSocket::Building;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> SeatedFuse;
};
