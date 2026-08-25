#pragma once

#include "CoreMinimal.h"

struct FRewindMessageLine
{
	FName Id;
	FString Text;
	float DurationSeconds = 0.f;
	float RemainingSeconds = 0.f;
};

/**
 * One visible line, then a FIFO of waiting lines. Expiry pops the current line
 * and starts the next. Leftover tick time carries into the next line so a long
 * tick cannot stall the queue.
 */
class FRewindMessageQueue
{
public:
	void Enqueue(const FRewindMessageLine& Line);
	void Clear();
	void Tick(float DeltaSeconds);

	bool HasActive() const { return Lines.Num() > 0; }
	const FRewindMessageLine* GetActive() const;
	int32 GetLineCount() const { return Lines.Num(); }
	int32 GetQueuedCount() const { return FMath::Max(0, Lines.Num() - 1); }

private:
	TArray<FRewindMessageLine> Lines;
};

/**
 * The small API the subsystem exposes: show a line, show a timed line, show a
 * sequence, and clear. Copy comes from the catalog. Unknown ids are no-ops.
 */
class FRewindMessageDirector
{
public:
	bool Show(FName Id);
	bool Show(FName Id, const FString& Arg0);
	bool ShowTimed(FName Id, float DurationSeconds);
	bool ShowTimed(FName Id, float DurationSeconds, const FString& Arg0);
	bool ShowSequence(const TArray<FName>& Ids);
	void Clear();
	void Tick(float DeltaSeconds);

	bool HasActive() const { return Queue.HasActive(); }
	FName GetActiveId() const;
	FString GetActiveText() const;
	float GetActiveRemaining() const;
	int32 GetLineCount() const { return Queue.GetLineCount(); }
	int32 GetQueuedCount() const { return Queue.GetQueuedCount(); }

	const FRewindMessageQueue& GetQueue() const { return Queue; }

private:
	bool EnqueueFromCatalog(FName Id, float DurationSeconds, const TArray<FString>& Args);

	FRewindMessageQueue Queue;
};
