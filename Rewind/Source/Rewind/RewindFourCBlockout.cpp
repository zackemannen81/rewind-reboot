#include "RewindFourCBlockout.h"

#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

namespace
{
	constexpr float ApartmentWidth = 800.f;
	constexpr float ApartmentDepth = 800.f;
	constexpr float WallHeight = 300.f;
	constexpr float WallThickness = 20.f;
	constexpr float DoorWidth = 220.f;
}

ARewindFourCBlockout::ARewindFourCBlockout()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeFinder(
		TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeFinder.Succeeded())
	{
		CubeMesh = CubeFinder.Object;
	}

	AddBox(TEXT("Floor"), FVector(0.f, 0.f, -5.f), FVector(ApartmentWidth, ApartmentDepth, 10.f));

	const float HalfW = ApartmentWidth * 0.5f;
	const float HalfD = ApartmentDepth * 0.5f;
	const float WallZ = WallHeight * 0.5f;

	AddBox(TEXT("Wall_West"), FVector(0.f, -HalfD + WallThickness * 0.5f, WallZ),
		FVector(ApartmentWidth, WallThickness, WallHeight));
	AddBox(TEXT("Wall_East"), FVector(0.f, HalfD - WallThickness * 0.5f, WallZ),
		FVector(ApartmentWidth, WallThickness, WallHeight));
	AddBox(TEXT("Wall_South"), FVector(-HalfW + WallThickness * 0.5f, 0.f, WallZ),
		FVector(WallThickness, ApartmentDepth, WallHeight));

	const float NorthSegment = (ApartmentWidth - DoorWidth) * 0.5f;
	AddBox(TEXT("Wall_North_Left"),
		FVector(HalfW - WallThickness * 0.5f, -HalfD + NorthSegment * 0.5f, WallZ),
		FVector(WallThickness, NorthSegment, WallHeight));
	AddBox(TEXT("Wall_North_Right"),
		FVector(HalfW - WallThickness * 0.5f, HalfD - NorthSegment * 0.5f, WallZ),
		FVector(WallThickness, NorthSegment, WallHeight));
}

void ARewindFourCBlockout::BeginPlay()
{
	Super::BeginPlay();
	EnsureLoopStart();
}

FVector ARewindFourCBlockout::GetLoopStartLocation()
{
	return FVector(-150.f, 0.f, 92.f);
}

FRotator ARewindFourCBlockout::GetLoopStartRotation()
{
	return FRotator(0.f, 0.f, 0.f);
}

void ARewindFourCBlockout::AddBox(FName Name, FVector RelativeLocation, FVector SizeCm)
{
	UStaticMeshComponent* Mesh = CreateDefaultSubobject<UStaticMeshComponent>(Name);
	Mesh->SetupAttachment(Root);
	Mesh->SetRelativeLocation(RelativeLocation);
	Mesh->SetRelativeScale3D(SizeCm / 100.f);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	if (CubeMesh)
	{
		Mesh->SetStaticMesh(CubeMesh);
	}
}

void ARewindFourCBlockout::EnsureLoopStart() const
{
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	for (TActorIterator<APlayerStart> It(World); It; ++It)
	{
		if (It->PlayerStartTag == FName(TEXT("LoopStart")))
		{
			return;
		}
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	APlayerStart* Start = World->SpawnActor<APlayerStart>(
		GetLoopStartLocation(), GetLoopStartRotation(), Params);
	if (Start)
	{
		Start->PlayerStartTag = FName(TEXT("LoopStart"));
	}
}
