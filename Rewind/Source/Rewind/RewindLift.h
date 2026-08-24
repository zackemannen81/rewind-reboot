#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RewindInteractable.h"
#include "RewindLoopParticipant.h"
#include "RewindLift.generated.h"

/** A powered cage lift between 4C and the entrance floor. */
UCLASS()
class REWIND_API ARewindLift : public AActor, public IRewindInteractable, public IRewindLoopParticipant
{
	GENERATED_BODY()

public:
	ARewindLift();

	virtual void Tick(float DeltaSeconds) override;
	virtual bool TryInteract(APawn* InstigatorPawn) override;
	virtual void RestoreFromBaseline() override;
	virtual void ApplyAnchorOverride(FName AnchorId) override;

	void Configure(double InTopZ, double InBottomZ);
	static constexpr double GetAuthoredTravelSeconds();

private:
	void SetCabinZ(double Z);
	void ReleasePassenger();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Cabin;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Platform;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> CabinFloorCollision;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> CabinBack;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> CabinRoof;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> ShaftLeft;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> ShaftRight;

	UPROPERTY()
	TObjectPtr<APawn> Passenger;

	double TopZ = 1200.0;
	double BottomZ = 0.0;
	double TravelStartedAt = 0.0;
	double TravelStartZ = 1200.0;
	double TravelTargetZ = 0.0;
	FVector PassengerOffset = FVector::ZeroVector;
	bool bMoving = false;
	bool bAtEntranceFloor = false;
	bool bPowerLogged = false;
};

constexpr double ARewindLift::GetAuthoredTravelSeconds()
{
	return 6.0;
}
