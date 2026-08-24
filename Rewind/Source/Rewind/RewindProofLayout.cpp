#include "RewindProofLayout.h"

#include "RewindAnchorBoard.h"
#include "RewindCameraRegion.h"
#include "RewindCameraRig.h"
#include "RewindChapter1Metrics.h"
#include "RewindCodeLock.h"
#include "RewindCourtyardGate.h"
#include "RewindFourCBlockout.h"
#include "RewindFuse.h"
#include "RewindFuseSocket.h"
#include "RewindGenerator.h"
#include "RewindLift.h"
#include "RewindLog.h"
#include "RewindPatrol.h"
#include "RewindRadio.h"
#include "RewindStairwell.h"
#include "RewindTurnstile.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/CollisionProfile.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "Engine/PointLight.h"
#include "Engine/PostProcessVolume.h"
#include "EngineUtils.h"
#include "UObject/ConstructorHelpers.h"

ARewindProofLayout::ARewindProofLayout()
{
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeFinder(TEXT("/Engine/BasicShapes/Cube.Cube"));
	UStaticMesh* Cube = CubeFinder.Succeeded() ? CubeFinder.Object : nullptr;

	auto AddBox = [this, Cube, Root](FName Name, FVector Location, FVector SizeCm, bool bVisible = true)
	{
		UStaticMeshComponent* Mesh = CreateDefaultSubobject<UStaticMeshComponent>(Name);
		Mesh->SetupAttachment(Root);
		Mesh->SetRelativeLocation(Location);
		Mesh->SetRelativeScale3D(SizeCm / 100.f);
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Mesh->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
		Mesh->SetVisibility(bVisible);
		if (Cube)
		{
			Mesh->SetStaticMesh(Cube);
		}
	};

	const float FourthFloor = static_cast<float>(RewindChapter1Metrics::FourthFloorZ);

	// Owner top plan: stairs, lift and 4C form the upper row; their common hall
	// is the parallel rectangle below. 4C crosses that shared wall at its own
	// door, while the hall/lift camera is authored from the opposite side.
	AddBox(TEXT("FourthFloorHallway"), FVector(2600.f, -300.f, FourthFloor - 5.f),
		FVector(5200.f, 600.f, 10.f));

	// Bottom plan: both vertical routes meet a long hall whose fuse-box end
	// opens into one large courtyard. The learned route runs straight along the
	// courtyard's top edge to Transit. The fuse route is a 170 m U-shaped
	// service walk around the patrol yard to a generator near the far edge, then
	// back again; it remains a branch rather than an incidental stop.
	AddBox(TEXT("EntranceHallway"), FVector(755.f, -300.f, -5.f),
		FVector(6710.f, 600.f, 10.f));
	AddBox(TEXT("GroundFuseBoxPanel"), FVector(3900.f, -10.f, 100.f),
		FVector(140.f, 20.f, 180.f));
	AddBox(TEXT("CourtyardMain"), FVector(8110.f, 0.f, -5.f),
		FVector(8000.f, 800.f, 10.f));
	AddBox(TEXT("HubFloor"), FVector(13010.f, 0.f, -5.f),
		FVector(1800.f, 800.f, 10.f));
	AddBox(TEXT("PowerBranchLeft"), FVector(4510.f, -2777.5f, -5.f),
		FVector(800.f, 5555.f, 10.f));
	AddBox(TEXT("PowerBranchBottom"), FVector(7755.f, -5555.f, -5.f),
		FVector(6490.f, 800.f, 10.f));
	AddBox(TEXT("PowerBranchRight"), FVector(11000.f, -3077.5f, -5.f),
		FVector(800.f, 4955.f, 10.f));
	AddBox(TEXT("PatrolYard"), FVector(7755.f, -2777.5f, -8.f),
		FVector(5690.f, 4755.f, 6.f));

	// Narrow collision edges keep the branch honest around the yard. The lower
	// outer edge is hidden because the courtyard camera is authored from there.
	constexpr float ProofWallHeight = 300.f;
	constexpr float ProofWallThickness = 20.f;
	AddBox(TEXT("MainRouteFarWall"), FVector(9010.f, 390.f, ProofWallHeight * .5f),
		FVector(9800.f, ProofWallThickness, ProofWallHeight));
	AddBox(TEXT("BranchInnerTop"), FVector(7755.f, -410.f, ProofWallHeight * .5f),
		FVector(5690.f, ProofWallThickness, ProofWallHeight));
	AddBox(TEXT("GeneratorDeadEnd"), FVector(11000.f, -410.f, ProofWallHeight * .5f),
		FVector(800.f, ProofWallThickness, ProofWallHeight));
	AddBox(TEXT("HubNearWall"), FVector(12650.f, -410.f, ProofWallHeight * .5f),
		FVector(2500.f, ProofWallThickness, ProofWallHeight), false);
	AddBox(TEXT("BranchInnerLeft"), FVector(4910.f, -2777.5f, ProofWallHeight * .5f),
		FVector(ProofWallThickness, 4755.f, ProofWallHeight));
	AddBox(TEXT("BranchInnerBottom"), FVector(7755.f, -5155.f, ProofWallHeight * .5f),
		FVector(5690.f, ProofWallThickness, ProofWallHeight));
	AddBox(TEXT("BranchInnerRight"), FVector(10600.f, -2777.5f, ProofWallHeight * .5f),
		FVector(ProofWallThickness, 4755.f, ProofWallHeight));
	// Leave the upper 400 cm open where the entrance hall feeds the main route
	// and the service branch. The wall begins below that junction and still
	// bounds the full long leg of the U.
	AddBox(TEXT("BranchOuterLeft"), FVector(4110.f, -2977.5f, ProofWallHeight * .5f),
		FVector(ProofWallThickness, 5155.f, ProofWallHeight));
	AddBox(TEXT("BranchOuterBottom"), FVector(7755.f, -5955.f, ProofWallHeight * .5f),
		FVector(7290.f, ProofWallThickness, ProofWallHeight), false);
	AddBox(TEXT("BranchOuterRight"), FVector(11400.f, -3077.5f, ProofWallHeight * .5f),
		FVector(ProofWallThickness, 5755.f, ProofWallHeight));
	AddBox(TEXT("HubEnd"), FVector(13900.f, 0.f, ProofWallHeight * .5f),
		FVector(ProofWallThickness, 800.f, ProofWallHeight));
}

