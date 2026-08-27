#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RewindAuthoredCourtyard.generated.h"

class ARewindCameraRegion;
class ARewindCausalCheckpoint;
class ARewindCourtyardGate;
class ARewindFuseSocket;
class ARewindGenerator;

/**
 * Authored courtyard for `Rewind.SkipProofLayout` maps.
 *
 * Geometry, camera regions, the ground-floor socket, generator, gate and
 * GroundFuseGate are placed against `GroundHall_Region`'s positive-Y face so
 * the volumes abut. The procedural proof map does not spawn this actor and
 * keeps its own checkpoint at X = 4110.
 */
UCLASS()
class REWIND_API ARewindAuthoredCourtyard : public AActor
{
	GENERATED_BODY()

public:
	ARewindAuthoredCourtyard();

	void EnsureContents();

	ARewindCameraRegion* GetApproachRegion() const { return ApproachRegion; }
	ARewindCameraRegion* GetCourtyardRegion() const { return CourtyardRegion; }
	ARewindCausalCheckpoint* GetCheckpoint() const { return Checkpoint; }
	ARewindFuseSocket* GetGroundSocket() const { return GroundSocket; }
	ARewindGenerator* GetGenerator() const { return Generator; }
	ARewindCourtyardGate* GetGate() const { return Gate; }

	FVector GetThresholdWorldLocation() const;
	static ARewindCameraRegion* FindGroundHall(const UWorld* World);

private:
	enum class ESurface : uint8
	{
		UpperWall,
		LowerWall,
		Floor,
		Metal,
		Door
	};

	void AddBox(FName Name, FVector RelativeLocation, FVector SizeCm, ESurface Surface, bool bVisible = true, bool bBlock = true);
	void AddPointLight(FName Name, FVector RelativeLocation, float Intensity, float Attenuation, float Temperature);
	void ApplySurface(class UStaticMeshComponent* Mesh, ESurface Surface) const;
	void EnsureRegions(ARewindCameraRegion* GroundHall);
	void EnsureGameplay();
	template <typename T>
	T* FindOrSpawn(TObjectPtr<T>& Cache, const FVector& WorldLocation);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY()
	TObjectPtr<UStaticMesh> CubeMesh;

	UPROPERTY()
	TObjectPtr<class UMaterialInterface> UpperWallMaterial;

	UPROPERTY()
	TObjectPtr<class UMaterialInterface> LowerWallMaterial;

	UPROPERTY()
	TObjectPtr<class UMaterialInterface> FloorMaterial;

	UPROPERTY()
	TObjectPtr<class UMaterialInterface> MetalMaterial;

	UPROPERTY()
	TObjectPtr<class UMaterialInterface> DoorMaterial;

	UPROPERTY()
	TObjectPtr<ARewindCameraRegion> ApproachRegion;

	UPROPERTY()
	TObjectPtr<ARewindCameraRegion> CourtyardRegion;

	UPROPERTY()
	TObjectPtr<ARewindCausalCheckpoint> Checkpoint;

	UPROPERTY()
	TObjectPtr<ARewindFuseSocket> GroundSocket;

	UPROPERTY()
	TObjectPtr<ARewindGenerator> Generator;

	UPROPERTY()
	TObjectPtr<ARewindCourtyardGate> Gate;
};
