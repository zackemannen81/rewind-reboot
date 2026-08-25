#include "RewindMessageCatalog.h"

#include "RewindMessageIds.h"

const TMap<FName, FRewindMessageDef>& FRewindMessageCatalog::GetAll()
{
	// Durations restate the previous overlay times so the slice does not
	// silently change how long a line is readable.
	static const TMap<FName, FRewindMessageDef> Entries = {
		{RewindMessageIds::CharacterControls,
			{TEXT("RE:WIND 4C  |  WASD move  E interact  digits at lock"), 8.f}},
		{RewindMessageIds::LockPrompt,
			{TEXT("Lock: enter 4 digits, or learn 7312 from the radio"), 3.f}},
		{RewindMessageIds::LockCodeBuffer, {TEXT("Code: {0}"), 2.f}},
		{RewindMessageIds::LockAccepted, {TEXT("Lock: 7312 accepted"), 3.f}},
		{RewindMessageIds::LockRejected, {TEXT("Lock: {0} rejected"), 3.f}},
		{RewindMessageIds::FuseCarried, {TEXT("Fuse: carried"), 3.f}},
		{RewindMessageIds::FuseSeatedBuilding,
			{TEXT("Fuse: in the building socket"), 3.f}},
		{RewindMessageIds::FuseSeatedCourtyard,
			{TEXT("Fuse: in the courtyard socket"), 3.f}},
		{RewindMessageIds::FuseTakenBuilding,
			{TEXT("Fuse: taken from the building socket, carried"), 3.f}},
		{RewindMessageIds::FuseTakenCourtyard,
			{TEXT("Fuse: taken from the courtyard socket, carried"), 3.f}},
		{RewindMessageIds::SocketEmptyOther,
			{TEXT("Socket: empty. The fuse is in the other socket"), 3.f}},
		{RewindMessageIds::SocketEmptyAtRest,
			{TEXT("Socket: empty. The fuse is still where it started"), 3.f}},
		{RewindMessageIds::GeneratorDead,
			{TEXT("Generator: dead. The fuse is not in the courtyard socket"), 3.f}},
		{RewindMessageIds::GeneratorOnline,
			{TEXT("Generator: online, gate open"), 3.f}},
		{RewindMessageIds::LiftNoPower,
			{TEXT("Lift: no power. Seat the fuse in the building socket"), 3.f}},
		{RewindMessageIds::AnchorAccepted,
			{TEXT("Anchor: courtyard gate will hold"), 4.f}},
		{RewindMessageIds::AnchorRefused,
			{TEXT("Anchor: gate was not opened by this loop"), 4.f}},
		{RewindMessageIds::RadioChannelStatic, {TEXT("Radio: channel {0}  static"), 3.f}},
		{RewindMessageIds::RadioChannelVoice,
			{TEXT("Radio: channel {0}  ...a voice, under the static"), 3.f}},
		{RewindMessageIds::RadioDigitSeven, {TEXT("Radio:  ...seven..."), 5.f}},
		{RewindMessageIds::RadioDigitThree, {TEXT("Radio:  ...three..."), 5.f}},
		{RewindMessageIds::RadioDigitOne, {TEXT("Radio:  ...one..."), 5.f}},
		{RewindMessageIds::RadioDigitTwo, {TEXT("Radio:  ...two..."), 5.f}},
		{RewindMessageIds::RadioCodeObtained, {TEXT("Radio: 7312"), 6.f}},
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
