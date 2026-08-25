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

ARewindCameraRegion* ARewindCameraRegion::FindByName(const UWorld* World, FName Name)
{
	if (!World || Name.IsNone())
	{
		return nullptr;
	}

	for (TActorIterator<ARewindCameraRegion> It(const_cast<UWorld*>(World)); It; ++It)
	{
		if (It->GetRegionName() == Name)
		{
			return *It;
		}
		if (It->GetActorNameOrLabel() == Name.ToString())
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
	bool bInCutOnEntry,
	float InFieldOfView)
{
	RegionName = InName;
	PlayerVolumeExtent = InPlayerVolumeExtent;
	TravelAxis = InTravelAxis;
	CameraOffset = InCameraOffset;
	CameraRotation = InCameraRotation;
	FieldOfView = InFieldOfView;
	TravelPadding = InTravelPadding;
	DeadZone = InDeadZone;
	bCutOnEntry = bInCutOnEntry;
}

bool ARewindCameraRegion::Contains(const FVector& Location) const
{
	const FVector Min = GetActorLocation() - PlayerVolumeExtent;
	const FVector Max = GetActorLocation() + PlayerVolumeExtent;

	// Half-open volumes let adjacent regions share an authored threshold
	// without making that exact coordinate belong to both of them.
	return Location.X >= Min.X && Location.X < Max.X
		&& Location.Y >= Min.Y && Location.Y < Max.Y
		&& Location.Z >= Min.Z && Location.Z < Max.Z;
}

FVector ARewindCameraRegion::ClampToPlayerVolume(const FVector& Location) const
{
	const FVector Centre = GetActorLocation();

	// The positive faces are half-open, because `Contains` gives a shared
	// threshold exactly one owner. The clamp must therefore stop just inside
	// them: clamping to the face itself put the player on a coordinate this
	// region no longer contains, and if no adjacent region overlapped it, the
	// player belonged to no region at all. That state used to cost the player
	// their controller. It is now survivable, and it is still wrong to create.
	//
	// One millimetre, which is deliberately larger than the 0.01 tolerance
	// `ARewindCharacter::Tick` compares against. At 0.01 the correction would
	// be inside its own tolerance, so the player would be left standing on the
	// face this inset exists to keep them off.
	const double Inset = 0.1;
	return FVector(
		FMath::Clamp(Location.X,
			Centre.X - PlayerVolumeExtent.X, Centre.X + PlayerVolumeExtent.X - Inset),
		FMath::Clamp(Location.Y,
			Centre.Y - PlayerVolumeExtent.Y, Centre.Y + PlayerVolumeExtent.Y - Inset),
		Location.Z);
}

double ARewindCameraRegion::GetTravelCoord(const FVector& Location) const
{
	switch (TravelAxis)
	{
	case ERewindTravelAxis::X:
		return Location.X;
	case ERewindTravelAxis::Y:
		return Location.Y;
	case ERewindTravelAxis::Z:
		return Location.Z;
	default:
		return Location.X;
	}
}

double ARewindCameraRegion::ClampTravel(double Coord) const
{
	const FVector Centre = GetActorLocation();
	double Half = PlayerVolumeExtent.X;
	if (TravelAxis == ERewindTravelAxis::Y)
	{
		Half = PlayerVolumeExtent.Y;
	}
	else if (TravelAxis == ERewindTravelAxis::Z)
	{
		Half = PlayerVolumeExtent.Z;
	}
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
		Location.X = TravelCoord + CameraOffset.X;
	}
	else if (TravelAxis == ERewindTravelAxis::Y)
	{
		Location.Y = TravelCoord + CameraOffset.Y;
	}
	else
	{
		Location.Z = TravelCoord + CameraOffset.Z;
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
