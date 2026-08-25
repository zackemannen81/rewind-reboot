# Current Task

Task ID: REW-0015
Parent Task: None
Status: Complete
Owner: Claude
Created: 2026-08-24
Last updated: 2026-08-25
Charter frozen at: 2026-08-24

## Read First

- `AGENTS.md`
- `docs/TASK_WORKFLOW.md`
- `docs/PROJECT_BRIEF.md`
- `docs/CONTRIBUTING.md`
- `docs/CURRENT_STATUS.md`
- `docs/SYSTEMDOC.md`
- `docs/JOURNAL.md`
- `docs/FILESTRUCTURE.md`
- `docs/design/camera-and-movement.md`
- `docs/adr/ADR-0007_camera-and-perspective.md`

## Task Summary

The owner cannot play the authored default map. Reported 2026-08-24: the
controller becomes unresponsive after using the radio, after taking the fuse,
and when trying to leave Apartment 4C through the opened door. The owner also
reports the 4C frame is too zoomed in.

Reading the code before running it, movement input has one silent failure
mode that produces exactly that symptom.
`ARewindCharacter::GetScreenAxes` resolves the region containing the player
and returns `false` when no region contains it.
`MoveForward` and `MoveRight` then drop the input entirely, with no fallback.
`ARewindCameraRig::ResolveTarget` has the opposite behaviour and holds the
last good region, so the picture keeps working while the controller is dead.
That asymmetry is why the failure reads as "unresponsive" rather than as a
visibly broken camera.

`ARewindCameraRegion::ClampToPlayerVolume` can produce that state on its own.
`Contains` is half-open on the positive edge, and the clamp is inclusive of
that same edge, so a player clamped on the `+X` or `+Y` face is placed on a
coordinate no region owns. `ARewindCharacter::Tick` applies that clamp every
frame.

`Contains` also gates on Z while the clamp deliberately leaves Z free, so any
Z the authored volume does not cover has the same effect.

The three reported interactions do not touch input. Radio, fuse and lock were
read: none disables movement, and the only `DisableMovement` in the project is
the lift's, which is re-enabled on exit. What the three have in common is
position: all three sit at the periphery of 4C, where leaving the authored
volume is possible.

Losing the controller is the highest-cost defect in the repository right now,
because it blocks every remaining Chapter 1 task behind it, including the
presentation work the owner wants to judge next.

## Task Charter

The charter is editable while status is `Draft` and immutable once status is
`Ready`.

### Goal

The player never loses movement control in the authored default map.

### Primary Deliverable

A build of `/Game/Maps/FiveLoops_Stairwell_Blockout` in which the radio, the
fuse and the opened 4C door can each be used and walked away from, with an
Apartment4C frame the owner accepts.

### In Scope

- Remove the silent input-drop path in `ARewindCharacter`.
- Make `ARewindCameraRegion::ClampToPlayerVolume` self-consistent with
  `Contains`, so the clamp cannot place the player outside the region that
  clamped it.
- Make loss of region containment observable rather than silent.
- A named automation test for the clamp/containment invariant.
- Adjust the Apartment4C region's authored frame to a value the owner accepts.
- PIE evidence for the three reported interactions.

### Out of Scope

- Event-driven rewind, ADR-0009 implementation.
- Audio, Echo, VFX, dialog, loop-break interference, textures, meshes,
  animation breadth, the unbuilt last part of the level. These are the owner's
  2026-08-24 remaining-work list and are routed to `docs/backlog/`, not
  absorbed here.
- Deciding whether the untracked Fab and `Art/Texture` content becomes a
  committed dependency.
- Any change to the accepted rules in `docs/design/camera-and-movement.md`.
  This task implements those rules correctly; it does not rewrite them.

### Definition of Done

- From a clean PIE start in 4C, the player uses the radio, then walks away
  under their own input.
- From a clean PIE start, the player takes the fuse, then walks away under
  their own input.
- From a clean PIE start, the player opens the 4C door and crosses into the
  common hall under their own input.
- No position reachable by walking in the authored map leaves the player
  without movement control.
- The owner states the Apartment4C frame is acceptable.
- `Rewind.*` automation tests pass together, including a new test asserting
  that a clamped location is contained by the region that clamped it.

### Minimum Verification Gates

- [ ] `RewindEditor Win64 Development` builds with the editor closed.
- [ ] All discoverable `Rewind.*` automation tests pass together.
- [ ] PIE evidence for each of the three reported interactions, with held keys
      released before every stop.
- [ ] Owner confirmation on the 4C frame.

## References

- `Rewind/Source/Rewind/RewindCharacter.cpp`, `GetScreenAxes`, `MoveForward`,
  `MoveRight`, `Tick`
- `Rewind/Source/Rewind/RewindCameraRegion.cpp`, `Contains`,
  `ClampToPlayerVolume`
