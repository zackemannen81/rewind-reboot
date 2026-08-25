#include "RewindLoopClock.h"
#include "RewindIds.h"
#include "RewindSaveGame.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FRewindLoopClockStepIndependent,
	"Rewind.Loop.Clock.StepIndependent",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FRewindLoopClockStepIndependent::RunTest(const FString& Parameters)
{
	FRewindLoopClock Fine;
	FRewindLoopClock Coarse;
	Fine.Start();
	Coarse.Start();

	for (int32 Index = 0; Index < 10; ++Index)
	{
		Fine.Advance(0.1);
	}
	Coarse.Advance(1.0);

	TestTrue(TEXT("elapsed matches across step sizes"),
		FMath::IsNearlyEqual(Fine.GetElapsedSeconds(), Coarse.GetElapsedSeconds()));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FRewindLoopClockIgnoresStopped,
	"Rewind.Loop.Clock.IgnoresStopped",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FRewindLoopClockIgnoresStopped::RunTest(const FString& Parameters)
{
	FRewindLoopClock Clock;
	Clock.Advance(1.0);
	TestTrue(TEXT("stopped clock stays zero"), FMath::IsNearlyZero(Clock.GetElapsedSeconds()));
	Clock.Start();
	Clock.Stop();
	Clock.Advance(5.0);
	TestTrue(TEXT("stopped after start does not advance"), FMath::IsNearlyZero(Clock.GetElapsedSeconds()));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FRewindIdsMatchDesign,
	"Rewind.Ids.MatchDesign",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FRewindIdsMatchDesign::RunTest(const FString& Parameters)
{
	TestEqual(TEXT("radio knowledge id"), RewindIds::KnowledgeRadioCode7312, FName(TEXT("radio_code_7312")));
	TestEqual(TEXT("gate anchor id"), RewindIds::AnchorCourtyardGateOpen, FName(TEXT("courtyard_gate_open")));
	TestEqual(TEXT("ground fuse gate checkpoint id"), RewindIds::CheckpointGroundFuseGate, FName(TEXT("GroundFuseGate")));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FRewindSaveGameStoresSessionBuckets,
	"Rewind.Save.StoresSessionBuckets",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FRewindSaveGameStoresSessionBuckets::RunTest(const FString& Parameters)
{
	URewindSaveGame* Save = NewObject<URewindSaveGame>();
	TestNotNull(TEXT("save object"), Save);
	Save->KnowledgeFacts.Add(RewindIds::KnowledgeRadioCode7312);
	Save->ActiveAnchor = RewindIds::AnchorCourtyardGateOpen;
	TestTrue(TEXT("knowledge fact stored"), Save->KnowledgeFacts.Contains(RewindIds::KnowledgeRadioCode7312));
	TestEqual(TEXT("anchor stored"), Save->ActiveAnchor, RewindIds::AnchorCourtyardGateOpen);
	return true;
}

#endif
