# Current Task

Task ID: REW-0032
Parent Task: None
Status: Ready
Owner: codex-surfaces2
Created: 2026-08-26
Last updated: 2026-08-26
Charter frozen at: 2026-08-26

## Read First

- `AGENTS.md`
- `docs/TASK_WORKFLOW.md`
- `docs/PROJECT_BRIEF.md`
- `docs/CONTRIBUTING.md`
- `docs/CURRENT_STATUS.md`
- `docs/SYSTEMDOC.md`
- `docs/JOURNAL.md`
- `docs/FILESTRUCTURE.md`
- Relevant ADRs under `docs/adr/`

## Task Summary
A task is never considered done until:
JOURNAL.md, SYSTEMDOC.md, CURRENT_STATUS.md is a jour.

REW-0031's derived maps are valid but its rebuilt surface master removed the
working world-aligned projection graph. That revision made all Apartment 4C
decals disappear and turned the dark-brick bay column white. Reapply the
retained detail maps through a graph that preserves the working projection and
base-colour behavior.

## Task Charter

The charter is editable while status is `Draft` and immutable once status is
`Ready`.

### Goal

All eight environment-kit surfaces use REW-0031 normal and roughness maps
without regressing Apartment 4C's twenty decals or dark-brick response.

### Primary Deliverable

A corrected `M_REW_Surface`, eight correctly bound kit instances, retained
REW-0031 detail assets and reproducible material/binding verification with
matched Apartment 4C wall-region captures.

### In Scope

- Recover the retained REW-0031 detail assets and bindings; do not regenerate
  source maps.
- Reproduce the reported bisect with forced material/shader recompilation.
- Preserve the working base-colour/world-aligned UV contract while adding
  normal and roughness sampling.
- Bind and read back expected detail textures for all eight instances.
- Capture and inspect same-region, same-exposure evidence for decals and brick.
- Archive the charter and record evidence in the newest-first journal.

### Out of Scope

- Any edit to `Rewind/Content/Maps/*`.
- Regenerating or retuning REW-0031 source images.
- New surface families, decals, props, lighting, geometry or broad look work.
- Changes to `docs/CURRENT_STATUS.md` or `docs/FILESTRUCTURE.md`.

### Definition of Done

- All sixteen retained detail textures have required saved settings and all
  eight kit instances read back expected `NormalTex`/`RoughnessTex` values.
- The master compiles without errors and preserves the prior world-aligned
  base-colour projection, sampler/color behavior and compatible TileSize shape.
- Same-wall, same-exposure magnified evidence shows Apartment 4C decals are
  present; map inspection counts twenty visible, materialed decal actors.
- `MI_REW_BrickDark_Aged` is visibly dark brick, not a white column.
- This task is archived, the active charter restored and its evidence journaled.

### Minimum Verification Gates

- [ ] Force recompilation while reproducing the old/new master/instance bisect.
- [ ] Read back master graph parameter names/types and all saved texture and
  instance bindings after writing them.
- [ ] Run the material update/import script headlessly with no `LogMaterial: Error`.
- [ ] Capture matched full and magnified same-wall images at one manual exposure.
- [ ] Run `git diff --check` and review changed paths for map edits.

## References

- `docs/finished/REW-0031_surface-normal-and-roughness-maps-for-the-environment-kit.md`
- `docs/JOURNAL.md` entries dated 2026-08-26 for REW-0030 and REW-0031
- `Rewind/ArtSource/Scripts/capture_region.py`
- `Rewind/ArtSource/Scripts/verify_surface_bindings.py`
- `docs/EDITOR_AUTOMATION.md`

## Checklist

- [x] Read project workflow, REW-0031 record and relevant journal evidence.
- [x] Freeze REW-0032 at `Ready`.
- [x] Inspect and compare working and REW-0031 material graphs.
- [x] Re-run the bisect with forced shader recompilation.
- [x] Implement the minimal compatible master/binding update without maps.
- [x] Run binding, compiler-log and matched capture gates.
- [x] Archive, journal and prepare the branch for review.

## Decisions and Notes
- A checkpoint after each step or substep is required. Checklist is therefore updated along the work and `CURRENT_STATUS.md` is always updated when changes affect the behavior.
- Record decisions and assumptions within the frozen charter.
- Classify discoveries using `docs/TASK_WORKFLOW.md`.

## Charter Amendment Log

Only non-semantic corrections are allowed after `Ready`.

-none

## Verification

- [x] Forced-recompile bisect recorded.
- [x] Saved-asset graph, texture and instance read-back passes.
- [x] Material compiler log is clean.
- [x] Matched full and magnified wall captures inspected.
- [x] `git diff --check` clean; no map paths changed.
- [x] No checks skipped.

## Documentation Updates

- [x] `docs/SYSTEMDOC.md` unchanged: this is a material implementation contract, not a game-system change.
- [x] `docs/JOURNAL.md`.
- [x] `docs/CURRENT_STATUS.md` and `docs/FILESTRUCTURE.md` intentionally unchanged by scope.
- [x] No ADR expected.

## Handoff and Follow-ups

- Current state: complete pending archive and review.
- Next recommended step: review the matched captures and merge through PR only.
- Blockers: none.
- Child tasks: none.
- Resume condition: n/a.
- Open questions: none. `TileSize` remains vector; no REW-0030 prop rerun is needed.

## Finalize When Complete

- Archive this file under `docs/finished/`.
- Restore this template or populate the next approved task.
- Add a signed `docs/JOURNAL.md` entry.
- If Goal or Definition of Done changed, supersede this task instead of
  rewriting it.
