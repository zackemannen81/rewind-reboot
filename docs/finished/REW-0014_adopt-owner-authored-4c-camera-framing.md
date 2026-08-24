# Current Task

Task ID: REW-0014
Parent Task: REW-0013
Status: Complete
Owner: Codex
Created: 2026-08-24
Last updated: 2026-08-24
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
- `docs/EDITOR_AUTOMATION.md`
- `docs/adr/ADR-0007_camera-and-perspective.md`
- `docs/design/camera-and-movement.md`

## Task Summary

Adopt the owner's newly placed `4c_camera` as the initial authored frame for
Apartment 4C. Runtime uses `Apartment4C_Region`, so its camera properties must
be derived from the reference camera rather than switching gameplay to a
second CameraActor.

## Task Charter

### Goal

Make standard PIE begin Apartment 4C from the position, rotation and horizontal
FOV authored by the owner's `4c_camera`.

### Primary Deliverable

A saved `/Game/Maps/FiveLoops_Stairwell_Blockout` whose
`Apartment4C_Region` produces the `4c_camera` frame at the existing PlayerStart.

### In Scope

- Inspect the saved reference camera transform, lens and Apartment4C region.
- Derive the region offset from the existing PlayerStart and region travel rule.
- Apply the reference rotation and horizontal FOV to `Apartment4C_Region`.
- Preserve `4c_camera` as an editor reference and preserve its authored transform.
- Save, reopen and verify the exact initial PIE camera state and clean frame.
- Update durable status/system/journal documentation and archive the task.

### Out of Scope

- Moving PlayerStart, changing 4C geometry, camera-region bounds, dead zone,
  transitions, other camera regions or gameplay.
- Importing, editing, staging or deleting the owner's untracked texture content.
- C++ changes, final art, prop dressing or wider camera work.

### Definition of Done

- `4c_camera` remains at its owner-authored transform and 35 mm lens.
- On loop start, the runtime camera reports the same world position and rotation
  as `4c_camera`, with its approximately 37.5-degree horizontal FOV.
- The saved map reopens cleanly and standard PIE shows one authored 4C view.
- Relevant camera automation and documentation checks pass.

### Minimum Verification Gates

- [x] Before/after editor inspection records the reference and region values.
- [x] Saved reopen reports the correct map and no dirty package.
- [x] Standard PIE reports `Apartment4C_Region`, exact camera transform and FOV.
- [x] Clean PIE viewport capture visually matches the reference composition.
- [x] Held PIE keys are empty before stop.
- [x] Relevant `Rewind.*` camera/default-map automation tests pass.
- [x] `git diff --check` and Git LFS checks pass; unrelated texture content is unstaged.

## References

- `Rewind/Content/Maps/FiveLoops_Stairwell_Blockout.umap`
- `Rewind/Source/Rewind/RewindCameraRegion.cpp`
- `Rewind/Source/Rewind/RewindCameraRig.cpp`
- `docs/adr/ADR-0007_camera-and-perspective.md`
- `docs/design/camera-and-movement.md`

## Checklist

- [x] Inspect `4c_camera`, PlayerStart and `Apartment4C_Region`.
- [x] Derive and apply the runtime region values.
- [x] Save/reopen and inspect the authored map.
- [x] Verify standard PIE state, clean viewport and held-key cleanup.
- [x] Run relevant automation, LFS and documentation checks.
- [x] Update durable documents, archive and restore the task template.
- [x] Commit with a clean task-owned diff; leave unrelated owner assets untouched.

## Decisions and Notes

- Observed reference: location `(750, 1330, 1330)`, rotation `(0, 180, 0)`,
  35 mm on a 23.76 mm-wide 16:9 filmback, horizontal FOV `37.497356` degrees.
- Existing PlayerStart is `(0, 1580, 1296)`. Because the region follows Y, the
  required region offset is `(640, -250, -10)` against its current actor anchor
  `(110, 1368, 1340)`.
- The owner's modified map and untracked `Rewind/Content/Art/Texture/` existed
  before task implementation. Only the map is in scope; the texture tree remains
  untouched and unstaged.

## Charter Amendment Log

- none

## Verification

- [x] Editor and saved-map evidence recorded.
- [x] PIE camera state and clean frame recorded.
- [x] Relevant automation and repository checks recorded.

- Clean reopen loaded `/Game/Maps/FiveLoops_Stairwell_Blockout`. The reference
  remained at `(750, 1330, 1330)`, rotation `(0, 180, 0)`, 35 mm and horizontal
  FOV `37.497356`; the region retained offset `(640, -250, -10)`, the same
  rotation and FOV.
- Standard floating PIE spawned at `(0, 1580, 1298.15)`. The sole runtime view
  target was `RewindCameraRig_0` in `Apartment4C_Region`, travel axis Y. It
  reported camera `(750, 1330, 1330)`, rotation `(0, 180, 0)` and FOV
  `37.497356`, exactly matching the reference. The clean viewport capture is
  `Rewind/Saved/Screenshots/WindowsEditor/RewindPIE.png` and is generated
  evidence, not tracked content.
- `ReleaseAllKeys` released zero keys and the final state reported an empty
  held-key list before PIE stopped.
- `Rewind.Camera.Region.VerticalTravel` and
  `Rewind.Project.DefaultAuthoredMap` passed together, 2/2 in 0.027780 seconds,
  with no errors or warnings. No C++ changed, so no editor rebuild was required.
- The map resolves to Git LFS. `git diff --check` passed; the owner's untracked
  `Rewind/Content/Art/Texture/` remains untouched and unstaged. Additional
  owner content under `Rewind/Content/Art/Materials/4c_door.uasset` and
  `Rewind/Content/Fab/` appeared during verification and likewise remains
  untouched and unstaged.

## Documentation Updates

- [x] `docs/CURRENT_STATUS.md`
- [x] `docs/SYSTEMDOC.md`
- [x] `docs/JOURNAL.md`
- [x] `docs/FILESTRUCTURE.md` not changed because structure did not change

## Handoff and Follow-ups

- Current state: complete and archived after saved reopen, exact runtime-state
  comparison, clean viewport capture and relevant automation.
- Next recommended step: owner visual review the new 4C start composition.
- Blockers: none known.
- Child tasks: none.
- Resume condition: n/a.
- Open questions: none.

## Finalize When Complete

- Archive as `docs/finished/REW-0014_adopt-owner-authored-4c-camera-framing.md`.
- Restore `docs/CURRENT_TASK.md` from the template.
- Add a signed `docs/JOURNAL.md` entry.