void ARewindProofLayout::EnsureContents()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	if (!Blockout)
	{
		for (TActorIterator<ARewindFourCBlockout> It(World); It; ++It)
		{
			Blockout = *It;
			break;
		}
	}
	if (!Blockout)
	{
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Blockout = World->SpawnActor<ARewindFourCBlockout>(FVector::ZeroVector, FRotator::ZeroRotator, Params);
	}
	if (Blockout)
	{
		Blockout->SetActorLocationAndRotation(FVector(4000.f, 500.f, 0.f),
			FRotator::ZeroRotator, false, nullptr, ETeleportType::TeleportPhysics);
		Blockout->EnsureLoopStart();
	}

	const float FourthFloor = static_cast<float>(RewindChapter1Metrics::FourthFloorZ);
	auto Place = [](AActor* Actor, const FVector& Location, const FRotator& Rotation = FRotator::ZeroRotator)
	{
		if (Actor)
		{
			Actor->SetActorLocationAndRotation(Location, Rotation, false, nullptr,
				ETeleportType::TeleportPhysics);
		}
	};

	Radio = EnsureActor(Radio, FVector(4400.f, 200.f, FourthFloor + 95.f));
	Place(Radio, FVector(4400.f, 200.f, FourthFloor + 95.f));

	// The fuse rests in 4C at Baseline, then chooses between the lift socket on
	// the fourth-floor landing and the generator socket at the branch end.
	Fuse = EnsureActor(Fuse, FVector(4900.f, 150.f, FourthFloor + 90.f));
	Place(Fuse, FVector(4900.f, 150.f, FourthFloor + 90.f));
	CodeLock = EnsureActor(CodeLock, FVector(3400.f, 0.f, FourthFloor + 120.f));
	Place(CodeLock, FVector(3400.f, 0.f, FourthFloor + 120.f), FRotator(0.f, 90.f, 0.f));

	Lift = EnsureActor(Lift, FVector(2200.f, 100.f, 0.f));
	Place(Lift, FVector(2200.f, 100.f, 0.f));
	if (Lift)
	{
		Lift->Configure(RewindChapter1Metrics::FourthFloorZ, 0.0);
	}
	Stairwell = EnsureActor(Stairwell, FVector(1000.f, 200.f, 0.f));
	Place(Stairwell, FVector(1000.f, 200.f, 0.f), FRotator(0.f, 180.f, 0.f));

	Generator = EnsureActor(Generator, FVector(11000.f, -600.f, 50.f));
	Place(Generator, FVector(11000.f, -600.f, 50.f));
	Board = EnsureActor(Board, FVector(11000.f, -850.f, 100.f));
	Place(Board, FVector(11000.f, -850.f, 100.f));
	Gate = EnsureActor(Gate, FVector(5510.f, 0.f, 150.f));
	Place(Gate, FVector(5510.f, 0.f, 150.f));
	Patrol = EnsureActor(Patrol, FVector(8510.f, 0.f, 0.f));
	Place(Patrol, FVector(8510.f, 0.f, 0.f));
	Turnstile = EnsureActor(Turnstile, FVector(12110.f, 0.f, 110.f));
	Place(Turnstile, FVector(12110.f, 0.f, 110.f));
	if (Turnstile)
	{
		Turnstile->ConfigureHubDirection(FVector(1.f, 0.f, 0.f));
	}

	EnsureFuseSockets();

	if (Generator)
	{
		Generator->SetGate(Gate);
	}

	EnsureLights();
	EnsureExposure();
	EnsureCamera();

	RewindLog::Baseline(FString::Printf(
		TEXT("Chapter1 routes: lift %.2fs; stairs minimum %.2fs; delta %.2fs; LoopB model %.2fs; LoopC model %.2fs"),
		RewindChapter1Metrics::LiftTravelSeconds,
		RewindChapter1Metrics::StairMinimumSeconds(),
		RewindChapter1Metrics::StairMinimumSeconds() - RewindChapter1Metrics::LiftTravelSeconds,
		RewindChapter1Metrics::FuseToCourtyardRouteSeconds(),
		RewindChapter1Metrics::LearnedRouteSeconds()));
}

