# Current Task

Task ID: REW-0029
Parent Task: None
Status: Complete
Owner: codex-character
Created: 2026-08-26
Last updated: 2026-08-26
Charter frozen at: 2026-08-26

## Read First

- `AGENTS.md`
- `docs/TASK_WORKFLOW.md`
- `Rewind/Source/Rewind/RewindCharacter.cpp`
- `Rewind/Source/Rewind/RewindCharacter.h`
- `docs/design/camera-and-movement.md`
- `docs/design/stairwell-visual-direction.md`
- `docs/concept/README.md`
- `Rewind/ArtSource/Characters/Returner/PROVENANCE.md`
- `docs/EDITOR_AUTOMATION.md`

## Task Summary

Bind the already imported Returner assets to the runtime player while retaining
the accepted silhouette material and existing velocity-driven locomotion.

## Task Charter

The charter is editable while status is `Draft` and immutable once status is
`Ready`.

### Goal

The player pawn is The Returner, idling and walking correctly.

### Primary Deliverable

ARewindCharacter driving the Returner skeletal mesh with A_Returner_Alert
as idle and A_Returner_Walk as walk, correctly aligned to the capsule.

### In Scope

- Replace the Tier1_UAL1 mesh and animation references in
  ARewindCharacter's constructor with the Returner equivalents.
- Keep the existing locomotion logic: idle and walk switch on a velocity
  threshold in Tick. Do not restructure it.
- Alignment: the mesh's feet must meet the capsule bottom, and its
  animation-forward direction must match CharacterMovement's movement
  direction. The existing Tier1 code offsets by capsule half-height and
  rotates -90 degrees on yaw; the Returner's rig may need different
  values. Determine them rather than copying.
- Keep MI_CharacterSilhouette applied to every material slot. The
  near-black silhouette is accepted visual direction and is not yours to
  change.
- Leave the Tier1 assets in place. They are not being deleted in this task.

### Out of Scope

- Any .umap. You must not open or modify the level. The operator owns it.
- Any texture or material asset. REW-0027 and REW-0028 own those.
- Animation breadth beyond idle and walk. The run and second walk clip
  stay unbound.
- Retargeting. The Quaternius skeleton has 65 bones and the Returner 24;
  they are structurally different rigs and the Meshy clips are authored on
  the Returner's own skeleton.
- Changing the accepted walk speed or any rule in docs/design/.
- docs/TASK_IDS.md. Already claimed.

### Definition of Done

- The player pawn uses /Game/Characters/Returner/Returner.
- Idle plays when stationary and walk plays when moving.
- The feet meet the capsule bottom and the figure faces its direction of
  travel, with the values used stated in the journal entry.
- MI_CharacterSilhouette is applied to all material slots.
- RewindEditor Win64 Development builds with the editor closed and all
  discoverable Rewind.* automation tests pass together.

### Minimum Verification Gates

- [x] Editor-closed build succeeds.
- [x] All discoverable Rewind.* tests pass together, run headless.
- [x] Evidence that the alignment is right, not merely that it compiles.
- [x] The journal entry names what was verified and what was not.

## References

- `Rewind/Source/Rewind/RewindCharacter.cpp`
- `Rewind/Source/Rewind/RewindCharacter.h`
- `Rewind/ArtSource/Scripts/inspect_returner_alignment.py`
- `Rewind/ArtSource/Scripts/import_returner_anims.py`
- `docs/EDITOR_AUTOMATION.md`

## Checklist

- [x] Record the frozen REW-0029 charter.
- [x] Inspect Returner mesh bounds and reference pose using the editor Python path.
- [x] Bind the Returner mesh, idle and walk assets with measured alignment.
- [x] Build editor-closed and run all discoverable `Rewind.*` automation tests.
- [x] Update durable status and system documentation, then journal evidence.
- [x] Archive the completed charter and restore the active-task template.

## Decisions and Notes

- A checkpoint after each step or substep is required.
- The frozen scope requires values derived from imported-asset evidence; no
  design rule is introduced by this task.
- Classify discoveries using `docs/TASK_WORKFLOW.md`.

## Charter Amendment Log

Only non-semantic corrections are allowed after `Ready`.

- none

## Verification

- [x] Editor-closed build: succeeded, 2026-08-26.
- [x] Full `Rewind.*` automation run, headless: 22 passed, 0 failed, exit code 0.
- [x] Imported mesh bounds and reference-pose evidence for capsule alignment.
- [x] Document skipped checks and reasons in `docs/JOURNAL.md`.

## Documentation Updates

- [x] `docs/CURRENT_STATUS.md`
- [x] `docs/SYSTEMDOC.md`
- [x] `docs/JOURNAL.md`
- [x] `docs/FILESTRUCTURE.md` — alignment inspector added under an existing directory.
- [x] ADRs — no long-lived decision changed.

## Handoff and Follow-ups

- Current state: complete; archive and restore the active-task template in this change.
- Next recommended step: operator review of the pull request.
- Blockers: none.
- Child tasks: none.
- Resume condition: not applicable.
- Open questions: no PIE visual frame was captured because the required editor MCP toolsets were unavailable in this session.

## Finalize When Complete

- Archive this file under `docs/finished/`.
- Restore this template or populate the next approved task.
- Add a signed `docs/JOURNAL.md` entry.
- If Goal or Definition of Done changed, supersede this task instead of
  rewriting it.
