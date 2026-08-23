#pragma once

#include "CoreMinimal.h"

/**
 * Measured construction values for the authored Chapter 1 blockout.
 *
 * These are implementation dimensions, not new game rules. The relationships
 * they satisfy are owned by `docs/design/chapter-1-authored.md`.
 */
namespace RewindChapter1Metrics
{
	constexpr int32 FloorTransitions = 3;
	constexpr int32 StepsPerFlight = 24;
	constexpr double StepRunCm = 140.0;
	constexpr double FloorHeightCm = 400.0;
	constexpr double FourthFloorZ = FloorTransitions * FloorHeightCm;
	constexpr double LiftTravelSeconds = 6.0;
	constexpr double PlayerWalkSpeedCmPerSecond = 200.0;
	constexpr double TurnstilePeriodSeconds = 30.0;
	constexpr double LoopDurationSeconds = 240.0;

	// The power branch is 170 m from the lower landing. Loop B walks it out
	// and back before it can approach the gate; an anchored Loop C skips it.
	constexpr double CourtyardPowerBranchOneWayCm = 17000.0;
	constexpr double LearnedMainRouteCm = 8000.0;

	constexpr double StairRunLengthCm()
	{
		return FloorTransitions * StepsPerFlight * StepRunCm;
	}

	constexpr double StairMinimumSeconds()
	{
		return StairRunLengthCm() / PlayerWalkSpeedCmPerSecond;
	}

	constexpr double LearnedRouteSeconds()
	{
		return LiftTravelSeconds + LearnedMainRouteCm / PlayerWalkSpeedCmPerSecond;
	}

	constexpr double FuseToCourtyardRouteSeconds()
	{
		return StairMinimumSeconds()
			+ (2.0 * CourtyardPowerBranchOneWayCm + LearnedMainRouteCm)
				/ PlayerWalkSpeedCmPerSecond;
	}
}
