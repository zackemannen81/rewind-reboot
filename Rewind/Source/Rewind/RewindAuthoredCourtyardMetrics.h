#pragma once

#include "CoreMinimal.h"

/**
 * Authored courtyard blockout dimensions. Implementation values, not new game
 * rules. The relationships they satisfy are owned by
 * `docs/design/chapter-1-authored.md` and `docs/design/camera-and-movement.md`.
 *
 * The courtyard actor attaches these to `GroundHall_Region`'s positive-Y face
 * at runtime. Proof-layout coordinates (X = 4110) do not transfer.
 */
namespace RewindAuthoredCourtyardMetrics
{
	const FName GroundHallRegionName{TEXT("GroundHall_Region")};
	const FName ApproachRegionName{TEXT("CourtyardApproach_Region")};
	const FName CourtyardRegionName{TEXT("Courtyard_Region")};

	/** Hall extension from GroundHall's +Y face to the courtyard threshold. */
	constexpr double ApproachLengthCm = 1200.0;

	/** Plaza depth along +Y after the threshold. */
	constexpr double CourtyardLengthCm = 2800.0;

	/** Plaza width along X, measured from the hall centre toward the camera. */
	constexpr double CourtyardWidthCm = 2200.0;

	constexpr double WallHeightCm = 300.0;
	constexpr double LowerBandHeightCm = 120.0;
	constexpr double FloorThicknessCm = 10.0;
	constexpr double WallThicknessCm = 20.0;
	constexpr double DoorWidthCm = 240.0;

	/** GroundFuseGate box. Thin on Y because the player walks +Y through it. */
	constexpr double CheckpointExtentXCm = 200.0;
	constexpr double CheckpointExtentYCm = 80.0;
	constexpr double CheckpointExtentZCm = 180.0;
	constexpr double CheckpointHeightCm = 150.0;

	/** First-pass courtyard lens. Every region must declare one; this is not a design rule. */
	constexpr float CourtyardFieldOfView = 42.f;
	constexpr double CourtyardTravelPadding = 400.0;
	constexpr double CourtyardDeadZone = 280.0;

	constexpr double ApproachHalfY() { return ApproachLengthCm * 0.5; }
	constexpr double CourtyardHalfY() { return CourtyardLengthCm * 0.5; }

	/** Local Y of the hall-to-courtyard seam, relative to the courtyard actor. */
	constexpr double ThresholdLocalY() { return ApproachLengthCm; }

	constexpr double ApproachCentreLocalY() { return ApproachHalfY(); }
	constexpr double CourtyardCentreLocalY() { return ApproachLengthCm + CourtyardHalfY(); }
}
