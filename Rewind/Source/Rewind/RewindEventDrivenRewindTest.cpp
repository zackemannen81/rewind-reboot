#include "RewindLatchedRewind.h"
#include "RewindCausalCheckpoint.h"
#include "RewindDeveloperSettings.h"
#include "RewindIds.h"
#include "RewindLoopSubsystem.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "Engine/World.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FRewindLoopPreludeBoundsTest,
	"Rewind.Loop.Prelude.Bounds",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FRewindLoopPreludeBoundsTest::RunTest(const FString& Parameters)
{
	(void)Parameters;

	TestTrue(TEXT("default prelude sits inside the accepted band"),
		FRewindLatchedRewind::DefaultPreludeSeconds >= FRewindLatchedRewind::MinPreludeSeconds
		&& FRewindLatchedRewind::DefaultPreludeSeconds <= FRewindLatchedRewind::MaxPreludeSeconds);

	TestEqual(TEXT("a short request clamps to 1.0s"),
		FRewindLatchedRewind::ClampPreludeSeconds(0.1), 1.0);
	TestEqual(TEXT("a long request clamps to 3.0s"),
		FRewindLatchedRewind::ClampPreludeSeconds(10.0), 3.0);
	TestEqual(TEXT("an in-band request is unchanged"),
		FRewindLatchedRewind::ClampPreludeSeconds(2.0), 2.0);

	const double SharedElapsed = 17.25;
	FRewindLatchedRewind RunA;
	FRewindLatchedRewind RunB;
	TestTrue(TEXT("run A latches"),
		RunA.TryLatch(RewindIds::CheckpointGroundFuseGate, SharedElapsed, 2.0));
	TestTrue(TEXT("run B latches"),
		RunB.TryLatch(RewindIds::CheckpointGroundFuseGate, SharedElapsed, 2.0));

	TestEqual(TEXT("identical runs share prelude duration"),
		RunA.PreludeDurationSeconds, RunB.PreludeDurationSeconds);
	TestEqual(TEXT("identical runs share apply elapsed"),
		RunA.GetApplyElapsedSeconds(), RunB.GetApplyElapsedSeconds());
	TestTrue(TEXT("prelude duration is at least 1.0s"),
		RunA.PreludeDurationSeconds >= FRewindLatchedRewind::MinPreludeSeconds);
	TestTrue(TEXT("prelude duration is at most 3.0s"),
		RunA.PreludeDurationSeconds <= FRewindLatchedRewind::MaxPreludeSeconds);

	TestFalse(TEXT("apply has not started before the prelude elapses"),
		RunA.ShouldApply(SharedElapsed + RunA.PreludeDurationSeconds - 0.001));
	TestTrue(TEXT("apply starts once the loop clock reaches request plus prelude"),
		RunA.ShouldApply(SharedElapsed + RunA.PreludeDurationSeconds));
	TestTrue(TEXT("identical runs apply at the same t"),
		RunA.ShouldApply(SharedElapsed + RunA.PreludeDurationSeconds)
			== RunB.ShouldApply(SharedElapsed + RunB.PreludeDurationSeconds));

	const URewindDeveloperSettings* Settings = GetDefault<URewindDeveloperSettings>();
	TestNotNull(TEXT("developer settings"), Settings);
	if (Settings)
	{
		TestFalse(TEXT("Chapter 1 does not declare a whole-space deadline"),
			Settings->bUseWholeSpaceDeadline);
		TestTrue(TEXT("configured prelude is inside [1, 3]"),
			Settings->RewindPreludeSeconds >= 1.f && Settings->RewindPreludeSeconds <= 3.f);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FRewindLoopPreludeLatchTest,
	"Rewind.Loop.Prelude.Latch",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FRewindLoopPreludeLatchTest::RunTest(const FString& Parameters)
{
	(void)Parameters;

	FRewindLatchedRewind Latch;
	TestTrue(TEXT("first crossing latches"),
		Latch.TryLatch(RewindIds::CheckpointGroundFuseGate, 8.0, 2.0));
	TestFalse(TEXT("a second latch request is refused"),
		Latch.TryLatch(RewindIds::CheckpointGroundFuseGate, 8.4, 2.0));
	TestTrue(TEXT("the original request remains latched"), Latch.IsLatched());
	TestEqual(TEXT("stepping back does not move the request time"),
		Latch.RequestElapsedSeconds, 8.0);
	TestEqual(TEXT("stepping back does not change the checkpoint"),
		Latch.Checkpoint, RewindIds::CheckpointGroundFuseGate);

	UWorld* World = UWorld::CreateWorld(EWorldType::Game, false);
	TestNotNull(TEXT("transient test world"), World);
	if (!World)
	{
		return false;
	}

	URewindLoopSubsystem* Loop = World->GetSubsystem<URewindLoopSubsystem>();
	TestNotNull(TEXT("loop subsystem"), Loop);
	ARewindCausalCheckpoint* Checkpoint = World->SpawnActor<ARewindCausalCheckpoint>();
	TestNotNull(TEXT("checkpoint actor"), Checkpoint);
	if (!Loop || !Checkpoint)
	{
		World->DestroyWorld(false);
		return false;
	}

	Checkpoint->Configure(
		RewindIds::CheckpointGroundFuseGate,
		ERewindCheckpointPredicate::GroundFuseGate,
		FVector(80.f, 300.f, 180.f));
	TestFalse(TEXT("GroundFuseGate fails without ground power or the gate Anchor"),
		Checkpoint->EvaluatePredicate());

	Checkpoint->NotifyCrossing();
	TestTrue(TEXT("a failed crossing latches rewind"), Loop->IsRewindLatched());
	TestEqual(TEXT("latched reason is causal contract"),
		static_cast<int32>(Loop->GetLatchedReason()),
		static_cast<int32>(ERewindLoopEndReason::CausalContract));
	TestEqual(TEXT("latched checkpoint is GroundFuseGate"),
		Loop->GetLatchedCheckpoint(), RewindIds::CheckpointGroundFuseGate);

	const double RequestElapsed = Loop->GetLatchedRequestElapsed();
	const double Prelude = Loop->GetLatchedPreludeDuration();
	TestTrue(TEXT("latched prelude is inside [1, 3]"), Prelude >= 1.0 && Prelude <= 3.0);

	Checkpoint->NotifyExited();
	TestTrue(TEXT("leaving the volume does not cancel the latch"), Loop->IsRewindLatched());
	TestEqual(TEXT("leaving the volume does not change the request time"),
		Loop->GetLatchedRequestElapsed(), RequestElapsed);

	Loop->Tick(0.5f);
	TestTrue(TEXT("the latch holds while the prelude runs"), Loop->IsRewindLatched());
	TestTrue(TEXT("elapsed loop time advanced during the prelude"),
		Loop->GetElapsedLoopTime() >= RequestElapsed + 0.5 - 0.0001);

	World->DestroyWorld(false);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FRewindLoopAnchorCommitBoundaryTest,
	"Rewind.Loop.AnchorCommit.Boundary",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FRewindLoopAnchorCommitBoundaryTest::RunTest(const FString& Parameters)
{
	(void)Parameters;

	// The board ends the loop only when TryCommit succeeds and a pending write
	// remains. That is first-time. Rejected is !accepted. Redundant is accepted
	// with pending still false because the identifier is already active.
	TestFalse(TEXT("a rejected commit does not end the loop"),
		RewindAnchorCommit::EndsLoop(false, false));
	TestTrue(TEXT("a first-time pending commit ends the loop"),
		RewindAnchorCommit::EndsLoop(true, true));
	TestFalse(TEXT("a redundant already-active commit does not end the loop"),
		RewindAnchorCommit::EndsLoop(true, false));

	UWorld* World = UWorld::CreateWorld(EWorldType::Game, false);
	TestNotNull(TEXT("transient test world"), World);
	if (!World)
	{
		return false;
	}

	URewindLoopSubsystem* Loop = World->GetSubsystem<URewindLoopSubsystem>();
	TestNotNull(TEXT("loop subsystem"), Loop);
	if (!Loop)
	{
		World->DestroyWorld(false);
		return false;
	}

	TestFalse(TEXT("loop is not latched before a commit"), Loop->IsRewindLatched());
	Loop->NotifyAnchorCommitted();
	TestTrue(TEXT("a successful first-time commit latches rewind"), Loop->IsRewindLatched());
	TestEqual(TEXT("latched reason is Anchor commit"),
		static_cast<int32>(Loop->GetLatchedReason()),
		static_cast<int32>(ERewindLoopEndReason::AnchorCommit));
	TestTrue(TEXT("commit prelude is inside [1, 3]"),
		Loop->GetLatchedPreludeDuration() >= 1.0
			&& Loop->GetLatchedPreludeDuration() <= 3.0);

	const double FirstRequest = Loop->GetLatchedRequestElapsed();
	Loop->NotifyAnchorCommitted();
	TestEqual(TEXT("a second commit notify does not replace the latch"),
		Loop->GetLatchedRequestElapsed(), FirstRequest);

	World->DestroyWorld(false);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FRewindLoopBreakSignatureReadsLoopTimeTest,
	"Rewind.Loop.BreakSignature.ReadsLoopTime",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FRewindLoopBreakSignatureReadsLoopTimeTest::RunTest(const FString& Parameters)
{
	(void)Parameters;

	TestEqual(TEXT("intensity is zero at the request instant"),
		FRewindLoopBreakMath::Intensity(10.0, 10.0, 2.0), 0.0);
	TestTrue(TEXT("intensity rises with elapsed loop time"),
		FMath::IsNearlyEqual(FRewindLoopBreakMath::Intensity(11.0, 10.0, 2.0), 0.5));
	TestEqual(TEXT("intensity saturates at apply"),
		FRewindLoopBreakMath::Intensity(12.0, 10.0, 2.0), 1.0);
	TestEqual(TEXT("identical elapsed times produce identical intensity"),
		FRewindLoopBreakMath::Intensity(11.25, 10.0, 2.0),
		FRewindLoopBreakMath::Intensity(11.25, 10.0, 2.0));
	TestTrue(TEXT("intensity is not a stored frame-delta sum"),
		!FMath::IsNearlyEqual(FRewindLoopBreakMath::Intensity(11.0, 10.0, 2.0), 1.0 / 60.0));

	UWorld* World = UWorld::CreateWorld(EWorldType::Game, false);
	TestNotNull(TEXT("transient test world"), World);
	if (!World)
	{
		return false;
	}

	URewindLoopSubsystem* Loop = World->GetSubsystem<URewindLoopSubsystem>();
	TestNotNull(TEXT("loop subsystem"), Loop);
	if (!Loop)
	{
		World->DestroyWorld(false);
		return false;
	}

	TestEqual(TEXT("intensity is zero while no rewind is latched"),
		Loop->GetLoopBreakIntensity(), 0.0);

	Loop->NotifyCausalContractFailed(RewindIds::CheckpointGroundFuseGate);
	const double Request = Loop->GetLatchedRequestElapsed();
	const double Prelude = Loop->GetLatchedPreludeDuration();
	Loop->Tick(static_cast<float>(Prelude * 0.5));
	const double Expected = FRewindLoopBreakMath::Intensity(
		Loop->GetElapsedLoopTime(), Request, Prelude);
	TestTrue(TEXT("subsystem intensity matches loop-clock math"),
		FMath::IsNearlyEqual(Loop->GetLoopBreakIntensity(), Expected, 1.e-4));
	TestTrue(TEXT("subsystem intensity is rising during the prelude"),
		Loop->GetLoopBreakIntensity() > 0.0 && Loop->GetLoopBreakIntensity() < 1.0);

	World->DestroyWorld(false);
	return true;
}

#endif