void ARewindProofLayout::EnsureFuseSockets()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Two sockets of one class, so EnsureActor cannot be used: it finds by type
	// and would hand back the same actor twice.
	//
	// The building socket is on the 4C landing beside the shaft. The courtyard
	// socket is kept clear of the generator so seating the fuse and starting it
	// remain two readable interactions.
	auto EnsureSocket = [&](TObjectPtr<ARewindFuseSocket>& Cache, ERewindFuseSocket Which, const FVector& Where)
	{
		if (!Cache)
		{
			if (ARewindFuseSocket* Existing = ARewindFuseSocket::Find(World, Which))
			{
				Cache = Existing;
			}
		}
		if (!Cache)
		{
			Cache = World->SpawnActor<ARewindFuseSocket>(Where, FRotator::ZeroRotator, Params);
		}
		if (Cache)
		{
			Cache->Configure(Which);
			Cache->SetActorLocation(Where, false, nullptr, ETeleportType::TeleportPhysics);
		}
	};

	EnsureSocket(BuildingSocket, ERewindFuseSocket::Building,
		FVector(2450.f, 0.f, static_cast<float>(RewindChapter1Metrics::FourthFloorZ + 80.0)));
	EnsureSocket(CourtyardSocket, ERewindFuseSocket::Courtyard, FVector(3900.f, -180.f, 60.f));
}

