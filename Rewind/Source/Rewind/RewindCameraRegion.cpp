#include "RewindCameraRegion.h"

#include "EngineUtils.h"
#include "Engine/World.h"

ARewindCameraRegion::ARewindCameraRegion()
{
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));
}

ARewindCameraRegion* ARewindCameraRegion::FindContaining(const UWorld* World, const FVector& Location)
{
	if (!World)
	{
		return nullptr;
	}

	for (TActorIterator<ARewindCameraRegion> It(const_cast<UWorld*>(World)); It; ++It)
	{
		if (It->Contains(Location))
		{
			return *It;
		}
	}
	return nullptr;
}

void ARewindCameraRegion::Configure(
	FName InName,
	const FVector& InPlayerVolumeExtent,
	ERewindTravelAxis InTravelAxis,
	const FVector& InCameraOffset,
	const FRotator& InCameraRotation,
	double InTravelPadding,
	double InDeadZone,
	bool bInCutOnEntry)
{
	RegionName = InName;
	PlayerVolumeExtent = InPlayerVolumeExtent;
	TravelAxis = InTravelAxis;
	CameraOffset = InCameraOffset;
	CameraRotation = InCameraRotation;
	TravelPadding = InTravelPadding;
	DeadZone = InDeadZone;
	bCutOnEntry = bInCutOnEntry;
}

bool ARewindCameraRegion::Contains(const FVector& Location) const
{
	const FVector Local = Location - GetActorLocation();
	return FMath::Abs(Local.X) <= PlayerVolumeExtent.X
		&& FMath::Abs(Local.Y) <= PlayerVolumeExtent.Y
		&& FMath::Abs(Local.Z) <= PlayerVolumeExtent.Z;
}

FVector ARewindCameraRegion::ClampToPlayerVolume(const FVector& Location) const
{
	const FVector Centre = GetActorLocation();
	return FVector(
		FMath::Clamp(Location.X, Centre.X - PlayerVolumeExtent.X, Centre.X + PlayerVolumeExtent.X),
		FMath::Clamp(Location.Y, Centre.Y - PlayerVolumeExtent.Y, Centre.Y + PlayerVolumeExtent.Y),
		Location.Z);
}

double ARewindCameraRegion::GetTravelCoord(const FVector& Location) const
{
	return TravelAxis == ERewindTravelAxis::X ? Location.X : Location.Y;
}

double ARewindCameraRegion::ClampTravel(double Coord) const
{
	const FVector Centre = GetActorLocation();
	const double Half = TravelAxis == ERewindTravelAxis::X ? PlayerVolumeExtent.X : PlayerVolumeExtent.Y;
	const double CentreCoord = GetTravelCoord(Centre);

	// Padding pulls the camera's travel inside the volume, so the frame never
	// reaches the edge of what the region was composed for. A region narrower
	// than twice the padding simply does not travel.
	const double Min = CentreCoord - FMath::Max(0.0, Half - TravelPadding);
	const double Max = CentreCoord + FMath::Max(0.0, Half - TravelPadding);
	return FMath::Clamp(Coord, Min, Max);
}

FVector ARewindCameraRegion::GetCameraLocation(double TravelCoord) const
{
	FVector Location = GetActorLocation() + CameraOffset;
	if (TravelAxis == ERewindTravelAxis::X)
	{
		Location.X = TravelCoord;
	}
	else
	{
		Location.Y = TravelCoord;
	}
	return Location;
}

FVector ARewindCameraRegion::GetScreenRight() const
{
	// Input is expressed against the frame rather than against world axes, so
	// that "right" still means right on screen when a region is angled
	// differently. Flattened, because the camera's pitch must not tilt walking.
	FVector Right = FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::Y);
	Right.Z = 0.0;
	return Right.GetSafeNormal();
}

FVector ARewindCameraRegion::GetScreenDepth() const
{
	FVector Depth = FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::X);
	Depth.Z = 0.0;
	return Depth.GetSafeNormal();
}
