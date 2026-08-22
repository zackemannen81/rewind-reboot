#pragma once

#include "CoreMinimal.h"

/**
 * Run events for the Five Loops Test.
 *
 * Every line carries elapsed loop time, because every FL criterion is stated
 * in `t` and a screenshot read by eye is weaker evidence than a stamped line.
 * This is instrumentation. No game rule reads it, and nothing here is world
 * state.
 */
DECLARE_LOG_CATEGORY_EXTERN(LogRewind, Log, All);

namespace RewindLog
{
	/** Elapsed loop time for this context, or -1 when no loop clock exists. */
	double ElapsedSeconds(const UObject* WorldContext);

	/** One timestamped run event. */
	void Event(const UObject* WorldContext, const FString& Message);

	/**
	 * One loop-start apply event, from `RestoreFromBaseline` or an Anchor
	 * override. These run at step 2 and step 3 of the apply order in
	 * `world-state-model.md`, before elapsed loop time is set to zero at step
	 * 5, so they carry no `t`: stamping them would print the previous loop's
	 * final time. They appear immediately above the `LOOP START` line they
	 * belong to.
	 */
	void Baseline(const FString& Message);
}
