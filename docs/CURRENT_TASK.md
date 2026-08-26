# Current Task

Task ID: REW-0038
Parent Task: None
Status: In Progress
Owner: Codex (process supervisor)
Created: 2026-08-26
Last updated: 2026-08-26
Charter frozen at: 2026-08-26 (Ready)

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
- `docs/design/chapter-1-authored.md`
- `docs/design/camera-and-movement.md`
- `docs/adr/ADR-0005_asset-storage.md`
- `docs/adr/ADR-0010_renderer-configuration.md`
- `docs/adr/ADR-0011_asset-source-and-provenance.md`
- `docs/concept/README.md`
- `docs/finished/REW-0030_apartment-4c-enlarged-to-the-concept-topology.md`
- `docs/finished/REW-0032_surface-detail-maps-that-do-not-break-decal-rendering.md`
- `docs/waves/REW-0038_apartment-4c-concept-target.md`
- `C:\code\docs-first-multiagent-orchestrator-addon\README.md`

## Task Summary

The owner supplied a brighter Apartment 4C target and asked that the authored
room be brought as close to it as practical, starting with 4C. The current
standard-PIE frame proves the topology and some generated props exist, but it
does not read like the target: the upper half is empty black, floor/ceiling
reflection dominates, the window has no city, furniture clusters are sparse,
the kitchen is unreadable and the central radio wall lacks its prop density.

This task is the process-supervisor wave. It freezes the visual target,
delegates one bounded implementation child to Codex, owns every scope and asset
decision, reviews rendered checkpoints and controls merge and final canonical
PIE. It does not directly author the child's map or content changes.

## Task Charter

The charter is editable while status is `Draft` and immutable once status is
`Ready`.

### Goal

Bring the authored Apartment 4C runtime frame materially closer to the supplied
light-enhanced concept while preserving its accepted gameplay, camera,
interaction and deterministic-loop contracts.

### Primary Deliverable

REW-0039 merged to `main` with a clean-saved authored map and tracked
project-owned materials/props that produce a canonical 4C PIE frame matching
the target's composition, prop zones, surface character and motivated
warm/cold lighting.

### In Scope

- Freeze `docs/concept/apartment-4c-interior-light-enhanced.png` as the visual
  target and use the exact Apartment4C runtime camera for comparisons.
- Reuse the existing Meshy props for sofa, low table/rug, shelf, radio, desk or
  cabinet, chair, door, coat, pendant lights, kitchen table, oven and window
  railing/plant where they survive scale, collision and material inspection.
- Bind the existing environment texture kit and its accepted normal/roughness
  maps to the appropriate shell and prop surfaces.
- Dress the target zones: rainy city/window at left; sofa/table/rug; central
  radio cabinet, chair and wall shelf; 4C neon and technical wall; entry
  door/keypad/coat; right-side kitchen, oven, cabinets and table.
- Add restrained pipes, conduits, cables, bottles, books or plants from existing
  project-owned assets or simple project-owned geometry when they materially
  improve the target read.
- Rebalance lighting and reflection so warm practical pools, cold window fill,
  magenta/cyan accents and player silhouette read without global wet gloss or
  crushed-black loss of the room.
- Permit Meshy generation through the existing local API key only for a named,
  operator-approved missing object after the existing inventory is exhausted.
- Delegate, monitor, review, merge and perform canonical build, automation,
  clean-save and standard-PIE visual verification.

### Out of Scope

- Changing gameplay actor locations, radio behavior, puzzle rules, loop timing,
  player speed, interaction ranges or the accepted authored camera transform,
  rotation and 37.5 degree FOV.
- Expanding Apartment 4C topology, moving the door/balcony openings or changing
  hallway, stairwell, lift, courtyard or Chapter 2.
- Replacing the Returner, adding gameplay to decorative props, broad rain,
  weather, final audio mix or final optimization beyond regressions introduced
  by this pass.
- Treating concept details as rules where owning design documents are silent or
  contradictory.
- Editing the owner's untracked canonical
  `Rewind/ArtSource/Props/Chapter1/` or `dev/` directories.
- Spending Meshy credits speculatively or exposing `.env.local` contents.

### Definition of Done

- The exact runtime camera presents all six target zones with recognizable
  silhouettes and no major target prop embedded, floating, hidden or absurdly
  scaled.
- Shell surfaces use appropriate tracked plaster, floor tile, ceramic tile,
  dark metal and worn wood instances with functioning detail maps and decals.
- The window reads as a rainy neon city source; the door/keypad and radio are
  immediate focal interactions; the right side reads as a kitchen.
- The image retains near-black framing but the room remains readable. Floor and
  ceiling reflections support the scene rather than becoming mirror-like or
  dominating half the frame.
- Accepted gameplay/collision/camera/audio behavior remains unchanged and the
  player can traverse and interact in standard PIE.
- REW-0039 carries before/after exact-camera captures, asset/provenance
  inventory, clean editor-closed build, complete `Rewind.*` suite, clean-save
  evidence, full diff review, archive and signed journal entry.
- The operator merges only sufficient work, reruns the combined gates and
  records the final canonical viewport.

### Minimum Verification Gates

- [ ] One clean physical worker clone, branch and exclusive path boundary are
      checked before launch; the canonical clone, task IDs and merge remain
      operator-owned.
- [ ] The Docs-First PowerShell Agent MCP plane passes a harmless smoke task and
      launches Codex, not Grok.
