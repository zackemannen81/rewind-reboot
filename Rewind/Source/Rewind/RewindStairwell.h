#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RewindLoopParticipant.h"
#include "RewindStairwell.generated.h"

/** Three physical stair flights from 4C to the entrance floor. */
UCLASS()
class REWIND_API ARewindStairwell : public AActor, public IRewindLoopParticipant
{
	GENERATED_BODY()

public:
	ARewindStairwell();

	virtual void Tick(float DeltaSeconds) override;
	virtual void RestoreFromBaseline() override;
	virtual void ApplyAnchorOverride(FName AnchorId) override;

	static constexpr int32 GetFlightCount();
	static constexpr double GetAuthoredRunLengthCm();
	static constexpr double GetMinimumTraversalSeconds();

private:
	void AddBox(FName Name, FVector RelativeLocation, FVector SizeCm, bool bVisible = true);
	void AddFlight(int32 FlightIndex, FVector Start, float DirectionX, float CentreY);
	bool IsPawnNear(const APawn* Pawn, const FVector& RelativePoint) const;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY()
	TObjectPtr<UStaticMesh> CubeMesh;

	int32 NextCheckpoint = 0;
	double StartedAt = 0.0;
};

constexpr int32 ARewindStairwell::GetFlightCount()
{
	return 3;
}

constexpr double ARewindStairwell::GetAuthoredRunLengthCm()
{
	return 3.0 * 24.0 * 140.0;
}

constexpr double ARewindStairwell::GetMinimumTraversalSeconds()
{
	return GetAuthoredRunLengthCm() / 200.0;
}
