#include "RewindMessageQueue.h"

#include "RewindMessageCatalog.h"

void FRewindMessageQueue::Enqueue(const FRewindMessageLine& Line)
{
	if (Line.Text.IsEmpty() || Line.DurationSeconds <= 0.f)
	{
		return;
	}

	FRewindMessageLine Queued = Line;
	Queued.RemainingSeconds = Line.DurationSeconds;
	Lines.Add(MoveTemp(Queued));
}

void FRewindMessageQueue::Clear()
{
	Lines.Reset();
}

void FRewindMessageQueue::Tick(float DeltaSeconds)
{
	if (DeltaSeconds <= 0.f || Lines.Num() == 0)
	{
		return;
	}

	float RemainingTick = DeltaSeconds;
	while (Lines.Num() > 0 && RemainingTick > 0.f)
	{
		Lines[0].RemainingSeconds -= RemainingTick;
		if (Lines[0].RemainingSeconds > 0.f)
		{
			break;
		}
		RemainingTick = -Lines[0].RemainingSeconds;
		Lines.RemoveAt(0);
	}
}

const FRewindMessageLine* FRewindMessageQueue::GetActive() const
{
	return Lines.Num() > 0 ? &Lines[0] : nullptr;
}

bool FRewindMessageDirector::EnqueueFromCatalog(
	FName Id, float DurationSeconds, const TArray<FString>& Args)
{
	if (!FRewindMessageCatalog::Find(Id))
	{
		return false;
	}

	FRewindMessageLine Line;
	Line.Id = Id;
	Line.Text = FRewindMessageCatalog::Format(Id, Args);
	Line.DurationSeconds = DurationSeconds;
	if (Line.Text.IsEmpty() || Line.DurationSeconds <= 0.f)
	{
		return false;
	}

	Queue.Enqueue(Line);
	return true;
}

bool FRewindMessageDirector::Show(FName Id)
{
	return EnqueueFromCatalog(Id, FRewindMessageCatalog::DefaultDuration(Id), {});
}

bool FRewindMessageDirector::Show(FName Id, const FString& Arg0)
{
	return EnqueueFromCatalog(Id, FRewindMessageCatalog::DefaultDuration(Id), {Arg0});
}

bool FRewindMessageDirector::ShowTimed(FName Id, float DurationSeconds)
{
	return EnqueueFromCatalog(Id, DurationSeconds, {});
}

bool FRewindMessageDirector::ShowTimed(FName Id, float DurationSeconds, const FString& Arg0)
{
	return EnqueueFromCatalog(Id, DurationSeconds, {Arg0});
}

bool FRewindMessageDirector::ShowSequence(const TArray<FName>& Ids)
{
	bool bAny = false;
	for (const FName Id : Ids)
	{
		bAny = EnqueueFromCatalog(Id, FRewindMessageCatalog::DefaultDuration(Id), {}) || bAny;
	}
	return bAny;
}

void FRewindMessageDirector::Clear()
{
	Queue.Clear();
}

void FRewindMessageDirector::Tick(float DeltaSeconds)
{
	Queue.Tick(DeltaSeconds);
}

FName FRewindMessageDirector::GetActiveId() const
{
	const FRewindMessageLine* Active = Queue.GetActive();
	return Active ? Active->Id : NAME_None;
}

FString FRewindMessageDirector::GetActiveText() const
{
	const FRewindMessageLine* Active = Queue.GetActive();
	return Active ? Active->Text : FString();
}

float FRewindMessageDirector::GetActiveRemaining() const
{
	const FRewindMessageLine* Active = Queue.GetActive();
	return Active ? Active->RemainingSeconds : 0.f;
}
