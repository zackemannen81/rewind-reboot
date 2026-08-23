#if WITH_DEV_AUTOMATION_TESTS

#include "RewindChapter1Metrics.h"
#include "RewindLift.h"
#include "RewindStairwell.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FRewindVerticalRoutesTest,
	"Rewind.Chapter1.VerticalRoutes",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FRewindVerticalRoutesTest::RunTest(const FString& Parameters)
{
	(void)Parameters;
	TestEqual(TEXT("4C to entrance has three floor transitions"),
		ARewindStairwell::GetFlightCount(), RewindChapter1Metrics::FloorTransitions);
	TestEqual(TEXT("stair geometry and metric share one run length"),
		ARewindStairwell::GetAuthoredRunLengthCm(),
		RewindChapter1Metrics::StairRunLengthCm());
	TestEqual(TEXT("lift actor and metric share one duration"),
		ARewindLift::GetAuthoredTravelSeconds(),
		RewindChapter1Metrics::LiftTravelSeconds);
	TestTrue(TEXT("stairs cost at least one 30-second turnstile period more than lift"),
		RewindChapter1Metrics::StairMinimumSeconds()
			- RewindChapter1Metrics::LiftTravelSeconds
			>= RewindChapter1Metrics::TurnstilePeriodSeconds);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FRewindChapter1RouteBudgetTest,
	"Rewind.Chapter1.RouteBudget",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FRewindChapter1RouteBudgetTest::RunTest(const FString& Parameters)
{
	(void)Parameters;
	TestTrue(TEXT("fuse-to-courtyard Loop B route exceeds 240 seconds"),
		RewindChapter1Metrics::FuseToCourtyardRouteSeconds()
			> RewindChapter1Metrics::LoopDurationSeconds);
	TestTrue(TEXT("anchored lift Loop C route fits inside 240 seconds"),
		RewindChapter1Metrics::LearnedRouteSeconds()
			< RewindChapter1Metrics::LoopDurationSeconds);
	TestTrue(TEXT("naive minus learned exceeds one turnstile period"),
		RewindChapter1Metrics::FuseToCourtyardRouteSeconds()
			- RewindChapter1Metrics::LearnedRouteSeconds()
			>= RewindChapter1Metrics::TurnstilePeriodSeconds);
	return true;
}

#endif
