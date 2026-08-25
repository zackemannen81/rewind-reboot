# Current Task

Task ID: REW-0028
Parent Task: None
Status: Complete
Owner: codex-decals
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

The owner's concept frames read weathered — cracked plaster, rust streaks,
peeling paint. A tiled wall texture alone reads flat. The difference is decals:
grime projected on top of clean surfaces.

## Task Charter

The charter is editable while status is `Draft` and immutable once status is
`Ready`.

### Goal

The grime, crack and peeling-paint atlases are usable as drop-in decals
without anyone opening a material editor.

### Primary Deliverable

The three atlas masks and three neon emissives imported as tracked textures, and one material instance per atlas cell on M_REW_GrimeDecal, each pre-set to its own cell.

### In Scope

- Import the three atlas masks: sRGB OFF, compression TC_Masks, address
  Clamp. They are linear opacity data, not colour; a gamma curve applied
  to them is wrong, not merely wasteful.
- Import the three neon emissive textures: sRGB ON, address Clamp, alpha
  preserved.
- Determine each atlas's real cell grid by inspecting the images, rather
  than assuming. The README states counts (4, 6, 6) but not layout.
- One MaterialInstanceConstant per cell, named MI_REW_Decal_<Kind><N>,
  with Mask, UVScale and UVOffset set so the instance shows exactly that
  cell and nothing of its neighbours.
- A Tint default that suits a near-black room: grime should darken and
  stain, not paint a light patch.
- A short markdown note under Rewind/ArtSource/Textures/ recording
  provenance and the cell grid you found for each atlas.

### Out of Scope

- Any .umap. You must not open or modify the level, and you must not place
  any DecalActor. The operator owns placement.
- The eight tileable surfaces in JPG_Import. REW-0027 owns those.
- Any C++ under Rewind/Source/. REW-0029 owns that.
- Editing M_REW_GrimeDecal itself. Make instances of it.
- docs/TASK_IDS.md. Already claimed.

### Definition of Done

- Six textures imported and tracked through Git LFS, with the mask
  textures confirmed non-sRGB.
- Sixteen material instances exist, one per atlas cell across the three
  atlases, each with Mask, UVScale and UVOffset set.
- The cell grid used for each atlas is recorded, with how it was
  determined.
- RewindEditor Win64 Development builds with the editor closed and all
  discoverable Rewind.* automation tests pass together.

### Minimum Verification Gates

- [ ] Editor-closed build succeeds.
- [ ] All discoverable Rewind.* tests pass together, run headless.
- [ ] The journal entry lists every asset created and names what was verified
  and what was not.

## References

- `docs/adr/ADR-0011_asset-source-and-provenance.md`
- `docs/design/stairwell-visual-direction.md`
- `docs/concept/README.md`
- `C:\Users\zakri\Desktop\rewind-environment-texture-kit-v1\README.md`
- `/Game/Art/Materials/Decals/M_REW_GrimeDecal`

## Checklist

- [x] Inspect the PNG atlas layouts and copy the six provenance sources.
- [x] Import and configure the six textures.
- [x] Create and configure the sixteen cell material instances.
- [x] Checkpoint the asset work and push it.
- [x] Update the owning status, structure, source-provenance and journal documents.
- [x] Run the editor-closed build and all discoverable headless `Rewind.*` tests.
- [x] Archive the charter and restore this template.

## Decisions and Notes

- A checkpoint after each step or substep is required. Checklist is therefore updated along the work and `CURRENT_STATUS.md` is always updated when changes affect the behavior.
- Record decisions and assumptions within the frozen charter.
- Classify discoveries using `docs/TASK_WORKFLOW.md`.

## Charter Amendment Log

Only non-semantic corrections are allowed after `Ready`.

-none

## Verification

- [x] Editor-closed build: `Result: Succeeded` (57.82 seconds).
- [x] Full `Rewind.*` automation run, headless: 22 successes, 0 failures,
  `TEST COMPLETE. EXIT CODE: 0`.
- [x] Asset parameter inspection after import: the headless script read back
  the parent, Mask, UVScale and UVOffset for all sixteen instances.
- [ ] `git diff --check` (run during finalization).

## Documentation Updates

- [x] `docs/CURRENT_STATUS.md`
- [ ] `docs/SYSTEMDOC.md` (no durable system boundary changed; assets are
  covered by the status and structure records).
- [x] `docs/JOURNAL.md`
- [x] `docs/FILESTRUCTURE.md` when structure changes
- [x] ADRs when long-lived decisions change (reviewed; none required)

## Handoff and Follow-ups

- Current state: complete. Six imported textures and sixteen verified cell
  instances exist; their tracked sources and reproducible importer are present.
- Next recommended step: the operator may place DecalActors and separately
  verify in-level visual treatment.
- Blockers: none.
- Child tasks: none.
- Resume condition: not applicable; task complete.
- Open questions: none.

## Finalize When Complete

- Archive this file under `docs/finished/`.
- Restore this template or populate the next approved task.
- Add a signed `docs/JOURNAL.md` entry.
- If Goal or Definition of Done changed, supersede this task instead of
  rewriting it.
