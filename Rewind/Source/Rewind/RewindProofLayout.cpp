#include "RewindProofLayout.h"

#include "RewindAnchorBoard.h"
#include "RewindCodeLock.h"
#include "RewindCourtyardGate.h"
#include "RewindFourCBlockout.h"
#include "RewindFuseBox.h"
#include "RewindGenerator.h"
#include "RewindPatrol.h"
#include "RewindRadio.h"
#include "RewindTurnstile.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SkyLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/DirectionalLight.h"
#include "Engine/SkyLight.h"
#include "Engine/PointLight.h"
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
		if (Cube)
		{
			Mesh->SetStaticMesh(Cube);
		}
	};

	AddFloor(TEXT("CourtyardFloor"), FVector(920.f, 0.f, -5.f), FVector(1000.f, 800.f, 10.f));
	AddFloor(TEXT("StreetFloor"), FVector(1900.f, 0.f, -5.f), FVector(900.f, 800.f, 10.f));
	AddFloor(TEXT("HubFloor"), FVector(2800.f, 0.f, -5.f), FVector(900.f, 800.f, 10.f));
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
	FuseBox = EnsureActor(FuseBox, FVector(-180.f, 220.f, 80.f));
	CodeLock = EnsureActor(CodeLock, FVector(400.f, 0.f, 120.f));
	Generator = EnsureActor(Generator, FVector(800.f, -220.f, 50.f));
	Gate = EnsureActor(Gate, FVector(1420.f, 0.f, 150.f));
	Patrol = EnsureActor(Patrol, FVector(1900.f, 0.f, 0.f));
	Turnstile = EnsureActor(Turnstile, FVector(2460.f, 0.f, 110.f));

	if (Generator)
	{
		Generator->SetLinks(FuseBox, Gate);
	}

	EnsureLights();
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
				Light->SetLightUnits(ELightUnits::Unitless);
				Light->SetIntensity(12.f);
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
				Light->SetLightUnits(ELightUnits::Candelas);
				Light->SetIntensity(2000.f);
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
