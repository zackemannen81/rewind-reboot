#pragma once

#include "CoreMinimal.h"

/**
 * Latched rewind request, per ADR-0009.
 *
 * Once a causal-contract failure or a successful Anchor commit latches, the
 * request cannot be cancelled by stepping back. Prelude duration is measured
 * on elapsed loop time and is clamped to the accepted [1, 3] second band.
 *
 * This is a value type so the bounds, the latch and identical-run behaviour
 * can be asserted without a world.
 */
struct FRewindLatchedRewind
{
	static constexpr double MinPreludeSeconds = 1.0;
	static constexpr double MaxPreludeSeconds = 3.0;
	static constexpr double DefaultPreludeSeconds = 2.0;

	static double ClampPreludeSeconds(double Requested)
	{
		return FMath::Clamp(Requested, MinPreludeSeconds, MaxPreludeSeconds);
	}

	bool TryLatch(FName InCheckpoint, double RequestElapsed, double RequestedPreludeSeconds)
	{
		if (bLatched)
		{
			return false;
		}

		bLatched = true;
		Checkpoint = InCheckpoint;
		RequestElapsedSeconds = RequestElapsed;
		PreludeDurationSeconds = ClampPreludeSeconds(RequestedPreludeSeconds);
		return true;
	}

	bool IsLatched() const { return bLatched; }

	bool ShouldApply(double ElapsedLoopTime) const
	{
		return bLatched
			&& ElapsedLoopTime >= RequestElapsedSeconds + PreludeDurationSeconds;
	}

	double GetApplyElapsedSeconds() const
	{
		return RequestElapsedSeconds + PreludeDurationSeconds;
	}

	void Reset()
	{
		bLatched = false;
		Checkpoint = NAME_None;
		RequestElapsedSeconds = 0.0;
		PreludeDurationSeconds = 0.0;
	}

	bool bLatched = false;
	FName Checkpoint = NAME_None;
	double RequestElapsedSeconds = 0.0;
	double PreludeDurationSeconds = 0.0;
};

/**
 * ADR-0009 commit boundary: a successful first-time write ends the loop.
 * A rejected commit or a redundant already-active commit does not.
 */
namespace RewindAnchorCommit
{
	inline bool EndsLoop(bool bAccepted, bool bPendingAfterCall)
	{
		return bAccepted && bPendingAfterCall;
	}
}

/**
 * Rising interference during a latched prelude.
 *
 * Intensity is a function of elapsed loop time, the latch request time and the
 * prelude duration. It does not read engine time or frame delta.
 */
struct FRewindLoopBreakMath
{
	static double Intensity(double ElapsedLoopTime, double RequestElapsedSeconds, double PreludeDurationSeconds)
	{
		if (PreludeDurationSeconds <= 0.0)
		{
			return 0.0;
		}

		const double Local = ElapsedLoopTime - RequestElapsedSeconds;
		if (Local <= 0.0)
		{
			return 0.0;
		}
		if (Local >= PreludeDurationSeconds)
		{
			return 1.0;
		}
		return Local / PreludeDurationSeconds;
	}
};
