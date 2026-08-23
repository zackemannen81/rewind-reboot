#include "RewindFourCBlockout.h"

#include "RewindChapter1Metrics.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

namespace
{
	constexpr float ApartmentWidth = 2400.f;
	constexpr float ApartmentDepth = 1000.f;
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

	const float FloorZ = static_cast<float>(RewindChapter1Metrics::FourthFloorZ);
	AddBox(TEXT("Floor"), FVector(0.f, 0.f, FloorZ - 5.f), FVector(ApartmentWidth, ApartmentDepth, 10.f));

	const float HalfW = ApartmentWidth * 0.5f;
	const float HalfD = ApartmentDepth * 0.5f;
	const float WallZ = FloorZ + WallHeight * 0.5f;

	// The owner plan films 4C from +Y, opposite the common hallway. The near
	// wall therefore keeps collision but is hidden so the authored frame can
	// see the whole room.
	AddBox(TEXT("Wall_North"), FVector(0.f, HalfD - WallThickness * 0.5f, WallZ),
		FVector(ApartmentWidth, WallThickness, WallHeight));
	if (UStaticMeshComponent* NearWall = Cast<UStaticMeshComponent>(
			GetDefaultSubobjectByName(TEXT("Wall_North"))))
	{
		NearWall->SetVisibility(false);
	}
	AddBox(TEXT("Wall_West"), FVector(-HalfW + WallThickness * 0.5f, 0.f, WallZ),
		FVector(WallThickness, ApartmentDepth, WallHeight));
	AddBox(TEXT("Wall_East"), FVector(HalfW - WallThickness * 0.5f, 0.f, WallZ),
		FVector(WallThickness, ApartmentDepth, WallHeight));

	// The 4C door crosses the south/shared wall into the common hallway. It is
	// left of centre exactly as drawn, leaving the radio and fuse-box landmarks
	// in the longer right-hand part of the room.
	constexpr float DoorCentreX = -600.f;
	const float LeftSegment = DoorCentreX - DoorWidth * .5f + HalfW;
	const float RightSegment = HalfW - (DoorCentreX + DoorWidth * .5f);
	AddBox(TEXT("Wall_South_Left"),
		FVector(-HalfW + LeftSegment * .5f, -HalfD + WallThickness * .5f, WallZ),
		FVector(LeftSegment, WallThickness, WallHeight));
	AddBox(TEXT("Wall_South_Right"),
		FVector(DoorCentreX + DoorWidth * .5f + RightSegment * .5f,
			-HalfD + WallThickness * .5f, WallZ),
		FVector(RightSegment, WallThickness, WallHeight));

	// Readable concept-image landmarks, still only blockout geometry. The radio
	// sits on a low cabinet in the room's middle-left; the carried fuse reads as
	// part of a wall-mounted fuse box on the technical/right side near the door.
	AddBox(TEXT("RadioCabinet"), FVector(400.f, -300.f, FloorZ + 40.f),
		FVector(180.f, 100.f, 80.f));
	AddBox(TEXT("FuseBoxPanel"), FVector(900.f, -485.f, FloorZ + 100.f),
		FVector(140.f, 10.f, 180.f));
}

void ARewindFourCBlockout::BeginPlay()
{
	Super::BeginPlay();
	EnsureLoopStart();
}

FVector ARewindFourCBlockout::GetLoopStartLocation()
{
	return FVector(3600.f, 500.f,
		static_cast<float>(RewindChapter1Metrics::FourthFloorZ + 96.0));
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
