#include "RewindRadio.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "Engine/World.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FRewindRadioDigitAudioSelectionTest,
	"Rewind.Radio.Audio.DigitSelection",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FRewindRadioDigitAudioSelectionTest::RunTest(const FString& Parameters)
{
	(void)Parameters;

	TestEqual(TEXT("phase 4 selects seven's SoundWave"),
		ARewindRadio::GetDigitSoundIndexForPhase(4.0), 0);
	TestEqual(TEXT("phase 9 selects three's SoundWave"),
		ARewindRadio::GetDigitSoundIndexForPhase(9.0), 1);
	TestEqual(TEXT("phase 14 selects one's SoundWave"),
		ARewindRadio::GetDigitSoundIndexForPhase(14.0), 2);
	TestEqual(TEXT("phase 19 selects two's SoundWave"),
		ARewindRadio::GetDigitSoundIndexForPhase(19.0), 3);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FRewindRadioAudioResetTest,
	"Rewind.Radio.Audio.ResetAtRewind",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FRewindRadioAudioResetTest::RunTest(const FString& Parameters)
{
	(void)Parameters;

	UWorld* World = UWorld::CreateWorld(EWorldType::Game, false);
	TestNotNull(TEXT("transient test world"), World);
	if (!World)
	{
		return false;
	}

	ARewindRadio* Radio = World->SpawnActor<ARewindRadio>();
	TestNotNull(TEXT("radio"), Radio);
	if (Radio)
	{
		Radio->TryInteract(nullptr); // channel 1 -> 2
		Radio->TryInteract(nullptr); // channel 2 -> 3 (station bed)
		TestEqual(TEXT("tuned radio selects the station bed"),
			Radio->GetCurrentBedSoundIndex(), 0);
		TestEqual(TEXT("station SoundWave is applied to the audio component"),
			Radio->GetAppliedBedSoundIndex(), 0);

		Radio->RestoreFromBaseline();
		TestEqual(TEXT("rewind restores authored channel"), Radio->GetChannel(), 1);
		TestEqual(TEXT("rewind replaces station bed with static"),
			Radio->GetCurrentBedSoundIndex(), 1);
		TestEqual(TEXT("rewind applies static to the audio component"),
			Radio->GetAppliedBedSoundIndex(), 1);
	}

	World->DestroyWorld(false);
	return Radio != nullptr;
}

#endif