void ARewindProofLayout::EnsureCamera()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// `camera-and-movement.md` requires every playable location to declare its
	// complete authored camera. The owner's two plan drawings provide three
	// opposing compositions: 4C/stairs from +Y, both common halls from -Y, and
	// the courtyard from +Y. The large courtyard is one authored room even
	// though its collision makes a U-shaped service route around the patrol yard.
	bool bHasRegion = false;
	for (TActorIterator<ARewindCameraRegion> It(World); It; ++It)
	{
		bHasRegion = true;
		break;
	}
	if (!bHasRegion)
	{
		struct FRegionSpec
		{
			const TCHAR* Name;
			FVector Centre;
			FVector Extent;
			ERewindTravelAxis Axis;
			FVector CameraOffset;
			FRotator CameraRotation;
			double Padding;
			double DeadZone;
			bool bCut;
		};
		const FVector TopOffset(0.f, 1800.f, 300.f);
		const FRotator TopRotation(-6.f, -90.f, 0.f);
		const FVector HallOffset(0.f, -1800.f, 300.f);
		const FRotator HallRotation(-6.f, 90.f, 0.f);
		const FVector CourtyardOffset(0.f, 4000.f, 700.f);
		const FRotator CourtyardRotation(-8.f, -90.f, 0.f);
		const FRegionSpec Specs[] = {
			{ TEXT("Apartment4C"), FVector(4000.f, 500.f, 1350.f), FVector(1200.f, 500.f, 250.f),
				ERewindTravelAxis::X, TopOffset, TopRotation, 300.0, 220.0, false },
			{ TEXT("FourthFloorHallway"), FVector(2600.f, -300.f, 1350.f), FVector(2600.f, 300.f, 250.f),
				ERewindTravelAxis::X, HallOffset, HallRotation, 320.0, 220.0, true },
			{ TEXT("LiftShaft"), FVector(2200.f, 100.f, 700.f), FVector(250.f, 260.f, 400.f),
				ERewindTravelAxis::X, HallOffset, HallRotation, 100.0, 80.0, true },
			{ TEXT("Stairs4To3"), FVector(-680.f, 0.f, 1097.5f), FVector(1680.f, 220.f, 202.5f),
				ERewindTravelAxis::X, TopOffset, TopRotation, 300.0, 220.0, false },
			{ TEXT("Floor3Landing"), FVector(-2360.f, 200.f, 850.f), FVector(250.f, 400.f, 80.f),
				ERewindTravelAxis::X, TopOffset, TopRotation, 140.0, 120.0, false },
			{ TEXT("Stairs3To2"), FVector(-680.f, 400.f, 697.5f), FVector(1680.f, 220.f, 202.5f),
				ERewindTravelAxis::X, TopOffset, TopRotation, 300.0, 220.0, false },
			{ TEXT("Floor2Landing"), FVector(1000.f, 200.f, 450.f), FVector(250.f, 400.f, 80.f),
				ERewindTravelAxis::X, TopOffset, TopRotation, 140.0, 120.0, false },
			{ TEXT("Stairs2ToEntrance"), FVector(-680.f, 0.f, 297.5f), FVector(1680.f, 220.f, 202.5f),
				ERewindTravelAxis::X, TopOffset, TopRotation, 300.0, 220.0, false },
			{ TEXT("EntranceHallway"), FVector(755.f, -300.f, 150.f), FVector(3355.f, 300.f, 150.f),
				ERewindTravelAxis::X, HallOffset, HallRotation, 400.0, 240.0, true },
			{ TEXT("Courtyard"), FVector(8110.f, -2777.5f, 150.f), FVector(4000.f, 3177.5f, 150.f),
				ERewindTravelAxis::X, CourtyardOffset, CourtyardRotation, 500.0, 260.0, true },
			{ TEXT("TransitHub"), FVector(13010.f, 0.f, 150.f), FVector(900.f, 400.f, 150.f),
				ERewindTravelAxis::X, TopOffset, TopRotation, 220.0, 180.0, false },
		};

		for (const FRegionSpec& Spec : Specs)
		{
			ARewindCameraRegion* Region = World->SpawnActor<ARewindCameraRegion>(
				Spec.Centre, FRotator::ZeroRotator, Params);
			if (Region)
			{
				Region->Configure(FName(Spec.Name), Spec.Extent, Spec.Axis,
					Spec.CameraOffset, Spec.CameraRotation, Spec.Padding,
					Spec.DeadZone, Spec.bCut);
			}
		}
	}

	int32 RegionCount = 0;
	for (TActorIterator<ARewindCameraRegion> It(World); It; ++It)
	{
		++RegionCount;
		RewindLog::Baseline(FString::Printf(
			TEXT("Camera region: %s axis=%s extent=(%.0f,%.0f,%.0f) padding=%.0f deadzone=%.0f"),
			*It->GetRegionName().ToString(),
			It->GetTravelAxis() == ERewindTravelAxis::X ? TEXT("X")
				: It->GetTravelAxis() == ERewindTravelAxis::Y ? TEXT("Y") : TEXT("Z"),
			It->GetPlayerVolumeExtent().X, It->GetPlayerVolumeExtent().Y,
			It->GetPlayerVolumeExtent().Z, It->GetTravelPadding(), It->GetDeadZone()));
	}
	RewindLog::Baseline(FString::Printf(TEXT("Camera regions: %d authored"), RegionCount));

	if (!CameraRig)
	{
		for (TActorIterator<ARewindCameraRig> It(World); It; ++It)
		{
			CameraRig = *It;
			break;
		}
	}
	if (!CameraRig)
	{
		CameraRig = World->SpawnActor<ARewindCameraRig>(FVector::ZeroVector, FRotator::ZeroRotator, Params);
	}
}

