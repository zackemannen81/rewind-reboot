#include "RewindMessageCatalog.h"
#include "RewindMessageIds.h"
#include "RewindMessageQueue.h"

#if WITH_DEV_AUTOMATION_TESTS

#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FRewindMessageQueueOrdering,
	"Rewind.Message.Queue.Ordering",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FRewindMessageQueueOrdering::RunTest(const FString& Parameters)
{
	(void)Parameters;

	FRewindMessageDirector Director;
	TestTrue(TEXT("sequence enqueued"), Director.ShowSequence({
		RewindMessageIds::LockPrompt,
		RewindMessageIds::FuseCarried,
		RewindMessageIds::GeneratorOnline,
	}));
	TestEqual(TEXT("three lines waiting in order"), Director.GetLineCount(), 3);
	TestEqual(TEXT("two queued behind the current line"), Director.GetQueuedCount(), 2);
	TestEqual(TEXT("first line is current"), Director.GetActiveId(), RewindMessageIds::LockPrompt);

	Director.Tick(FRewindMessageCatalog::DefaultDuration(RewindMessageIds::LockPrompt));
	TestEqual(TEXT("second line follows the first"), Director.GetActiveId(), RewindMessageIds::FuseCarried);

	Director.Tick(FRewindMessageCatalog::DefaultDuration(RewindMessageIds::FuseCarried));
	TestEqual(TEXT("third line follows the second"), Director.GetActiveId(), RewindMessageIds::GeneratorOnline);

	Director.Show(RewindMessageIds::LiftNoPower);
	TestEqual(TEXT("a later Show waits behind the current line"), Director.GetActiveId(),
		RewindMessageIds::GeneratorOnline);
	TestEqual(TEXT("one line queued after the extra Show"), Director.GetQueuedCount(), 1);

	Director.Tick(FRewindMessageCatalog::DefaultDuration(RewindMessageIds::GeneratorOnline));
	TestEqual(TEXT("queued Show becomes current in enqueue order"), Director.GetActiveId(),
		RewindMessageIds::LiftNoPower);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FRewindMessageQueueExpiry,
	"Rewind.Message.Queue.Expiry",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FRewindMessageQueueExpiry::RunTest(const FString& Parameters)
{
	(void)Parameters;

	FRewindMessageDirector Director;
	TestTrue(TEXT("timed line shown"),
		Director.ShowTimed(RewindMessageIds::LockPrompt, 0.5f));
	Director.Tick(0.49f);
	TestTrue(TEXT("line still current before expiry"), Director.HasActive());
	TestEqual(TEXT("id unchanged before expiry"), Director.GetActiveId(), RewindMessageIds::LockPrompt);
	TestTrue(TEXT("remaining time is still positive"), Director.GetActiveRemaining() > 0.f);

	Director.Tick(0.02f);
	TestFalse(TEXT("line is gone once its duration elapses"), Director.HasActive());
	TestEqual(TEXT("no id after expiry"), Director.GetActiveId(), NAME_None);
	TestTrue(TEXT("no text after expiry"), Director.GetActiveText().IsEmpty());

	TestTrue(TEXT("first of two timed lines"),
		Director.ShowTimed(RewindMessageIds::FuseCarried, 0.5f));
	TestTrue(TEXT("second of two timed lines"),
		Director.ShowTimed(RewindMessageIds::GeneratorOnline, 1.0f));
	Director.Tick(0.75f);
	TestEqual(TEXT("leftover tick time carries into the next line"), Director.GetActiveId(),
		RewindMessageIds::GeneratorOnline);
	TestTrue(TEXT("second line consumed the leftover"),
		FMath::IsNearlyEqual(Director.GetActiveRemaining(), 0.75f, 0.0001f));

	Director.Tick(0.75f);
	TestFalse(TEXT("second line expires after its remaining time"), Director.HasActive());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FRewindMessageQueueQueueing,
	"Rewind.Message.Queue.Queueing",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FRewindMessageQueueQueueing::RunTest(const FString& Parameters)
{
	(void)Parameters;

	FRewindMessageDirector Director;
	TestFalse(TEXT("unknown id does not enqueue"), Director.Show(FName(TEXT("Message.DoesNotExist"))));
	TestFalse(TEXT("empty director has no active line"), Director.HasActive());

	TestTrue(TEXT("first Show becomes current"), Director.Show(RewindMessageIds::LockPrompt));
	TestEqual(TEXT("catalog text is used"), Director.GetActiveText(),
		TEXT("Lock: enter 4 digits, or learn 7312 from the radio"));
	TestEqual(TEXT("nothing queued yet"), Director.GetQueuedCount(), 0);

	TestTrue(TEXT("second Show queues"), Director.Show(RewindMessageIds::FuseCarried));
	TestEqual(TEXT("current line is unchanged by a later Show"), Director.GetActiveId(),
		RewindMessageIds::LockPrompt);
	TestEqual(TEXT("one line queued"), Director.GetQueuedCount(), 1);

	TestTrue(TEXT("rejected-code arg is formatted from catalog data"),
		Director.Show(RewindMessageIds::LockRejected, TEXT("1111")));
	TestEqual(TEXT("two lines queued behind the prompt"), Director.GetQueuedCount(), 2);

	Director.Clear();
	TestFalse(TEXT("Clear removes current and queued lines"), Director.HasActive());
	TestEqual(TEXT("Clear leaves an empty queue"), Director.GetLineCount(), 0);

	TestTrue(TEXT("ShowTimed after Clear is current"),
		Director.ShowTimed(RewindMessageIds::RadioCodeObtained, 1.5f));
	TestEqual(TEXT("ShowTimed uses catalog copy with the given duration"), Director.GetActiveText(),
		TEXT("Radio: 7312"));
	TestTrue(TEXT("ShowTimed duration is the override, not the catalog default"),
		FMath::IsNearlyEqual(Director.GetActiveRemaining(), 1.5f, 0.0001f));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FRewindMessageCatalogResolvesSliceCopy,
	"Rewind.Message.Catalog.ResolvesSliceCopy",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FRewindMessageCatalogResolvesSliceCopy::RunTest(const FString& Parameters)
{
	(void)Parameters;

	const FName SliceIds[] = {
		RewindMessageIds::CharacterControls,
		RewindMessageIds::LockPrompt,
		RewindMessageIds::LockCodeBuffer,
		RewindMessageIds::LockAccepted,
		RewindMessageIds::LockRejected,
		RewindMessageIds::FuseCarried,
		RewindMessageIds::FuseSeatedBuilding,
		RewindMessageIds::FuseSeatedCourtyard,
		RewindMessageIds::FuseTakenBuilding,
		RewindMessageIds::FuseTakenCourtyard,
		RewindMessageIds::SocketEmptyOther,
		RewindMessageIds::SocketEmptyAtRest,
		RewindMessageIds::GeneratorDead,
		RewindMessageIds::GeneratorOnline,
		RewindMessageIds::LiftNoPower,
		RewindMessageIds::AnchorAccepted,
		RewindMessageIds::AnchorRefused,
		RewindMessageIds::RadioChannelStatic,
		RewindMessageIds::RadioChannelVoice,
		RewindMessageIds::RadioDigitSeven,
		RewindMessageIds::RadioDigitThree,
		RewindMessageIds::RadioDigitOne,
		RewindMessageIds::RadioDigitTwo,
		RewindMessageIds::RadioCodeObtained,
	};

	TestEqual(TEXT("catalog size matches the authored slice"),
		FRewindMessageCatalog::GetAll().Num(), static_cast<int32>(UE_ARRAY_COUNT(SliceIds)));

	for (const FName Id : SliceIds)
	{
		const FRewindMessageDef* Def = FRewindMessageCatalog::Find(Id);
		TestNotNull(*FString::Printf(TEXT("catalog contains %s"), *Id.ToString()), Def);
		if (Def)
		{
			TestFalse(*FString::Printf(TEXT("%s has copy"), *Id.ToString()), Def->Text.IsEmpty());
			TestTrue(*FString::Printf(TEXT("%s has a duration"), *Id.ToString()),
				Def->DefaultDurationSeconds > 0.f);
		}
	}

	TestEqual(TEXT("typed code is catalog data plus an argument"),
		FRewindMessageCatalog::Format(RewindMessageIds::LockCodeBuffer, {TEXT("73")}),
		TEXT("Code: 73"));
	TestEqual(TEXT("rejected code is catalog data plus an argument"),
		FRewindMessageCatalog::Format(RewindMessageIds::LockRejected, {TEXT("1111")}),
		TEXT("Lock: 1111 rejected"));
	TestEqual(TEXT("radio channel is catalog data plus an argument"),
		FRewindMessageCatalog::Format(RewindMessageIds::RadioChannelVoice, {TEXT("3")}),
		TEXT("Radio: channel 3  ...a voice, under the static"));
	TestNull(TEXT("unknown id is not catalog data"),
		FRewindMessageCatalog::Find(FName(TEXT("Message.DoesNotExist"))));
	return true;
}

#endif
