#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RewindCameraRegion.generated.h"

UENUM()
enum class ERewindTravelAxis : uint8
{
	X,
	Y
};

/**
 * One camera region, as `docs/design/camera-and-movement.md` defines it.
 *
 * A region declares its camera's rotation, a single travel axis, bounds on that
 * axis, a dead zone, and the volume the player may occupy. A region that
 * declares fewer of those is unfinished; there is no default to fall back to,
 * which is why every field here has a value and none is optional.
 *
 * This actor holds data and answers questions. It never moves anything.
 */
UCLASS()
class REWIND_API ARewindCameraRegion : public AActor
{
	GENERATED_BODY()

public:
	ARewindCameraRegion();

	/** The region containing this location, or null when the location is outside every region. */
	static ARewindCameraRegion* FindContaining(const UWorld* World, const FVector& Location);

	/** Set the fields a spawned region needs. Placed regions author them instead. */
	void Configure(FName InName, const FVector& InPlayerVolumeExtent);

	bool Contains(const FVector& Location) const;

	/** The player volume, which may be narrower than the region's collision geometry. */
	FVector ClampToPlayerVolume(const FVector& Location) const;

	/** This location's coordinate along the travel axis. */
	double GetTravelCoord(const FVector& Location) const;

	/** Travel coordinate clamped to the region's authored camera bounds. */
	double ClampTravel(double Coord) const;

	/** Camera position for a given travel coordinate. Off-axis components are fixed. */
	FVector GetCameraLocation(double TravelCoord) const;

	FRotator GetCameraRotation() const { return CameraRotation; }
	double GetDeadZone() const { return DeadZone; }
	bool CutsOnEntry() const { return bCutOnEntry; }
	FName GetRegionName() const { return RegionName; }

	/** Screen-right in world space, derived from the camera rotation so that input matches the frame. */
	FVector GetScreenRight() const;

	/** Screen-into-the-frame in world space, the depth direction the player moves along. */
	FVector GetScreenDepth() const;

private:
	/** Named so the log and the region enumeration are readable. */
	UPROPERTY(EditAnywhere, Category = "Region")
	FName RegionName = NAME_None;

	/** Half-size of the player volume, centred on this actor. */
	UPROPERTY(EditAnywhere, Category = "Region")
	FVector PlayerVolumeExtent = FVector(500.f, 380.f, 300.f);

	UPROPERTY(EditAnywhere, Category = "Camera")
	ERewindTravelAxis TravelAxis = ERewindTravelAxis::X;

	/** Camera position relative to the volume centre. The travel component is ignored. */
	UPROPERTY(EditAnywhere, Category = "Camera")
	FVector CameraOffset = FVector(0.f, -1600.f, 260.f);

	UPROPERTY(EditAnywhere, Category = "Camera")
	FRotator CameraRotation = FRotator(-6.f, 90.f, 0.f);

	/** How far inside the volume edge the camera stops, so a region cannot show what was not built. */
	UPROPERTY(EditAnywhere, Category = "Camera")
	double TravelPadding = 260.0;

	/** The player may move this far from the camera's current centre before it follows. */
	UPROPERTY(EditAnywhere, Category = "Camera")
	double DeadZone = 220.0;

	/** True only where the threshold is a real visual break, such as a doorway. */
	UPROPERTY(EditAnywhere, Category = "Camera")
	bool bCutOnEntry = false;
};
