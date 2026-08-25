#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "RewindWorldStateSubsystem.generated.h"

UCLASS()
class REWIND_API URewindWorldStateSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	void EnsureAuthoredSpace();
	void ApplyLoopStart();
	void PlacePlayerBody();
	FTransform GetLoopStartPose() const;

	/**
	 * Developer verification poses. `threshold` stands just before
	 * GroundFuseGate; `fuse` stands at the 4C fuse; `courtyard_socket` stands
	 * at the ground-floor socket. Not a player action.
	 */
	bool PlacePlayerForVerification(FName Pose);

private:
	void EnsureCausalCheckpoints();
	void EnsureAuthoredCourtyard();
	void EnsureLoopBreakSignature();
};
