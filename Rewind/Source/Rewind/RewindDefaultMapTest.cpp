#include "Misc/AutomationTest.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/ConfigCacheIni.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FRewindDefaultAuthoredMapTest,
	"Rewind.Project.DefaultAuthoredMap",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FRewindDefaultAuthoredMapTest::RunTest(const FString& Parameters)
{
	(void)Parameters;

	constexpr const TCHAR* Section = TEXT("/Script/EngineSettings.GameMapsSettings");
	constexpr const TCHAR* AuthoredMap = TEXT("/Game/Maps/FiveLoops_Stairwell_Blockout");

	FString GameDefaultMap;
	FString EditorStartupMap;
	TestTrue(TEXT("game default map is configured"),
		GConfig->GetString(Section, TEXT("GameDefaultMap"), GameDefaultMap, GEngineIni));
	TestTrue(TEXT("editor startup map is configured"),
		GConfig->GetString(Section, TEXT("EditorStartupMap"), EditorStartupMap, GEngineIni));
	TestEqual(TEXT("game starts in the authored building slice"), GameDefaultMap, FString(AuthoredMap));
	TestEqual(TEXT("editor opens the authored building slice"), EditorStartupMap, FString(AuthoredMap));

	return true;
}

#endif
