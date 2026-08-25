#include "RewindMessageCatalog.h"

#include "RewindMessageIds.h"

const TMap<FName, FRewindMessageDef>& FRewindMessageCatalog::GetAll()
{
	// Durations are catalog data. First-run lines are owned by
	// `docs/design/tutorial-and-first-run.md`. Operational durations keep
	// the previous overlay times so those lines do not silently change
	// how long they are readable.
	static const TMap<FName, FRewindMessageDef> Entries = {
		{RewindMessageIds::ApartmentWaking,
			{TEXT("Apartment 4C. The room starts over. You do not."), 6.f}},
		{RewindMessageIds::ApartmentReturned,
			{TEXT("The room is as it was. You remember."), 6.f}},
		{RewindMessageIds::CharacterControls,
			{TEXT("WASD move. E interact."), 4.f}},
		{RewindMessageIds::LockPrompt,
			{TEXT("A lock. Four digits."), 3.f}},
		{RewindMessageIds::LockCodeBuffer, {TEXT("Code: {0}"), 2.f}},
		{RewindMessageIds::LockAccepted, {TEXT("The lock opens."), 3.f}},
		{RewindMessageIds::LockRejected, {TEXT("{0} does not open it."), 3.f}},
		{RewindMessageIds::FuseCarried, {TEXT("A fuse."), 3.f}},
		{RewindMessageIds::FuseSeatedBuilding,
			{TEXT("The fuse is seated."), 3.f}},
		{RewindMessageIds::FuseSeatedCourtyard,
			{TEXT("The fuse is seated."), 3.f}},
		{RewindMessageIds::FuseTakenBuilding,
			{TEXT("The fuse is in hand."), 3.f}},
		{RewindMessageIds::FuseTakenCourtyard,
			{TEXT("The fuse is in hand."), 3.f}},
		{RewindMessageIds::SocketEmptyOther,
			{TEXT("Empty. The fuse is elsewhere."), 3.f}},
		{RewindMessageIds::SocketEmptyAtRest,
			{TEXT("An empty socket."), 3.f}},
		{RewindMessageIds::GeneratorDead,
			{TEXT("The generator is dead."), 3.f}},
		{RewindMessageIds::GeneratorOnline,
			{TEXT("The generator is running."), 3.f}},
		{RewindMessageIds::LiftNoPower,
			{TEXT("The cage is still. The socket is empty."), 4.f}},
		{RewindMessageIds::StairsRemain,
			{TEXT("The stairs remain."), 4.f}},
		{RewindMessageIds::LoopBreak,
			{TEXT("The world will not hold."), 4.f}},
		{RewindMessageIds::AnchorAccepted,
			{TEXT("This will hold."), 4.f}},
		{RewindMessageIds::AnchorRefused,
			{TEXT("Nothing to hold."), 4.f}},
		{RewindMessageIds::RadioPresent, {TEXT("A radio."), 3.f}},
		{RewindMessageIds::RadioChannelStatic, {TEXT("Radio: channel {0}. Static."), 3.f}},
		{RewindMessageIds::RadioChannelVoice,
			{TEXT("Radio: channel {0}. A voice, under the static."), 3.f}},
		{RewindMessageIds::RadioDigitSeven, {TEXT("Radio:  ...seven..."), 5.f}},
		{RewindMessageIds::RadioDigitThree, {TEXT("Radio:  ...three..."), 5.f}},
		{RewindMessageIds::RadioDigitOne, {TEXT("Radio:  ...one..."), 5.f}},
		{RewindMessageIds::RadioDigitTwo, {TEXT("Radio:  ...two..."), 5.f}},
		{RewindMessageIds::RadioCodeObtained, {TEXT("The sequence is yours."), 6.f}},
	};
	return Entries;
}

const FRewindMessageDef* FRewindMessageCatalog::Find(FName Id)
{
	return GetAll().Find(Id);
}

FString FRewindMessageCatalog::Format(FName Id, const TArray<FString>& Args)
{
	const FRewindMessageDef* Def = Find(Id);
	if (!Def)
	{
		return FString();
	}
	if (Args.Num() == 0)
	{
		return Def->Text;
	}

	FStringFormatOrderedArguments Ordered;
	Ordered.Reserve(Args.Num());
	for (const FString& Arg : Args)
	{
		Ordered.Add(Arg);
	}
	return FString::Format(*Def->Text, Ordered);
}

float FRewindMessageCatalog::DefaultDuration(FName Id)
{
	const FRewindMessageDef* Def = Find(Id);
	return Def ? Def->DefaultDurationSeconds : 0.f;
}
