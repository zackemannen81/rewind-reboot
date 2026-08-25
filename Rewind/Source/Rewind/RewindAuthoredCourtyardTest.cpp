#include "RewindAuthoredCourtyard.h"
#include "RewindAuthoredCourtyardMetrics.h"
#include "RewindCameraRegion.h"
#include "RewindCausalCheckpoint.h"
#include "RewindIds.h"
#include "RewindProofLayout.h"
#include "RewindWorldStateSubsystem.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/WorldSettings.h"
#include "Misc/AutomationTest.h"

namespace Metrics = RewindAuthoredCourtyardMetrics;

namespace
{

	ARewindCameraRegion* SpawnGroundHall(UWorld* World)
	{
		ARewindCameraRegion* Hall = World->SpawnActor<ARewindCameraRegion>(
			FVector(-400.f, 600.f, 140.f), FRotator::ZeroRotator);
		if (Hall)
		{
			Hall->Configure(
				Metrics::GroundHallRegionName,
				FVector(200.f, 500.f, 140.f),
				ERewindTravelAxis::Y,
				FVector(-140.f, -700.f, 120.f),
				FRotator(-7.f, 75.f, 0.f),
				260.0,
				220.0,
				false,
				58.f);
		}
		return Hall;
	}

	int32 ContainingCount(const TArray<ARewindCameraRegion*>& Regions, const FVector& Point)
	{
		int32 Count = 0;
		for (ARewindCameraRegion* Region : Regions)
		{
			if (Region && Region->Contains(Point))
			{
				++Count;
			}
		}
		return Count;
	}
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FRewindAuthoredCourtyardSpatialConstantsTest,
	"Rewind.AuthoredCourtyard.SpatialConstants",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FRewindAuthoredCourtyardSpatialConstantsTest::RunTest(const FString& Parameters)
{
	(void)Parameters;

	TestTrue(TEXT("approach length is a walkable hall, not a door thickness"),
		Metrics::ApproachLengthCm >= 800.0);
	TestTrue(TEXT("courtyard length holds gate, generator and Transit in one room"),
		Metrics::CourtyardLengthCm >= 1600.0);
	TestEqual(TEXT("threshold local Y is the end of the approach"),
		Metrics::ThresholdLocalY(), Metrics::ApproachLengthCm);
	TestTrue(TEXT("checkpoint is thin on the walk axis"),
		Metrics::CheckpointExtentYCm < Metrics::CheckpointExtentXCm);
	TestTrue(TEXT("courtyard FOV is an explicit authored lens"),
		Metrics::CourtyardFieldOfView >= 5.f && Metrics::CourtyardFieldOfView <= 170.f);
	TestTrue(TEXT("proof-layout X=4110 is not this courtyard's seam"),
		!FMath::IsNearlyEqual(Metrics::ApproachLengthCm, 4110.0)
		&& !FMath::IsNearlyEqual(Metrics::CourtyardLengthCm, 4110.0));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FRewindAuthoredCourtyardRegionAbutmentTest,
	"Rewind.AuthoredCourtyard.RegionAbutment",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FRewindAuthoredCourtyardRegionAbutmentTest::RunTest(const FString& Parameters)
{
	(void)Parameters;

	UWorld* World = UWorld::CreateWorld(EWorldType::Game, false);
	TestNotNull(TEXT("transient test world"), World);
	if (!World)
	{
		return false;
	}

	ARewindCameraRegion* GroundHall = SpawnGroundHall(World);
	TestNotNull(TEXT("GroundHall_Region"), GroundHall);
	ARewindAuthoredCourtyard* Courtyard = World->SpawnActor<ARewindAuthoredCourtyard>();
	TestNotNull(TEXT("authored courtyard"), Courtyard);
	if (!GroundHall || !Courtyard)
	{
		World->DestroyWorld(false);
		return false;
	}

	Courtyard->EnsureContents();

	ARewindCameraRegion* Approach = Courtyard->GetApproachRegion();
	ARewindCameraRegion* Plaza = Courtyard->GetCourtyardRegion();
	TestNotNull(TEXT("approach region"), Approach);
	TestNotNull(TEXT("courtyard region"), Plaza);
	if (!Approach || !Plaza)
	{
		World->DestroyWorld(false);
		return false;
	}

	TestEqual(TEXT("approach is named"), Approach->GetRegionName(), Metrics::ApproachRegionName);
	TestEqual(TEXT("courtyard is named"), Plaza->GetRegionName(), Metrics::CourtyardRegionName);
	TestEqual(TEXT("approach travels Y"), static_cast<int32>(Approach->GetTravelAxis()),
		static_cast<int32>(ERewindTravelAxis::Y));
	TestEqual(TEXT("courtyard travels Y"), static_cast<int32>(Plaza->GetTravelAxis()),
		static_cast<int32>(ERewindTravelAxis::Y));
	TestTrue(TEXT("courtyard declares its FOV"),
		FMath::IsNearlyEqual(Plaza->GetFieldOfView(), Metrics::CourtyardFieldOfView));
	TestTrue(TEXT("courtyard cuts at the doorway"), Plaza->CutsOnEntry());

	TestTrue(TEXT("approach min Y equals GroundHall max Y"),
		FMath::IsNearlyEqual(Approach->GetVolumeMin().Y, GroundHall->GetVolumeMax().Y, 0.01));
	TestTrue(TEXT("courtyard min Y equals approach max Y"),
		FMath::IsNearlyEqual(Plaza->GetVolumeMin().Y, Approach->GetVolumeMax().Y, 0.01));

	const double HallX = GroundHall->GetActorLocation().X;
	const double HallZ = GroundHall->GetActorLocation().Z;
	const TArray<ARewindCameraRegion*> Regions = {GroundHall, Approach, Plaza};

	const double HallMaxY = GroundHall->GetVolumeMax().Y;
	const double ApproachMaxY = Approach->GetVolumeMax().Y;
	TestEqual(TEXT("just inside GroundHall belongs to exactly one region"),
		ContainingCount(Regions, FVector(HallX, HallMaxY - 0.001, HallZ)), 1);
	TestEqual(TEXT("GroundHall +Y seam belongs to exactly one region"),
		ContainingCount(Regions, FVector(HallX, HallMaxY, HallZ)), 1);
	TestEqual(TEXT("just inside the approach belongs to exactly one region"),
		ContainingCount(Regions, FVector(HallX, HallMaxY + 0.001, HallZ)), 1);
	TestEqual(TEXT("approach-to-courtyard seam belongs to exactly one region"),
		ContainingCount(Regions, FVector(HallX, ApproachMaxY, HallZ)), 1);
	TestEqual(TEXT("just inside the courtyard belongs to exactly one region"),
		ContainingCount(Regions, FVector(HallX, ApproachMaxY + 0.001, HallZ)), 1);

	TestTrue(TEXT("GroundHall owns the coordinate before its +Y face"),
		GroundHall->Contains(FVector(HallX, HallMaxY - 0.001, HallZ)));
	TestTrue(TEXT("approach owns GroundHall's +Y face"),
		Approach->Contains(FVector(HallX, HallMaxY, HallZ)));
	TestTrue(TEXT("courtyard owns the approach +Y face"),
		Plaza->Contains(FVector(HallX, ApproachMaxY, HallZ)));

	ARewindCausalCheckpoint* Checkpoint = Courtyard->GetCheckpoint();
	TestNotNull(TEXT("GroundFuseGate"), Checkpoint);
	if (Checkpoint)
	{
		TestEqual(TEXT("checkpoint id is GroundFuseGate"),
			Checkpoint->GetCheckpointId(), RewindIds::CheckpointGroundFuseGate);
		const FVector CheckpointLocation = Checkpoint->GetActorLocation();
		TestTrue(TEXT("checkpoint sits on the authored threshold, not proof X=4110"),
			!FMath::IsNearlyEqual(CheckpointLocation.X, 4110.0, 1.0));
		TestTrue(TEXT("checkpoint Y matches the hall-to-courtyard seam"),
			FMath::IsNearlyEqual(CheckpointLocation.Y, ApproachMaxY, 1.0));
		TestFalse(TEXT("without ground power or the Anchor, the predicate fails"),
			Checkpoint->EvaluatePredicate());
	}

	World->DestroyWorld(false);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FRewindAuthoredCourtyardSkipProofLayoutTest,
	"Rewind.AuthoredCourtyard.SkipProofLayoutSpawnsCheckpoint",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FRewindAuthoredCourtyardSkipProofLayoutTest::RunTest(const FString& Parameters)
{
	(void)Parameters;

	UWorld* World = UWorld::CreateWorld(EWorldType::Game, false);
	TestNotNull(TEXT("transient test world"), World);
	if (!World)
	{
		return false;
	}

	if (AWorldSettings* Settings = World->GetWorldSettings())
	{
		Settings->Tags.AddUnique(FName(TEXT("Rewind.SkipProofLayout")));
	}

	ARewindCameraRegion* GroundHall = SpawnGroundHall(World);
	TestNotNull(TEXT("GroundHall_Region"), GroundHall);

	URewindWorldStateSubsystem* WorldState = World->GetSubsystem<URewindWorldStateSubsystem>();
	TestNotNull(TEXT("world state"), WorldState);
	if (!WorldState || !GroundHall)
	{
		World->DestroyWorld(false);
		return false;
	}

	WorldState->EnsureAuthoredSpace();

	ARewindCausalCheckpoint* Checkpoint = nullptr;
	int32 CheckpointCount = 0;
	for (TActorIterator<ARewindCausalCheckpoint> It(World); It; ++It)
	{
		++CheckpointCount;
		if (It->GetCheckpointId() == RewindIds::CheckpointGroundFuseGate)
		{
			Checkpoint = *It;
		}
	}
	TestEqual(TEXT("exactly one GroundFuseGate on a SkipProofLayout map"), CheckpointCount, 1);
	TestNotNull(TEXT("spawned GroundFuseGate"), Checkpoint);
	if (Checkpoint)
	{
		TestTrue(TEXT("SkipProofLayout does not force the proof-layout X=4110 seam"),
			!FMath::IsNearlyEqual(Checkpoint->GetActorLocation().X, 4110.0, 1.0));
		TestTrue(TEXT("checkpoint is at GroundHall's +Y plus the approach length"),
			FMath::IsNearlyEqual(
				Checkpoint->GetActorLocation().Y,
				GroundHall->GetVolumeMax().Y + Metrics::ApproachLengthCm,
				1.0));
	}

	ARewindProofLayout* Proof = nullptr;
	for (TActorIterator<ARewindProofLayout> It(World); It; ++It)
	{
		Proof = *It;
		break;
	}
	TestTrue(TEXT("SkipProofLayout does not spawn the procedural proof"), Proof == nullptr);

	World->DestroyWorld(false);
	return true;
}

#endif