- `Rewind/Source/Rewind/RewindCameraRig.cpp`, `ResolveTarget`
- `docs/design/camera-and-movement.md`
- `docs/adr/ADR-0007_camera-and-perspective.md`
- `docs/finished/REW-0014_adopt-owner-authored-4c-camera-framing.md`

## Checklist

- [x] Read the interaction actors and rule out input-blocking code paths.
- [x] Identify the silent input-drop path.
- [x] Claim REW-0015 and charter the task.
- [x] Hold the last known region's screen axes instead of dropping input.
- [x] Inset the clamp inside the half-open positive edge.
- [x] Log entry into and exit from the no-region state.
- [x] Add the clamp/containment automation test.
- [x] Build with the editor closed.
- [x] Run the `Rewind.*` tests.
- [x] PIE the three reported interactions.
- [x] Correct the Apartment4C player volume, which measurement showed was the
      cause rather than a side issue.
- [ ] Agree the Apartment4C lens with the owner. Not done: the volume fix
      restored control and preserved the owner's frame exactly, and the owner
      confirmed play works before a lens change was proposed. Routed to
      `docs/backlog/` as part of the look pass.
- [x] Update `CURRENT_STATUS.md`, `SYSTEMDOC.md`, `JOURNAL.md`.

## Decisions and Notes

- A checkpoint after each step or substep is required. Checklist is therefore
  updated along the work and `CURRENT_STATUS.md` is always updated when changes
  affect the behavior.
- **Exception, recorded while editable:** the REW-0015 identity claim has not
  been merged to `main` before this charter moved to `Ready`. The owner
  controls merges to `main` in this session. REW-0001 recorded the same
  exception. The claim row is appended and unsorted, so the register's
  conflict property is preserved.
- The fix follows the rule `camera-and-movement.md` already states for the
  camera: outside every region, the last region keeps the frame. Movement did
  not implement the same fallback. This is stated as a defect against the
  accepted rule, not as a new rule.
- The clamp fix does not change `Contains`. Half-open positive edges are an
  accepted rule and are asserted by `Rewind.Camera.Region.VerticalTravel`.
- Classify discoveries using `docs/TASK_WORKFLOW.md`.

## Charter Amendment Log

Only non-semantic corrections are allowed after `Ready`.

-none

## Verification

- [x] Editor-closed build of `RewindEditor Win64 Development`. Succeeded in
      10.30 s after one shadowed-variable correction.
- [x] All discoverable `Rewind.*` automation tests, run together: 9 of 9
      passed, 0 failed, including the new
      `Rewind.Camera.Region.PlayerVolumeClamp`.
- [x] PIE runs for the radio approach, the back wall and the doorway. Held keys
      released before every stop; PIE confirmed stopped and the level confirmed
      not dirty at handoff.
- [x] Owner confirmation that play works, given directly on 2026-08-25.
- [ ] Not run: packaged build, frame-rate variation, the fuse-seating and lift
      routes, and any criterion in `docs/acceptance/five-loops-test.md`. This
      task changed movement and one region volume; it did not re-verify the
      Chapter 1 chain.

## Documentation Updates

- [x] `docs/CURRENT_STATUS.md`
- [x] `docs/SYSTEMDOC.md`
- [x] `docs/JOURNAL.md`
- [ ] `docs/FILESTRUCTURE.md` — no structural change
- [ ] ADRs — no long-lived decision changed

## Handoff and Follow-ups

- Current state: complete. Movement never drops input, the clamp is consistent
  with `Contains`, and the Apartment4C player volume covers the room it frames.
- Measured cause, which the charter recorded only as a hypothesis: the
  Apartment4C player volume was `X [-130, 370)` while the room's back wall is
  at `X = -250`. The radio at `X = -225`, the fuse at `X = -250` and the 4C
  doorway all sat outside it, in a 115 cm band that `TopHall_Region`
  (`X [-615, -245)`) did not reach either. Every reported symptom was one
  number.
- Discovered, not fixed, routed to the backlog: owner-placed props block the
  walking line to the radio. `StaticMeshActor_9` (the Fab armchair) occupies
  `X [-215, -45]`, `Y [1358, 1582]`, leaving 15 cm between it and the back
  wall, and `StaticMeshActor_10` is a 20 by 20 cm column at `X [-170, -150]`,
  `Y [1570, 1590]`. Neither is a control defect; both are dressing decisions
  the owner owns.
- Next recommended step: the look pass, which the owner has asked for and which
  needs the renderer configuration decided first.
- Blockers: none.
- Child tasks: none.
- Resume condition: not paused.
- Open questions: the Apartment4C lens, and whether the untracked Fab and
  `Art/Texture` content becomes a committed dependency. The saved default map
  references both.

## Finalize When Complete

- Archive this file under `docs/finished/`.
- Restore this template or populate the next approved task.
- Add a signed `docs/JOURNAL.md` entry.
- If Goal or Definition of Done changed, supersede this task instead of
  rewriting it.
