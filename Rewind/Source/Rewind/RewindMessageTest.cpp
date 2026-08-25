#include "RewindFirstRun.h"
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
		TEXT("A lock. Four digits."));
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
		TEXT("The sequence is yours."));
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
		RewindMessageIds::ApartmentWaking,
		RewindMessageIds::ApartmentReturned,
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
		RewindMessageIds::StairsRemain,
		RewindMessageIds::LoopBreak,
		RewindMessageIds::AnchorAccepted,
		RewindMessageIds::AnchorRefused,
		RewindMessageIds::RadioPresent,
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
		TEXT("1111 does not open it."));
	TestEqual(TEXT("radio channel is catalog data plus an argument"),
		FRewindMessageCatalog::Format(RewindMessageIds::RadioChannelVoice, {TEXT("3")}),
		TEXT("Radio: channel 3. A voice, under the static."));
	TestNull(TEXT("unknown id is not catalog data"),
		FRewindMessageCatalog::Find(FName(TEXT("Message.DoesNotExist"))));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FRewindMessageFirstRunGating,
	"Rewind.Message.FirstRun.Gating",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FRewindMessageFirstRunGating::RunTest(const FString& Parameters)
{
	(void)Parameters;

	// The same TSet<FName> URewindSessionSubsystem stores as knowledge.
	// A GameInstanceSubsystem cannot be NewObject'd outside a GameInstance.
	TSet<FName> Knowledge;
	FRewindMessageDirector Director;

	TestEqual(TEXT("seen-fact is knowledge, not a second store"),
		RewindFirstRun::SeenFact(RewindMessageIds::ApartmentWaking),
		FName(TEXT("seen.Apartment.Waking")));

	TestTrue(TEXT("first ShowOnce becomes current"),
		RewindFirstRun::ShowOnce(Knowledge, Director, RewindMessageIds::ApartmentWaking));
	TestEqual(TEXT("first-run copy is catalog data"), Director.GetActiveText(),
		TEXT("Apartment 4C. The room starts over. You do not."));
	TestTrue(TEXT("shown line is recorded as session knowledge"),
		Knowledge.Contains(RewindFirstRun::SeenFact(RewindMessageIds::ApartmentWaking)));
	TestEqual(TEXT("nothing queued after the first ShowOnce"), Director.GetQueuedCount(), 0);

	TestFalse(TEXT("the same line is not shown again in the same session"),
		RewindFirstRun::ShowOnce(Knowledge, Director, RewindMessageIds::ApartmentWaking));
	TestEqual(TEXT("repeat does not enqueue a second copy"), Director.GetQueuedCount(), 0);
	TestEqual(TEXT("current line is unchanged by a repeat"), Director.GetActiveId(),
		RewindMessageIds::ApartmentWaking);

	TestTrue(TEXT("a different first-run line still shows"),
		RewindFirstRun::ShowOnce(Knowledge, Director, RewindMessageIds::StairsRemain));
	TestEqual(TEXT("the new line waits behind the current one"), Director.GetQueuedCount(), 1);

	Director.Tick(FRewindMessageCatalog::DefaultDuration(RewindMessageIds::ApartmentWaking));
	TestEqual(TEXT("the later first-run line becomes current in enqueue order"),
		Director.GetActiveId(), RewindMessageIds::StairsRemain);

	TestFalse(TEXT("the later line also does not repeat"),
		RewindFirstRun::ShowOnce(Knowledge, Director, RewindMessageIds::StairsRemain));
	TestEqual(TEXT("knowledge count is the two shown lines"), Knowledge.Num(), 2);

	TestFalse(TEXT("unknown id is not shown and is not marked seen"),
		RewindFirstRun::ShowOnce(Knowledge, Director, FName(TEXT("Message.DoesNotExist"))));
	TestFalse(TEXT("unknown id left no knowledge fact"),
		Knowledge.Contains(RewindFirstRun::SeenFact(FName(TEXT("Message.DoesNotExist")))));

	TestTrue(TEXT("authored stair region is a stair region"),
		RewindFirstRun::IsStairRegion(FName(TEXT("Stairwell_Stairs"))));
	TestTrue(TEXT("proof-layout stair region is a stair region"),
		RewindFirstRun::IsStairRegion(FName(TEXT("Stairs4To3"))));
	TestFalse(TEXT("upper threshold is not a stair region"),
		RewindFirstRun::IsStairRegion(FName(TEXT("Stairwell_Upper"))));
	TestFalse(TEXT("entrance is not a stair region"),
		RewindFirstRun::IsStairRegion(FName(TEXT("Stairwell_Entrance"))));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	FRewindMessageFirstRunOmitsSolutions,
	"Rewind.Message.FirstRun.OmitsSolutions",
	EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool FRewindMessageFirstRunOmitsSolutions::RunTest(const FString& Parameters)
{
	(void)Parameters;

	TestEqual(TEXT("waking names the loop and that knowledge persists"),
		FRewindMessageCatalog::Format(RewindMessageIds::ApartmentWaking, {}),
		TEXT("Apartment 4C. The room starts over. You do not."));
	TestEqual(TEXT("return names the reset and that knowledge remains"),
		FRewindMessageCatalog::Format(RewindMessageIds::ApartmentReturned, {}),
		TEXT("The room is as it was. You remember."));
	TestEqual(TEXT("radio first-run copy names the object"),
		FRewindMessageCatalog::Format(RewindMessageIds::RadioPresent, {}),
		TEXT("A radio."));
	TestEqual(TEXT("radio voice copy names a voice and not the digits"),
		FRewindMessageCatalog::Format(RewindMessageIds::RadioChannelVoice, {TEXT("3")}),
		TEXT("Radio: channel 3. A voice, under the static."));
	TestEqual(TEXT("lock first-run copy names the lock and not the code"),
		FRewindMessageCatalog::Format(RewindMessageIds::LockPrompt, {}),
		TEXT("A lock. Four digits."));
	TestEqual(TEXT("fuse first-run copy names the fuse"),
		FRewindMessageCatalog::Format(RewindMessageIds::FuseCarried, {}),
		TEXT("A fuse."));
	TestEqual(TEXT("empty socket first-run copy names the socket"),
		FRewindMessageCatalog::Format(RewindMessageIds::SocketEmptyAtRest, {}),
		TEXT("An empty socket."));
	TestEqual(TEXT("lift refusal names the still cage and empty socket"),
		FRewindMessageCatalog::Format(RewindMessageIds::LiftNoPower, {}),
		TEXT("The cage is still. The socket is empty."));
	TestEqual(TEXT("stairs first-run copy names the stairs"),
		FRewindMessageCatalog::Format(RewindMessageIds::StairsRemain, {}),
		TEXT("The stairs remain."));
	TestEqual(TEXT("loop-break copy names the boundary"),
		FRewindMessageCatalog::Format(RewindMessageIds::LoopBreak, {}),
		TEXT("The world will not hold."));

	for (const TPair<FName, FRewindMessageDef>& Pair : FRewindMessageCatalog::GetAll())
	{
		TestFalse(*FString::Printf(TEXT("%s does not print 7312"), *Pair.Key.ToString()),
			Pair.Value.Text.Contains(TEXT("7312")));
		TestFalse(*FString::Printf(TEXT("%s does not tell the player to seat the fuse"),
			*Pair.Key.ToString()),
			Pair.Value.Text.Contains(TEXT("Seat the fuse")));
		const FString Lower = Pair.Value.Text.ToLower();
		TestFalse(*FString::Printf(TEXT("%s does not describe a ghost"), *Pair.Key.ToString()),
			Lower.Contains(TEXT("ghost")) || Lower.Contains(TEXT("echo")));
	}
	return true;
}

#endif
