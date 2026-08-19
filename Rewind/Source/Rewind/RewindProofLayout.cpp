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
#include "Components/StaticMeshComponent.h"
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