- [ ] Before and after frames use the same runtime Apartment4C camera transform,
      rotation and 37.5 degree FOV at 1920x1080 or a documented equivalent.
- [ ] The worker records actor/asset inventory, material assignments, bounds and
      provenance for every newly generated or imported asset.
- [ ] The authored map clean-saves, an editor-closed `RewindEditor Win64
      Development` build succeeds and the complete `Rewind.*` suite passes.
- [ ] Standard PIE verifies traversal, radio/door interaction reachability,
      unchanged camera state and no blocking decorative collision.
- [ ] Operator visual review explicitly scores target-zone composition,
      material readability, lighting hierarchy, silhouette, reflection control
      and obvious placement defects against the supplied target.
- [ ] `git diff --check` is clean; LFS closure is checked; the owner's
      untracked canonical directories remain untouched.

## References

- `docs/concept/apartment-4c-interior-light-enhanced.png`
- `docs/concept/apartment-4c-interior.png`
- `docs/concept/apartment-4c-walkable-and-interactions.png`
- `docs/concept/apartment-4c-window-city-view.jpg`
- `Rewind/Content/Props/Chapter1/`
- `Rewind/Content/Art/Materials/Surfaces/`
- `Rewind/Content/Art/Textures/Surfaces/`
- `Rewind/Content/Art/Materials/Decals/`
- `Rewind/ArtSource/Scripts/dress_4c_props.py`
- `Rewind/ArtSource/Scripts/light_4c_pass.py`
- `Rewind/Content/Maps/FiveLoops_Stairwell_Blockout.umap`
- `C:\Users\zakri\Desktop\rewind-environment-texture-kit-v1`
- `C:\code\rewind-multiagents`

## Checklist

- [x] Complete and merge REW-0036 before activating unrelated art work.
- [x] Claim REW-0038 and REW-0039 on `main`.
- [x] Inspect the supplied target, current exact-camera PIE frame, existing
      concept set, texture kit and tracked generated-prop inventory.
- [x] Preserve the light-enhanced target at a stable indexed repository path.
- [x] Freeze this supervisor charter and wave ownership plan.
- [ ] Prepare one clean isolated clone at the claimed base and a verbatim
      REW-0039 brief.
- [ ] Smoke-test the process plane, launch Codex and monitor durable checkpoints.
- [ ] Review rendered checkpoints and decide every generation, placement,
      material and lighting tradeoff.
- [ ] Review the full child diff/evidence and return named defects until
      sufficient.
- [ ] Merge REW-0039, rerun canonical combined gates and standard PIE.
- [ ] Update durable documentation, archive REW-0038 and restore the template.

## Decisions and Notes

- The brighter supplied target owns visual judgement for this task; the darker
  existing 4C image remains useful mood context. Neither image overrides design
  authority.
- Exact-camera similarity matters more than unseen room detail. Work outside the
  authored frame must be justified by traversal, reflection, shadow or
  interaction.
- Existing assets are the first choice. Meshy credits are available and
  authorized by the owner, but the operator approves generation only after a
  concrete silhouette gap is demonstrated.
- Material-kit guidance is reference data, not task instruction. Accepted
  project documentation and observed Unreal behavior decide implementation.
- The current PIE frame is the baseline: broad empty ceiling/letterbox,
  mirror-dominant floor/ceiling, missing city window and weak middle/right prop
  zoning are named defects, not an invitation to change the camera.
- One implementation child owns the coupled map/material/prop/light boundary.
  Splitting it across parallel workers would make visual authorship and the
  binary map save conflict.
- The canonical editor is closed before the child takes the shared Unreal
  process/build lease. The operator reopens it only after the worker releases
  the lease and its branch is reviewed.
- Every worker step or substep requires a durable checkpoint. Repository state,
  not MCP process state, is recovery authority.

## Charter Amendment Log

Only non-semantic corrections are allowed after `Ready`.

- none

## Verification

- [x] Baseline standard PIE captured at player start under
      `RewindCameraRig_0`, `Apartment4C_Region`, camera about
      `(1969.78,1300,1390)`, rotation `(0,180,0)`, FOV `37.5`.
- [x] Baseline visual inspection recorded the named composition, reflection,
      city-window, material and prop-zone defects.
- [ ] Worker and canonical implementation gates pending.

## Documentation Updates

- [ ] `docs/CURRENT_STATUS.md`
- [ ] `docs/SYSTEMDOC.md`
- [ ] `docs/JOURNAL.md`
- [ ] `docs/FILESTRUCTURE.md`
- [ ] ADRs only if a long-lived boundary changes

## Handoff and Follow-ups

- Current state: charter frozen; target and existing asset inventory inspected.
- Next recommended step: prepare the isolated REW-0039 worktree and launch
  Codex through the project process plane.
- Blockers: none.
- Child tasks: REW-0039, one implementation child.
- Resume condition: after process loss, inspect the worker branch and resume
  from its last pushed checkpoint.
- Open questions: none; object generation is a per-gap operator decision.

## Finalize When Complete

- Archive this file as
  `docs/finished/REW-0038_operator-bring-apartment-4c-to-concept-target.md`.
- Restore `docs/CURRENT_TASK.md` from the template.
- Add a signed `docs/JOURNAL.md` entry.
- If Goal or Definition of Done changed, supersede this task instead of
  rewriting it.

