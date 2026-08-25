#include "RewindCameraRegion.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "Engine/World.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FRewindCameraRegionVerticalTravelTest,
	"Rewind.Camera.Region.VerticalTravel",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FRewindCameraRegionVerticalTravelTest::RunTest(const FString& Parameters)
{
	(void)Parameters;

	UWorld* World = UWorld::CreateWorld(EWorldType::Game, false);
	TestNotNull(TEXT("transient test world"), World);
	if (!World)
	{
		return false;
	}

	ARewindCameraRegion* Region = World->SpawnActor<ARewindCameraRegion>();
	TestNotNull(TEXT("camera region"), Region);
	if (!Region)
	{
		World->DestroyWorld(false);
		return false;
	}

	Region->SetActorLocation(FVector::ZeroVector);
	Region->Configure(
		TEXT("VerticalTest"),
		FVector(100.f, 100.f, 600.f),
		ERewindTravelAxis::Z,
		FVector(1500.f, 0.f, 100.f),
		FRotator(0.f, 180.f, 0.f),
		100.0,
		50.0,
		false,
		37.5f);

	TestEqual(TEXT("vertical coordinate comes from Z"), Region->GetTravelCoord(FVector(1.f, 2.f, 345.f)), 345.0);
	TestEqual(TEXT("vertical lower bound includes authored padding"), Region->ClampTravel(-900.0), -500.0);
	TestEqual(TEXT("vertical upper bound includes authored padding"), Region->ClampTravel(900.0), 500.0);
	TestTrue(TEXT("minimum edge belongs to the region"), Region->Contains(FVector(0.f, 0.f, -600.f)));
	TestFalse(TEXT("maximum edge belongs to the adjacent region"), Region->Contains(FVector(0.f, 0.f, 600.f)));

	const FVector CameraLocation = Region->GetCameraLocation(400.0);
	TestEqual(TEXT("vertical travel applies the authored framing offset"), CameraLocation.Z, 500.0);
	TestEqual(TEXT("vertical travel preserves authored camera X"), CameraLocation.X, 1500.0);
	TestEqual(TEXT("vertical travel preserves authored camera Y"), CameraLocation.Y, 0.0);
	TestTrue(TEXT("region declares its horizontal FOV"), FMath::IsNearlyEqual(Region->GetFieldOfView(), 37.5f));

	ARewindCameraRegion* AdjacentRegion = World->SpawnActor<ARewindCameraRegion>();
	TestNotNull(TEXT("adjacent camera region"), AdjacentRegion);
	if (AdjacentRegion)
	{
		AdjacentRegion->SetActorLocation(FVector(0.f, 0.f, 1200.f));
		AdjacentRegion->Configure(
			TEXT("AdjacentVerticalTest"),
			FVector(100.f, 100.f, 600.f),
			ERewindTravelAxis::Z,
			FVector(1500.f, 0.f, 100.f),
			FRotator(0.f, 180.f, 0.f),
			100.0,
			50.0,
			false,
			42.f);

		auto ContainingRegionCount = [Region, AdjacentRegion](double Z)
		{
			const FVector Point(0.f, 0.f, Z);
			return static_cast<int32>(Region->Contains(Point))
				+ static_cast<int32>(AdjacentRegion->Contains(Point));
		};
		TestEqual(TEXT("point before threshold belongs to exactly one region"),
			ContainingRegionCount(599.999), 1);
		TestEqual(TEXT("shared threshold belongs to exactly one region"),
			ContainingRegionCount(600.0), 1);
		TestEqual(TEXT("point after threshold belongs to exactly one region"),
			ContainingRegionCount(600.001), 1);
	}

	World->DestroyWorld(false);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FRewindCameraRegionPlayerVolumeClampTest,
	"Rewind.Camera.Region.PlayerVolumeClamp",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

/**
 * The clamp must not evict the player from the region that clamped them.
 *
 * `ARewindCharacter::Tick` clamps the player to the volume of whichever region
 * contains them, every frame. While the clamp was inclusive of the half-open
 * positive faces, its own output was a coordinate `Contains` rejected, so a
 * player pressed against `+X` or `+Y` could end up inside no region at all.
 * Movement then had no frame to express input against and dropped it, which is
 * the defect REW-0015 was chartered for.
 */
bool FRewindCameraRegionPlayerVolumeClampTest::RunTest(const FString& Parameters)
{
	(void)Parameters;

	UWorld* World = UWorld::CreateWorld(EWorldType::Game, false);
	TestNotNull(TEXT("transient test world"), World);
	if (!World)
	{
		return false;
	}

	ARewindCameraRegion* Region = World->SpawnActor<ARewindCameraRegion>();
	TestNotNull(TEXT("camera region"), Region);
	if (!Region)
	{
		World->DestroyWorld(false);
		return false;
	}

	Region->SetActorLocation(FVector(100.f, -200.f, 50.f));
	Region->Configure(
		TEXT("ClampTest"),
		FVector(400.f, 300.f, 200.f),
		ERewindTravelAxis::Y,
		FVector(900.f, -250.f, 60.f),
		FRotator(0.f, 180.f, 0.f),
		120.0,
		80.0,
		false,
		45.f);

	// Z is deliberately not clamped, so every case here keeps Z inside the
	// band. Clamping Z would fight gravity, and the character only reaches this
	// code while a region already contains it.
	const double InsideZ = 50.0;

	const FVector BeyondPositiveX(10000.f, -200.f, InsideZ);
	const FVector BeyondPositiveY(100.f, 10000.f, InsideZ);
	const FVector BeyondBothNegative(-10000.f, -10000.f, InsideZ);
	const FVector BeyondBothPositive(10000.f, 10000.f, InsideZ);

	TestTrue(TEXT("a location clamped from beyond +X is still in the region"),
		Region->Contains(Region->ClampToPlayerVolume(BeyondPositiveX)));
	TestTrue(TEXT("a location clamped from beyond +Y is still in the region"),
		Region->Contains(Region->ClampToPlayerVolume(BeyondPositiveY)));
	TestTrue(TEXT("a location clamped from beyond both negative faces is still in the region"),
		Region->Contains(Region->ClampToPlayerVolume(BeyondBothNegative)));
	TestTrue(TEXT("a location clamped from beyond both positive faces is still in the region"),
		Region->Contains(Region->ClampToPlayerVolume(BeyondBothPositive)));

	// The inset is a hair inside the face, not a retreat from the authored
	// volume. A player pressed against the edge must still stand at the edge.
	const FVector ClampedPositive = Region->ClampToPlayerVolume(BeyondBothPositive);
	TestTrue(TEXT("the +X clamp stays within one centimetre of the authored face"),
		FMath::Abs(ClampedPositive.X - 500.0) <= 1.0);
	TestTrue(TEXT("the +Y clamp stays within one centimetre of the authored face"),
		FMath::Abs(ClampedPositive.Y - 100.0) <= 1.0);

	// The negative faces are closed, so they are clamped to exactly.
	const FVector ClampedNegative = Region->ClampToPlayerVolume(BeyondBothNegative);
	TestEqual(TEXT("the -X clamp lands on the authored face"), ClampedNegative.X, -300.0);
	TestEqual(TEXT("the -Y clamp lands on the authored face"), ClampedNegative.Y, -500.0);

	// A location already inside is returned unchanged, so the clamp cannot
	// nudge a player who never touched an edge.
	const FVector Interior(120.f, -180.f, InsideZ);
	TestTrue(TEXT("an interior location is returned unchanged"),
		Region->ClampToPlayerVolume(Interior).Equals(Interior, 0.0001));

	// Z is passed through, whatever it is. This is the one axis a clamped
	// location can still fail `Contains` on, and it is why movement holds the
	// last region rather than trusting the clamp alone.
	const FVector HighZ(120.f, -180.f, 9999.f);
	TestEqual(TEXT("Z is not clamped"), Region->ClampToPlayerVolume(HighZ).Z, 9999.0);

	World->DestroyWorld(false);
	return true;
}

#endif
