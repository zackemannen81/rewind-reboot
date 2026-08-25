# Current Task

Task ID: REW-0027
Parent Task: None
Status: Ready
Owner: codex-surfaces
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
- `docs/adr/ADR-0011_asset-source-and-provenance.md`
- `docs/design/stairwell-visual-direction.md`
- `docs/concept/README.md`

## Task Summary

Import the project-generated environment texture kit as reproducible Unreal
surface assets, without changing any level or master material.

## Task Charter

The charter is editable while status is `Draft` and immutable once status is
`Ready`.

### Goal

Every surface in the environment texture kit is a tracked Unreal texture
with a ready-to-use material instance.

### Primary Deliverable

Imported textures under /Game/Art/Textures/Surfaces and one material
instance per surface under /Game/Art/Materials/Surfaces, each built on
M_REW_Surface with a TileSize chosen for that surface.

### In Scope

- Import the eight tileable surfaces and four backdrop/poster textures
  from the kit's JPG_Import folder.
- Import settings per the kit README: sRGB on for base colour, Wrap
  address mode for tileable surfaces, Clamp for signs, posters and
  backdrops.
- Cap every imported texture with MaxTextureSize 1024. This game renders
  dark and letterboxed; 4K maps are not visible and are not free.
- One MaterialInstanceConstant per surface, named MI_REW_<Surface>, with
  BaseColorTex set and TileSize chosen for the real-world scale of that
  material: brick and paving repeat at a larger world size than ceramic
  tile.
- A short markdown note under Rewind/ArtSource/Textures/ recording
  provenance: generated for this project, the kit version, the date.

### Out of Scope

- Any .umap. You must not open or modify the level. The operator owns it.
- The decal atlases and neon emissives in Decals_Emissive. REW-0028 owns
  those.
- Any C++ under Rewind/Source/. REW-0029 owns that.
- Editing M_REW_Surface itself. Make instances of it.
- docs/TASK_IDS.md. Already claimed.

### Definition of Done

- Twelve textures imported and tracked through Git LFS.
- Eight material instances exist, each with BaseColorTex and TileSize set.
- No imported texture exceeds 1024 in its built resolution.
- RewindEditor Win64 Development builds with the editor closed and all
  discoverable Rewind.* automation tests pass together.

### Minimum Verification Gates

- [ ] Editor-closed build succeeds.
- [ ] All discoverable Rewind.* tests pass together, run headless.
- [ ] The journal entry lists every asset created and names what was verified
  and what was not.

## References

- `C:\\Users\\zakri\\Desktop\\rewind-environment-texture-kit-v1\\README.md`
- `docs/adr/ADR-0011_asset-source-and-provenance.md`
- `Rewind/Content/Art/Materials/Surfaces/M_REW_Surface.uasset`

## Checklist

- [ ] Track the source JPEGs and provenance record.
- [ ] Import and configure the twelve textures through a committed editor script.
- [ ] Create and verify the eight surface material instances.
- [ ] Build with the editor closed and run the headless automation suite.
- [ ] Update status, file map and journal.
- [ ] Archive the completed charter and restore this template.

## Decisions and Notes

- A checkpoint after each step or substep is required.
- The kit README is authoritative for each texture's intended use and import settings.
- TileSize values are initial real-world-scale choices, not a new visual-direction rule.
- Classify discoveries using `docs/TASK_WORKFLOW.md`.

## Charter Amendment Log

Only non-semantic corrections are allowed after `Ready`.

-none

## Verification

- [ ] Read back all texture and instance settings from the saved Unreal assets.
- [ ] Editor-closed build.
- [ ] Full `Rewind.*` automation run, headless.
- [ ] `git diff --check`.

## Documentation Updates

- [ ] `docs/CURRENT_STATUS.md`
- [ ] `docs/JOURNAL.md`
- [ ] `docs/FILESTRUCTURE.md`

## Handoff and Follow-ups

- Current state: charter frozen; implementation has not yet started.
- Next recommended step: track sources and run the import script.
- Blockers: none.
- Child tasks: none.
- Resume condition: not paused.
- Open questions: final in-game texel-density judgement remains a later look-pass concern in the kit README.

## Finalize When Complete

- Archive this file under `docs/finished/`.
- Restore this template or populate the next approved task.
- Add a signed `docs/JOURNAL.md` entry.
- If Goal or Definition of Done changed, supersede this task instead of
  rewriting it.
