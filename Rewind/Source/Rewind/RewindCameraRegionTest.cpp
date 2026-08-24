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

#endif
