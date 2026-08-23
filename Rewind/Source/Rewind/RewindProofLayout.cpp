#include "RewindProofLayout.h"

#include "RewindAnchorBoard.h"
#include "RewindCameraRegion.h"
#include "RewindCameraRig.h"
#include "RewindCodeLock.h"
#include "RewindCourtyardGate.h"
#include "RewindFourCBlockout.h"
#include "RewindFuse.h"
#include "RewindFuseSocket.h"
#include "RewindGenerator.h"
#include "RewindPatrol.h"
#include "RewindRadio.h"
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

	auto AddFloor = [this, Cube, Root](FName Name, FVector Location, FVector SizeCm)
	{
		UStaticMeshComponent* Mesh = CreateDefaultSubobject<UStaticMeshComponent>(Name);
		Mesh->SetupAttachment(Root);
		Mesh->SetRelativeLocation(Location);
		Mesh->SetRelativeScale3D(SizeCm / 100.f);
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Mesh->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
		if (Cube)
		{
			Mesh->SetStaticMesh(Cube);
		}
	};

	// The walkable run is continuous from the 4C door at X=400 to the hub end at
	// X=3250. The courtyard slab covers 400 to 1450 so there is no gap under the
	// code lock at X=400 or under the gate at X=1420.
	AddFloor(TEXT("CourtyardFloor"), FVector(925.f, 0.f, -5.f), FVector(1050.f, 800.f, 10.f));
	AddFloor(TEXT("StreetFloor"), FVector(1900.f, 0.f, -5.f), FVector(900.f, 800.f, 10.f));
	AddFloor(TEXT("HubFloor"), FVector(2800.f, 0.f, -5.f), FVector(900.f, 800.f, 10.f));

	// The outdoor run is bounded on both sides and at the far end. Without this
	// the courtyard, street and hub are slabs in a void: the player walks off,
	// nothing catches them, and no FL criterion can be reached from there.
	// Falling is not a death rule; the space simply has no hole in it.
	const float EdgeCenterX = 1825.f;
	const float EdgeLengthX = 2850.f;
	const float EdgeHeight = 300.f;
	const float EdgeThickness = 20.f;
	const float EdgeY = 390.f;

	// Camera side: collision without visibility, for the same reason 4C has no
	// fourth wall. It still stops the player leaving the run.
	AddFloor(TEXT("EdgeWall_YNeg"), FVector(EdgeCenterX, -EdgeY, EdgeHeight * 0.5f),
		FVector(EdgeLengthX, EdgeThickness, EdgeHeight));
	if (UStaticMeshComponent* NearEdge = Cast<UStaticMeshComponent>(
			GetDefaultSubobjectByName(TEXT("EdgeWall_YNeg"))))
	{
		NearEdge->SetVisibility(false);
	}
	AddFloor(TEXT("EdgeWall_YPos"), FVector(EdgeCenterX, EdgeY, EdgeHeight * 0.5f),
		FVector(EdgeLengthX, EdgeThickness, EdgeHeight));
	AddFloor(TEXT("EdgeWall_XEnd"), FVector(3240.f, 0.f, EdgeHeight * 0.5f),
		FVector(EdgeThickness, 800.f, EdgeHeight));
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
		Blockout->EnsureLoopStart();
	}

	Radio = EnsureActor(Radio, FVector(-180.f, -220.f, 80.f));
	Board = EnsureActor(Board, FVector(0.f, -360.f, 100.f));

	// The fuse rests in 4C at Baseline, per `chapter-1-authored.md`.
	Fuse = EnsureActor(Fuse, FVector(-180.f, 220.f, 80.f));
	CodeLock = EnsureActor(CodeLock, FVector(400.f, 0.f, 120.f));
	Generator = EnsureActor(Generator, FVector(800.f, -220.f, 50.f));
	Gate = EnsureActor(Gate, FVector(1420.f, 0.f, 150.f));
	Patrol = EnsureActor(Patrol, FVector(1900.f, 0.f, 0.f));
	Turnstile = EnsureActor(Turnstile, FVector(2460.f, 0.f, 110.f));

	EnsureFuseSockets();

	if (Generator)
	{
		Generator->SetGate(Gate);
	}

	EnsureLights();
	EnsureExposure();
	EnsureCamera();
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
	// Both positions are provisional. The building socket belongs beside the
	// lift, and the landing and shaft do not exist yet; it sits in 4C until the
	// space is built. The courtyard socket is kept clear of the generator so
	// that seating the fuse and starting the generator remain two separate
	// interactions, which is what makes the sequence legible.
	auto EnsureSocket = [&](TObjectPtr<ARewindFuseSocket>& Cache, ERewindFuseSocket Which, const FVector& Where)
	{
		if (Cache)
		{
			return;
		}
		if (ARewindFuseSocket* Existing = ARewindFuseSocket::Find(World, Which))
		{
			Cache = Existing;
			return;
		}
		Cache = World->SpawnActor<ARewindFuseSocket>(Where, FRotator::ZeroRotator, Params);
		if (Cache)
		{
			Cache->Configure(Which);
		}
	};

	EnsureSocket(BuildingSocket, ERewindFuseSocket::Building, FVector(300.f, 220.f, 80.f));
	EnsureSocket(CourtyardSocket, ERewindFuseSocket::Courtyard, FVector(560.f, -220.f, 60.f));
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

	// One region per authored area, matching the blockout this rebuild starts
	// from. `camera-and-movement.md` requires every playable location to be in
	// exactly one region, so these spans are contiguous and cover the whole run
	// from 4C at X=-400 to the hub end at X=3250.
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
			double CentreX;
			double HalfX;
		};
		// Contiguous by construction: 4C -400..400, courtyard 400..1450,
		// street 1450..2350, hub 2350..3250.
		const FRegionSpec Specs[] = {
			{ TEXT("Apartment4C"), 0.0,    400.0 },
			{ TEXT("Courtyard"),   925.0,  525.0 },
			{ TEXT("Street"),      1900.0, 450.0 },
			{ TEXT("TransitHub"),  2800.0, 450.0 },
		};

		for (const FRegionSpec& Spec : Specs)
		{
			ARewindCameraRegion* Region = World->SpawnActor<ARewindCameraRegion>(
				FVector(Spec.CentreX, 0.f, 150.f), FRotator::ZeroRotator, Params);
			if (Region)
			{
				Region->Configure(FName(Spec.Name), FVector(Spec.HalfX, 380.f, 400.f));
			}
		}
	}

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