void ARewindProofLayout::EnsureExposure()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	for (TActorIterator<APostProcessVolume> It(World); It; ++It)
	{
		return;
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	APostProcessVolume* Volume = World->SpawnActor<APostProcessVolume>(
		FVector::ZeroVector, FRotator::ZeroRotator, Params);
	if (!Volume)
	{
		return;
	}

	// `r.DefaultFeature.AutoExposure=False` leaves exposure fixed, and the sun is
	// authored at 50000 lux. Nothing reconciles the two, so lit surfaces clip to
	// white and everything else reads as black. Exposure is therefore authored
	// here, once, next to the lights it has to match.
	//
	// Manual exposure is driven by the camera triangle. ISO 100, 1/125 s and
	// f/16 is the sunny-16 exposure for direct daylight, which is what a 50000
	// lux sun is. This is a legibility fix for the FL run, not a look: FL-01,
	// FL-07 and FL-11 are read off gate and generator state, and FL-09 and FL-12
	// are read off a moving patrol and turnstile, none of which can be judged in
	// a two-value image.
	Volume->bUnbound = true;
	Volume->BlendWeight = 1.f;

	FPostProcessSettings& Settings = Volume->Settings;
	Settings.bOverride_AutoExposureMethod = true;
	Settings.AutoExposureMethod = AEM_Manual;
	Settings.bOverride_AutoExposureBias = true;
	Settings.AutoExposureBias = 0.f;
	Settings.bOverride_CameraISO = true;
	Settings.CameraISO = 100.f;
	Settings.bOverride_CameraShutterSpeed = true;
	Settings.CameraShutterSpeed = 125.f;
	Settings.bOverride_DepthOfFieldFstop = true;
	Settings.DepthOfFieldFstop = 16.f;
}

void ARewindProofLayout::EnsureLights()
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	bool bHasSun = false;
	for (TActorIterator<ADirectionalLight> It(World); It; ++It)
	{
		bHasSun = true;
		break;
	}
	if (!bHasSun)
	{
		ADirectionalLight* Sun = World->SpawnActor<ADirectionalLight>(
			FVector(0.f, 0.f, 800.f), FRotator(-50.f, -35.f, 0.f), Params);
		if (Sun)
		{
			if (UDirectionalLightComponent* Light = Cast<UDirectionalLightComponent>(Sun->GetLightComponent()))
			{
				Light->SetMobility(EComponentMobility::Stationary);
				Light->SetIntensity(50000.f);
				Light->bUseTemperature = true;
				Light->Temperature = 5500.f;
				Light->SetAtmosphereSunLight(true);
			}
		}
	}

	bool bHasSky = false;
	for (TActorIterator<ASkyLight> It(World); It; ++It)
	{
		bHasSky = true;
		break;
	}
	if (!bHasSky)
	{
		ASkyLight* Sky = World->SpawnActor<ASkyLight>(FVector(0.f, 0.f, 400.f), FRotator::ZeroRotator, Params);
		if (Sky)
		{
			if (USkyLightComponent* Light = Sky->GetLightComponent())
			{
				Light->SetMobility(EComponentMobility::Stationary);
				Light->SetIntensity(1.5f);
				Light->bLowerHemisphereIsBlack = false;
				Light->LowerHemisphereColor = FLinearColor(0.15f, 0.16f, 0.2f);
				Light->RecaptureSky();
			}
		}
	}

	bool bHasPoint = false;
	for (TActorIterator<APointLight> It(World); It; ++It)
	{
		bHasPoint = true;
		break;
	}
	if (!bHasPoint)
	{
		APointLight* Lamp = World->SpawnActor<APointLight>(
			FVector(0.f, 0.f, 220.f), FRotator::ZeroRotator, Params);
		if (Lamp)
		{
			if (UPointLightComponent* Light = Cast<UPointLightComponent>(Lamp->GetLightComponent()))
			{
				Light->SetMobility(EComponentMobility::Stationary);
				Light->SetIntensity(5000.f);
				Light->SetAttenuationRadius(1800.f);
			}
		}
	}
}

template <typename T>
T* ARewindProofLayout::EnsureActor(TObjectPtr<T>& Cache, const FVector& Location)
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
	Cache = World->SpawnActor<T>(Location, FRotator::ZeroRotator, Params);
	return Cache;
}
