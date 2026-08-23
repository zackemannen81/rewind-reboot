# Owner Handmade level adoption

Status: Proposed
Discovered: 2026-08-24, during REW-0009, from the owner-authored live Unreal
blockout
Owner: unassigned
Activated: not activated. No REW identity is claimed for this.

The owner has built the intended Chapter 1 plan directly in Unreal rather than
asking a later task to infer it from concept art. REW-0009 inspected that level
live but did not move, commit or edit it because the task is authority-only and
the asset does not yet have a stable tracked path.

Observed construction input, not authority:

- 4C, its upper hallway, lift, stairwell, ground hallway, courtyard and Transit
  route are laid out as connected authored spaces.
- The vertical stack uses four large alternating stair flights and is much
  taller than the current three-flight C++ proof. Floor separation may still be
  tuned.
- Three owner-placed CineCameraActors cover 4C, the upper hallway and the
  stairwell. Each uses a 35 mm focal length on a 16:9 filmback, producing about
  37.5 degrees horizontal field of view.
- The intended plan in the live blockout takes precedence over reconstructing
  dimensions from the concept drawings; the drawings remain visual targets and
  clarification.

## Proposed outcome

Give the owner-authored map a stable Git LFS path, preserve its geometry and
camera transforms, and make it the spatial source for the playable Chapter 1
blockout. Adapt gameplay actors, collision, camera regions and test routes to
that plan rather than rebuilding a second interpretation in C++.

The adoption task may adjust vertical floor separation and small camera offsets
when capsule traversal or framing evidence requires it. Such adjustments must
be measured and reported; they may not silently replace the owner's plan.

## Why this is outside REW-0009

REW-0009 explicitly excludes map, camera, blockout and implementation changes.
The inspection was required to decide ordering: ADR-0009 had to settle what
ends a loop before geometry encoded the timer-driven chain again.

## Dependencies

- The owner-authored level is saved and assigned a stable tracked path before
  an immutable task or journal record cites it.
- ADR-0007 for authored camera behavior.
- ADR-0009 and the event-driven implementation proposal before the adopted map
  is claimed playable.
- ADR-0005 and available Git LFS budget.

## Suggested verification

- Before/after actor inventory and bounds proving that no owner geometry or
  camera was lost during adoption.
- Exact recorded transform, 35 mm focal length and 16:9 filmback comparison for
  all three owner cameras before runtime-region conversion.
- Capsule-complete PIE routes through 4C, lift, every stair flight, ground
  hallway, courtyard and Transit route.
- Named region enumeration with no playable camera gap.
- FL-17 and FL-18 on the adopted map after event-driven rewind exists.

## Not proposed

Final environment art, materials, rain, neon dressing, final rewind effects or
changing the accepted puzzle chain.
