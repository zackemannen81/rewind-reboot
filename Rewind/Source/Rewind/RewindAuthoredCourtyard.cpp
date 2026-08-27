#include "RewindAuthoredCourtyard.h"

#include "RewindAuthoredCourtyardMetrics.h"
#include "RewindCameraRegion.h"
#include "RewindCausalCheckpoint.h"
#include "RewindCourtyardGate.h"
#include "RewindFuseSocket.h"
#include "RewindGenerator.h"
#include "RewindIds.h"
#include "RewindLog.h"
#include "Components/PointLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"

namespace Metrics = RewindAuthoredCourtyardMetrics;

ARewindAuthoredCourtyard::ARewindAuthoredCourtyard()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeFinder(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeFinder.Succeeded())
	{
		CubeMesh = CubeFinder.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> UpperFinder(
		TEXT("/Game/Art/Materials/Stairwell/MI_StairWall_Upper.MI_StairWall_Upper"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> LowerFinder(
		TEXT("/Game/Art/Materials/Stairwell/MI_StairWall_Lower.MI_StairWall_Lower"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> FloorFinder(
		TEXT("/Game/Art/Materials/Stairwell/MI_StairFloor.MI_StairFloor"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MetalFinder(
		TEXT("/Game/Art/Materials/Stairwell/MI_StairMetal.MI_StairMetal"));
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> DoorFinder(
		TEXT("/Game/Art/Materials/Stairwell/MI_StairDoor.MI_StairDoor"));
	if (UpperFinder.Succeeded()) { UpperWallMaterial = UpperFinder.Object; }
	if (LowerFinder.Succeeded()) { LowerWallMaterial = LowerFinder.Object; }
	if (FloorFinder.Succeeded()) { FloorMaterial = FloorFinder.Object; }
	if (MetalFinder.Succeeded()) { MetalMaterial = MetalFinder.Object; }
	if (DoorFinder.Succeeded()) { DoorMaterial = DoorFinder.Object; }

	const float Approach = static_cast<float>(Metrics::ApproachLengthCm);
	const float Plaza = static_cast<float>(Metrics::CourtyardLengthCm);
	const float HallHalfWidth = 180.f;
	const float PlazaHalfWidth = static_cast<float>(Metrics::CourtyardWidthCm) * 0.5f;
	const float WallH = static_cast<float>(Metrics::WallHeightCm);
	const float LowerH = static_cast<float>(Metrics::LowerBandHeightCm);
	const float UpperH = WallH - LowerH;
	const float Thick = static_cast<float>(Metrics::WallThicknessCm);
	const float DoorW = static_cast<float>(Metrics::DoorWidthCm);
	const float FloorZ = static_cast<float>(-Metrics::FloorThicknessCm * 0.5);

	auto SplitWall = [this, LowerH, UpperH](const TCHAR* Base, FVector Centre, FVector Size)
	{
		const float LowerZ = LowerH * 0.5f;
		const float UpperZ = LowerH + UpperH * 0.5f;
		AddBox(*FString::Printf(TEXT("%s_Lower"), Base),
			FVector(Centre.X, Centre.Y, LowerZ), FVector(Size.X, Size.Y, LowerH), ESurface::LowerWall);
		AddBox(*FString::Printf(TEXT("%s_Upper"), Base),
			FVector(Centre.X, Centre.Y, UpperZ), FVector(Size.X, Size.Y, UpperH), ESurface::UpperWall);
	};

	AddBox(TEXT("ApproachFloor"),
		FVector(0.f, Approach * 0.5f, FloorZ),
		FVector(HallHalfWidth * 2.f, Approach, static_cast<float>(Metrics::FloorThicknessCm)),
		ESurface::Floor);
	AddBox(TEXT("PlazaFloor"),
		FVector(PlazaHalfWidth - HallHalfWidth, Approach + Plaza * 0.5f, FloorZ),
		FVector(PlazaHalfWidth * 2.f, Plaza, static_cast<float>(Metrics::FloorThicknessCm)),
		ESurface::Floor);

	// Approach is still interior: two-tone walls and a ceiling. The far +Y
	// wall is omitted; that opening is the building-to-courtyard threshold.
	SplitWall(TEXT("ApproachWest"),
		FVector(-HallHalfWidth, Approach * 0.5f, 0.f),
		FVector(Thick, Approach, WallH));
	SplitWall(TEXT("ApproachEast"),
		FVector(HallHalfWidth, Approach * 0.5f, 0.f),
		FVector(Thick, Approach, WallH));
	AddBox(TEXT("ApproachCeiling"),
		FVector(0.f, Approach * 0.5f, WallH + 5.f),
		FVector(HallHalfWidth * 2.f, Approach, 10.f),
		ESurface::UpperWall);

	const float FuseBoxY = Approach - 140.f;
	AddBox(TEXT("GroundFuseBoxPanel"),
		FVector(HallHalfWidth - Thick * 0.5f - 5.f, FuseBoxY, 100.f),
		FVector(10.f, 140.f, 180.f),
		ESurface::Metal);

	// Building face at the threshold, with a walkable opening. Barred gate
	// dressing sits in the opening without blocking the capsule.
	const float FaceY = Approach;
	const float FaceSpan = HallHalfWidth * 2.f;
	const float SideSpan = (FaceSpan - DoorW) * 0.5f;
	SplitWall(TEXT("ThresholdSouthLeft"),
		FVector(-HallHalfWidth + SideSpan * 0.5f, FaceY, 0.f),
		FVector(SideSpan, Thick, WallH));
	SplitWall(TEXT("ThresholdSouthRight"),
		FVector(HallHalfWidth - SideSpan * 0.5f, FaceY, 0.f),
		FVector(SideSpan, Thick, WallH));
	AddBox(TEXT("ThresholdLintel"),
		FVector(0.f, FaceY, WallH - 20.f),
		FVector(DoorW, Thick, 40.f),
		ESurface::UpperWall);
	AddBox(TEXT("ThresholdBarLeft"),
		FVector(-DoorW * 0.22f, FaceY + 4.f, 110.f),
		FVector(6.f, 6.f, 220.f),
		ESurface::Metal, true, false);
	AddBox(TEXT("ThresholdBarRight"),
		FVector(DoorW * 0.22f, FaceY + 4.f, 110.f),
		FVector(6.f, 6.f, 220.f),
		ESurface::Metal, true, false);

	const float PlazaCentreY = Approach + Plaza * 0.5f;
	const float PlazaMinX = -HallHalfWidth;
	const float PlazaMaxX = PlazaMinX + PlazaHalfWidth * 2.f;
	const float PlazaCentreX = (PlazaMinX + PlazaMaxX) * 0.5f;

	SplitWall(TEXT("PlazaWest"),
		FVector(PlazaMinX, PlazaCentreY, 0.f),
		FVector(Thick, Plaza, WallH));
	SplitWall(TEXT("PlazaNorth"),
		FVector(PlazaCentreX, Approach + Plaza, 0.f),
		FVector(PlazaHalfWidth * 2.f, Thick, WallH));

	// Camera-side wall is omitted so the authored +X frame can see the plaza.
	// The chain-link fence is the black-mass occluder, made of geometry.
	const float FenceX = PlazaMaxX - 40.f;
	AddBox(TEXT("FenceMass"),
		FVector(FenceX, PlazaCentreY, 35.f),
		FVector(8.f, Plaza - 80.f, 70.f),
		ESurface::Metal);
	for (int32 Post = 0; Post < 12; ++Post)
	{
		const float PostY = Approach + 80.f + Post * ((Plaza - 160.f) / 11.f);
		AddBox(*FString::Printf(TEXT("FencePost_%d"), Post),
			FVector(FenceX, PostY, 90.f),
			FVector(6.f, 6.f, 180.f),
			ESurface::Metal);
	}
	AddBox(TEXT("FenceRailTop"),
		FVector(FenceX, PlazaCentreY, 170.f),
		FVector(4.f, Plaza - 80.f, 4.f),
		ESurface::Metal);
	AddBox(TEXT("FenceRailMid"),
		FVector(FenceX, PlazaCentreY, 100.f),
		FVector(4.f, Plaza - 80.f, 4.f),
		ESurface::Metal);

	const float GeneratorX = PlazaMaxX - 420.f;
	const float GeneratorY = Approach + Plaza * 0.62f;
	AddBox(TEXT("GeneratorOutbuilding"),
		FVector(GeneratorX, GeneratorY, WallH * 0.5f),
		FVector(360.f, 420.f, WallH),
		ESurface::UpperWall);
	AddBox(TEXT("GeneratorDoor"),
		FVector(GeneratorX - 180.f, GeneratorY, 110.f),
		FVector(8.f, 120.f, 220.f),
		ESurface::Door);

	const float TransitY = Approach + Plaza - 80.f;
	const float TransitX = PlazaCentreX + 220.f;
	const float TransitOpening = 280.f;
	SplitWall(TEXT("TransitLeft"),
		FVector(TransitX - 220.f, TransitY, 0.f),
		FVector(160.f, Thick, WallH));
	SplitWall(TEXT("TransitRight"),
		FVector(TransitX + 220.f, TransitY, 0.f),
		FVector(160.f, Thick, WallH));
	AddBox(TEXT("TransitLintel"),
		FVector(TransitX, TransitY, WallH - 25.f),
		FVector(TransitOpening, Thick, 50.f),
		ESurface::Metal);
	AddBox(TEXT("TransitBlock"),
		FVector(TransitX, TransitY + 40.f, WallH * 0.5f),
		FVector(TransitOpening + 40.f, 40.f, WallH),
		ESurface::Metal);

	AddBox(TEXT("PatrolYard"),
		FVector(PlazaCentreX - 80.f, Approach + Plaza * 0.48f, FloorZ - 2.f),
		FVector(1100.f, 1100.f, 6.f),
		ESurface::Floor);

	// This actor used to carry its own 1600 lux directional moon key. A
	// directional light has no position, so it lit the whole level rather than
	// the courtyard: the authored map's own key is 10 lux, the fourth-floor
	// hall outside 4C rendered white under the level's manual -0.7 EV
	// exposure, and UE logged that several directional lights were competing
	// for forward shading every time PIE started. The courtyard keeps its
	// practicals and takes the level's single authored key like every other
	// space; a night exterior does not need a second sun.

	AddPointLight(TEXT("ThresholdPractical"), FVector(0.f, Approach - 40.f, 250.f), 1800.f, 900.f, 3200.f);
	AddPointLight(TEXT("GeneratorPractical"), FVector(GeneratorX - 200.f, GeneratorY, 230.f), 1600.f, 800.f, 3200.f);
	AddPointLight(TEXT("TransitPractical"), FVector(TransitX, TransitY - 80.f, 240.f), 1400.f, 800.f, 3400.f);
	AddPointLight(TEXT("PlazaFill"), FVector(PlazaCentreX, PlazaCentreY, 280.f), 600.f, 1600.f, 4100.f);
}

void ARewindAuthoredCourtyard::AddBox(
	FName Name, FVector RelativeLocation, FVector SizeCm, ESurface Surface, bool bVisible, bool bBlock)
{
	UStaticMeshComponent* Mesh = CreateDefaultSubobject<UStaticMeshComponent>(Name);
	Mesh->SetupAttachment(Root);
	Mesh->SetRelativeLocation(RelativeLocation);
	Mesh->SetRelativeScale3D(SizeCm / 100.f);
	Mesh->SetCollisionEnabled(bBlock ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
	Mesh->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	Mesh->SetVisibility(bVisible);
	if (CubeMesh)
	{
		Mesh->SetStaticMesh(CubeMesh);
	}
	ApplySurface(Mesh, Surface);
}

void ARewindAuthoredCourtyard::AddPointLight(
	FName Name, FVector RelativeLocation, float Intensity, float Attenuation, float Temperature)
{
	UPointLightComponent* Light = CreateDefaultSubobject<UPointLightComponent>(Name);
	Light->SetupAttachment(Root);
	Light->SetRelativeLocation(RelativeLocation);
	Light->SetMobility(EComponentMobility::Stationary);
	Light->SetIntensity(Intensity);
	Light->SetAttenuationRadius(Attenuation);
	Light->bUseTemperature = true;
	Light->Temperature = Temperature;
	Light->SetCastShadows(true);
	Light->SourceRadius = 8.f;
}

void ARewindAuthoredCourtyard::ApplySurface(UStaticMeshComponent* Mesh, ESurface Surface) const
{
	UMaterialInterface* Material = nullptr;
	switch (Surface)
	{
	case ESurface::UpperWall: Material = UpperWallMaterial; break;
	case ESurface::LowerWall: Material = LowerWallMaterial; break;
	case ESurface::Floor: Material = FloorMaterial; break;
	case ESurface::Metal: Material = MetalMaterial; break;
	case ESurface::Door: Material = DoorMaterial; break;
	}
	if (Mesh && Material)
	{
		Mesh->SetMaterial(0, Material);
	}
}

ARewindCameraRegion* ARewindAuthoredCourtyard::FindGroundHall(const UWorld* World)
{
	return ARewindCameraRegion::FindByName(World, Metrics::GroundHallRegionName);
}

FVector ARewindAuthoredCourtyard::GetThresholdWorldLocation() const
{
	return GetActorTransform().TransformPosition(
		FVector(0.f, static_cast<float>(Metrics::ThresholdLocalY()), static_cast<float>(Metrics::CheckpointHeightCm)));
}

void ARewindAuthoredCourtyard::EnsureContents()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	ARewindCameraRegion* GroundHall = FindGroundHall(World);
	if (!GroundHall)
	{
		RewindLog::Baseline(TEXT("AuthoredCourtyard: GroundHall_Region missing, not attached"));
		return;
	}

	const FVector HallCentre = GroundHall->GetActorLocation();
	const FVector HallExtent = GroundHall->GetPlayerVolumeExtent();
	const double SeamY = HallCentre.Y + HallExtent.Y;

	SetActorLocationAndRotation(
		FVector(HallCentre.X, SeamY, 0.f),
		FRotator::ZeroRotator,
		false, nullptr, ETeleportType::TeleportPhysics);

	EnsureRegions(GroundHall);
	EnsureGameplay();

	RewindLog::Baseline(FString::Printf(
		TEXT("AuthoredCourtyard: attached at GroundHall +Y seam Y=%.0f  threshold=(%.0f, %.0f, %.0f)"),
		SeamY,
		GetThresholdWorldLocation().X,
		GetThresholdWorldLocation().Y,
		GetThresholdWorldLocation().Z));
}

void ARewindAuthoredCourtyard::EnsureRegions(ARewindCameraRegion* GroundHall)
{
	UWorld* World = GetWorld();
	if (!World || !GroundHall)
	{
		return;
	}

	const FVector HallCentre = GroundHall->GetActorLocation();
	const FVector HallExtent = GroundHall->GetPlayerVolumeExtent();
	const double SeamY = HallCentre.Y + HallExtent.Y;

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const FVector ApproachCentre(
		HallCentre.X,
		SeamY + Metrics::ApproachHalfY(),
		HallCentre.Z);
	const FVector ApproachExtent(
		HallExtent.X,
		Metrics::ApproachHalfY(),
		HallExtent.Z);

	if (!ApproachRegion)
	{
		ApproachRegion = ARewindCameraRegion::FindByName(World, Metrics::ApproachRegionName);
	}
	if (!ApproachRegion)
	{
		ApproachRegion = World->SpawnActor<ARewindCameraRegion>(ApproachCentre, FRotator::ZeroRotator, Params);
	}
	if (ApproachRegion)
	{
		ApproachRegion->SetActorLocationAndRotation(
			ApproachCentre, FRotator::ZeroRotator, false, nullptr, ETeleportType::TeleportPhysics);
		ApproachRegion->Configure(
			Metrics::ApproachRegionName,
			ApproachExtent,
			ERewindTravelAxis::Y,
			GroundHall->GetCameraOffset(),
			GroundHall->GetCameraRotation(),
			GroundHall->GetTravelPadding(),
			GroundHall->GetDeadZone(),
			false,
			GroundHall->GetFieldOfView());
	}

	const double PlazaHalfWidth = Metrics::CourtyardWidthCm * 0.5;
	const double HallHalfWidth = HallExtent.X;
	const double PlazaMinX = HallCentre.X - HallHalfWidth;
	const double PlazaMaxX = PlazaMinX + PlazaHalfWidth * 2.0;
	const FVector CourtyardCentre(
		(PlazaMinX + PlazaMaxX) * 0.5,
		SeamY + Metrics::CourtyardCentreLocalY(),
		HallCentre.Z);
	const FVector CourtyardExtent(
		(PlazaMaxX - PlazaMinX) * 0.5,
		Metrics::CourtyardHalfY(),
		HallExtent.Z);

	if (!CourtyardRegion)
	{
		CourtyardRegion = ARewindCameraRegion::FindByName(World, Metrics::CourtyardRegionName);
	}
	if (!CourtyardRegion)
	{
		CourtyardRegion = World->SpawnActor<ARewindCameraRegion>(CourtyardCentre, FRotator::ZeroRotator, Params);
	}
	if (CourtyardRegion)
	{
		CourtyardRegion->SetActorLocationAndRotation(
			CourtyardCentre, FRotator::ZeroRotator, false, nullptr, ETeleportType::TeleportPhysics);
		// First-pass side-on lens: yaw 180 looks toward -X so the building face,
		// generator and Transit exit share one frame. Cut at the doorway.
		CourtyardRegion->Configure(
			Metrics::CourtyardRegionName,
			CourtyardExtent,
			ERewindTravelAxis::Y,
			FVector(1400.f, 0.f, 280.f),
			FRotator(-8.f, 180.f, 0.f),
			Metrics::CourtyardTravelPadding,
			Metrics::CourtyardDeadZone,
			true,
			Metrics::CourtyardFieldOfView);
	}

	int32 RegionCount = 0;
	for (TActorIterator<ARewindCameraRegion> It(World); It; ++It)
	{
		++RegionCount;
	}
	RewindLog::Baseline(FString::Printf(TEXT("Camera regions: %d authored after courtyard attach"), RegionCount));
}

void ARewindAuthoredCourtyard::EnsureGameplay()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	const FVector Threshold = GetThresholdWorldLocation();
	const FTransform ActorXform = GetActorTransform();

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	if (!Checkpoint)
	{
		for (TActorIterator<ARewindCausalCheckpoint> It(World); It; ++It)
		{
			if (It->GetCheckpointId() == RewindIds::CheckpointGroundFuseGate)
			{
				Checkpoint = *It;
				break;
			}
		}
	}
	if (!Checkpoint)
	{
		Checkpoint = World->SpawnActor<ARewindCausalCheckpoint>(Threshold, FRotator::ZeroRotator, Params);
		if (Checkpoint)
		{
			RewindLog::Baseline(TEXT("Checkpoint: GroundFuseGate at authored hall-to-courtyard seam"));
		}
	}
	if (Checkpoint)
	{
		Checkpoint->Configure(
			RewindIds::CheckpointGroundFuseGate,
			ERewindCheckpointPredicate::GroundFuseGate,
			FVector(
				Metrics::CheckpointExtentXCm,
				Metrics::CheckpointExtentYCm,
				Metrics::CheckpointExtentZCm));
		Checkpoint->SetActorLocationAndRotation(
			Threshold, FRotator::ZeroRotator, false, nullptr, ETeleportType::TeleportPhysics);
	}

	const FVector SocketLocation = ActorXform.TransformPosition(
		FVector(150.0, Metrics::ThresholdLocalY() - 140.0, 60.0));
	if (!GroundSocket)
	{
		GroundSocket = ARewindFuseSocket::Find(World, ERewindFuseSocket::Courtyard);
	}
	if (!GroundSocket)
	{
		GroundSocket = World->SpawnActor<ARewindFuseSocket>(SocketLocation, FRotator(0.f, 180.f, 0.f), Params);
	}
	if (GroundSocket)
	{
		GroundSocket->Configure(ERewindFuseSocket::Courtyard);
		GroundSocket->SetActorLocationAndRotation(
			SocketLocation, FRotator(0.f, 180.f, 0.f), false, nullptr, ETeleportType::TeleportPhysics);
	}

	const FVector GeneratorLocation = ActorXform.TransformPosition(
		FVector(Metrics::CourtyardWidthCm * 0.5 - 360.0,
			Metrics::ApproachLengthCm + Metrics::CourtyardLengthCm * 0.62,
			50.0));
	Generator = FindOrSpawn(Generator, GeneratorLocation);
	if (Generator)
	{
		Generator->SetActorLocationAndRotation(
			GeneratorLocation, FRotator(0.f, -90.f, 0.f), false, nullptr, ETeleportType::TeleportPhysics);
	}

	const FVector GateLocation = ActorXform.TransformPosition(
		FVector(Metrics::CourtyardWidthCm * 0.25,
			Metrics::ApproachLengthCm + 900.0,
			150.0));
	Gate = FindOrSpawn(Gate, GateLocation);
	if (Gate)
	{
		// Default mesh is thin on actor X and 760 cm on actor Y. Yaw 90 spans
		// world X and blocks walking +Y toward Transit.
		Gate->SetActorLocationAndRotation(
			GateLocation, FRotator(0.f, 90.f, 0.f), false, nullptr, ETeleportType::TeleportPhysics);
	}
	if (Generator)
	{
		Generator->SetGate(Gate);
	}
}

template <typename T>
T* ARewindAuthoredCourtyard::FindOrSpawn(TObjectPtr<T>& Cache, const FVector& WorldLocation)
{
	if (Cache)
	{
		return Cache.Get();
	}
	UWorld* World = GetWorld();
	if (!World)
	{
		return nullptr;
	}
	for (TActorIterator<T> It(World); It; ++It)
	{
		Cache = *It;
		return Cache.Get();
	}
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Cache = World->SpawnActor<T>(WorldLocation, FRotator::ZeroRotator, Params);
	return Cache.Get();
}
