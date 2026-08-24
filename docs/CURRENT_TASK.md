# Current Task

Task ID: REW-0014
Parent Task: REW-0013
Status: In Progress
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

- [ ] Before/after editor inspection records the reference and region values.
- [ ] Saved reopen reports the correct map and no dirty package.
- [ ] Standard PIE reports `Apartment4C_Region`, exact camera transform and FOV.
- [ ] Clean PIE viewport capture visually matches the reference composition.
- [ ] Held PIE keys are empty before stop.
- [ ] Relevant `Rewind.*` camera/default-map automation tests pass.
- [ ] `git diff --check` and Git LFS checks pass; unrelated texture content is unstaged.

## References

- `Rewind/Content/Maps/FiveLoops_Stairwell_Blockout.umap`
- `Rewind/Source/Rewind/RewindCameraRegion.cpp`
- `Rewind/Source/Rewind/RewindCameraRig.cpp`
- `docs/adr/ADR-0007_camera-and-perspective.md`
- `docs/design/camera-and-movement.md`

## Checklist

- [x] Inspect `4c_camera`, PlayerStart and `Apartment4C_Region`.
- [x] Derive and apply the runtime region values.
- [ ] Save/reopen and inspect the authored map.
- [ ] Verify standard PIE state, clean viewport and held-key cleanup.
- [ ] Run relevant automation, LFS and documentation checks.
- [ ] Update durable documents, archive and restore the task template.
- [ ] Commit with a clean task-owned diff; leave unrelated owner assets untouched.

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

- [ ] Editor and saved-map evidence recorded.
- [ ] PIE camera state and clean frame recorded.
- [ ] Relevant automation and repository checks recorded.

## Documentation Updates

- [ ] `docs/CURRENT_STATUS.md`
- [ ] `docs/SYSTEMDOC.md`
- [ ] `docs/JOURNAL.md`
- [ ] `docs/FILESTRUCTURE.md` not expected to change

## Handoff and Follow-ups

- Current state: the reference-derived offset, rotation and FOV are applied and
  saved; clean reopen and PIE verification remain.
- Next recommended step: reopen the map and record the initial runtime camera.
- Blockers: none known.
- Child tasks: none.
- Resume condition: n/a.
- Open questions: none.

## Finalize When Complete

- Archive as `docs/finished/REW-0014_adopt-owner-authored-4c-camera-framing.md`.
- Restore `docs/CURRENT_TASK.md` from the template.
- Add a signed `docs/JOURNAL.md` entry.
