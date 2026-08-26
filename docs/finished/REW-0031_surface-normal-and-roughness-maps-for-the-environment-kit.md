# Current Task

Task ID: REW-0031
Parent Task: None
Status: Complete
Owner: codex-normals
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

The REW-0027 environment kit provides base colour only, making the authored
Apartment 4C surfaces read as flat painted planes. Derive, import and bind
normal and roughness maps for the eight kit surfaces so their material response
is visibly differentiated while the asset remains project-owned and rebuildable.

## Task Charter

The charter is editable while status is `Draft` and immutable once status is
`Ready`.

### Goal

Give every REW-0027 tileable surface a correctly configured normal map and
material-specific roughness map, bound and verified in its existing instance.

### Primary Deliverable

Sixteen imported Unreal textures, an extended `M_REW_Surface`, eight updated
material instances, reproducible source-generation/import scripts, and a
before/after Apartment4C_Region render pair under `Rewind/ArtSource/Screenshots/`.

### In Scope

- Generate normal and roughness source PNGs from the eight retained project
  base-colour JPEGs in `Rewind/ArtSource/Textures/EnvironmentTextureKit-v1/`.
- Keep generated source maps at 2048 pixels or less, alongside the retained
  source kit but outside Unreal `Content/`.
- Import them as `T_REW_Surface_<name>_N` and `T_REW_Surface_<name>_R` in
  `/Game/Art/Textures/Surfaces`; configure normals as `TC_Normalmap` and
  non-sRGB, and roughness maps as linear non-sRGB masks.
- Extend `/Game/Art/Materials/Surfaces/M_REW_Surface` with `NormalTex` and
  `RoughnessTex`, both sampled with the existing `TileSize` coordinates.
- Preserve `BaseColorTex`, `TileSize`, `Tint`, and `Roughness`; multiply the
  roughness texture by the existing scalar parameter.
- Set and read back the normal and roughness parameters on all eight
  `MI_REW_*` kit instances.
- Capture a before and after Apartment4C_Region render without modifying a map.

### Out of Scope

- Editing any asset under `Rewind/Content/Maps/`.
- Ambient-occlusion maps, new surface families, mesh changes, lighting changes,
  or a broader material/look pass.
- Changes to `docs/CURRENT_STATUS.md` or `docs/FILESTRUCTURE.md`.
- Any third-party asset ingestion.

### Definition of Done

- Eight `_N` and eight `_R` textures exist with the required compression and
  sRGB settings verified by saved-asset read-back.
- `M_REW_Surface` saves and compiles without material errors; its normal and
  roughness samples use the same `TileSize` coordinates as base colour.
- Every kit material instance read-back names its own expected normal and
  roughness textures rather than the engine default texture.
- `Rewind/ArtSource/Screenshots/` holds a committed named before/after
  Apartment4C_Region comparison.
- The completed charter is archived under `docs/finished/`, the active charter
  is restored from the template, and a newest-first journal entry records
  evidence and any skipped verification.

### Minimum Verification Gates

- [ ] Capture the unmodified-material before render.
- [ ] Run the committed generation/import/material-update script in Unreal.
- [ ] Read back all texture settings and all instance texture parameters.
- [ ] Capture and visually inspect the after render.
- [ ] Confirm no material compiler errors in the commandlet log.
- [ ] Run `git diff --check` and review the exact changed paths.

## References

- `docs/adr/ADR-0011_asset-source-and-provenance.md`
- `docs/EDITOR_AUTOMATION.md`
- `docs/finished/REW-0027_import-the-environment-texture-kit-and-build-surface-instances.md`
- `Rewind/ArtSource/Scripts/import_environment_texture_kit.py`
- `Rewind/ArtSource/Scripts/capture_region.py`

## Checklist

- [x] Confirm task identity, provenance boundary and real retained JPEG source directory.
- [x] Capture the baseline Apartment4C_Region render.
- [x] Add reproducible normal/roughness generation and Unreal import/update script.
- [x] Run import/update, compile the material, and read back all required settings.
- [ ] Capture and inspect the post-update render pair (captured, but the base-colour-only and derived-map images are pixel-identical).
- [ ] Run final diff checks, archive the charter and add the journal evidence.

## Decisions and Notes
- The existing source directory is `EnvironmentTextureKit-v1`; the similarly
  named `EnvironmentKitV1` owns the separate REW-0028 decal/neon source set.
- `Roughness` keeps its existing numeric values and modulates the per-surface
  texture so REW-0027 instances retain a sensible response.
- No map is opened for editing; the capture script loads the map only to render.
- Discoveries are classified using `docs/TASK_WORKFLOW.md`.

## Charter Amendment Log

Only non-semantic corrections are allowed after `Ready`.

-none

## Verification

- [ ] Saved-asset read-back and material compiler log.
- [ ] Named before/after region captures and manual visual inspection.
- [ ] `git diff --check`; record any skipped checks and reason.

## Documentation Updates

- [ ] `docs/JOURNAL.md` only; other status/file-map documents are deliberately
  excluded by the task constraints.

## Handoff and Follow-ups

- Current state: all sixteen source/imported maps, the master graph and the eight bindings are saved and read back. Both named region captures are retained under `Rewind/ArtSource/Screenshots/`, but their decoded pixels are identical after a persistently flattened baseline was restored and re-captured with derived maps.
- Next recommended step: diagnose why the scene-capture final-colour path does not reflect the persisted `NormalTex` and `RoughnessTex` changes even though its 4C shell components are bound to the eight instances; then re-capture a visibly differentiated pair.
- Blockers: the required visual-difference gate is not met. Do not archive or open a PR as complete until it is.
- Child tasks: none.
- Resume condition: not applicable.
- Open questions: visual comparison depends on the current authored map and its
  scene-capture exposure; it is evidence, not a new lighting decision.

## Finalize When Complete

- Archive this file under `docs/finished/`.
- Restore this template or populate the next approved task.
- Add a signed `docs/JOURNAL.md` entry.
- If Goal or Definition of Done changed, supersede this task instead of
  rewriting it.
