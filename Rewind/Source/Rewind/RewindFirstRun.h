#pragma once

#include "CoreMinimal.h"

class FRewindMessageDirector;

/**
 * First-run copy and first-time gating.
 * `docs/design/tutorial-and-first-run.md` owns the rules.
 *
 * Seen-state is a knowledge fact. The knowledge set is the same `TSet<FName>`
 * `URewindSessionSubsystem` stores. This is not a second store.
 */
namespace RewindFirstRun
{
	FName SeenFact(FName MessageId);

	/** Show catalog copy only the first time the knowledge set does not
	 *  already contain the seen-fact. The set is session knowledge. */
	bool ShowOnce(TSet<FName>& Knowledge, FRewindMessageDirector& Director, FName MessageId);

	/** Actor path: same gating against `URewindSessionSubsystem`, then
	 *  `URewindMessageSubsystem::Show`. */
	bool ShowOnce(const UObject* WorldContext, FName MessageId);

	/** Stair camera regions on both the authored slice and the proof layout. */
	bool IsStairRegion(FName RegionName);
}
