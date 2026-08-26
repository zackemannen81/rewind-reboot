# Current Task

Task ID: REW-0039
Parent Task: REW-0038
Status: In Progress
Owner: codex-4c-art
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
- `docs/EDITOR_AUTOMATION.md`
- `docs/design/chapter-1-authored.md`
- `docs/design/camera-and-movement.md`
- `docs/design/stairwell-visual-direction.md`
- `docs/design/save-and-session.md`
- `docs/design/tutorial-and-first-run.md`
- `docs/concept/README.md` and the Apartment 4C concept images it indexes
- `docs/adr/ADR-0005_asset-storage.md`
- `docs/adr/ADR-0010_renderer-configuration.md`
- `docs/adr/ADR-0011_asset-source-and-provenance.md`
- `docs/finished/REW-0030_apartment-4c-enlarged-to-the-concept-topology.md`
- `docs/finished/REW-0031_surface-normal-and-roughness-maps-for-the-environment-kit.md`
- `docs/finished/REW-0032_surface-detail-maps-that-do-not-break-decal-rendering.md`

## Task Summary

REW-0038 delegates one bounded art implementation pass on Apartment 4C. The
accepted authored room already has its gameplay actors, deterministic loop,
fixed camera, shell, generated props, surface family and decals, but the
runtime frame remains materially unlike the owner-supplied light-enhanced
target. This child owns the map/content art correction and its isolated-clone
evidence. The operator retains every scope decision, canonical integration,
merge and final integrated Unreal evidence.

## Task Charter

The charter is frozen. Goal, deliverable, scope, out-of-scope, definition of
done and minimum gates may not be weakened or semantically rewritten.

### Goal

Bring the authored Apartment 4C runtime frame materially closer to the
operator-supplied light-enhanced target while preserving accepted gameplay,
camera, interaction and deterministic-loop behavior.

### Primary Deliverable

A clean-saved `FiveLoops_Stairwell_Blockout.umap` plus tracked project-owned
materials/props that produce an exact-camera 4C frame matching the target's
composition, six prop zones, surface character and motivated warm/cold
lighting, delivered as a reviewed-ready PR.

### In Scope

- Read-only primary target:
  `C:\code\rewind-reboot\docs\concept\apartment-4c-interior-light-enhanced.png`.
- Read-only baseline:
  `C:\code\rewind-reboot\Rewind\Saved\Screenshots\WindowsEditor\RewindPIE.png`.
- Existing repository concepts: `docs/concept/apartment-4c-interior.png`,
  `docs/concept/apartment-4c-walkable-and-interactions.png`, and
  `docs/concept/apartment-4c-window-city-view.jpg`.
- Reuse and fix existing tracked Chapter 1 Meshy props before creating
  anything: sofa, low table/rug, shelf, radio, desk/cabinet, chair, door,
  coat, pendant lamps, kitchen table, oven, window railing/plant and 4C sign.
- Bind appropriate tracked surface instances and accepted normal/roughness
  maps to shell and props.
- Compose six target zones: window/city; sofa/table/rug; radio cabinet/chair/
  shelf; neon/technical wall; entry door/keypad/coat; kitchen/oven/cabinets/
  table.
- Add restrained pipes/conduits/cables and secondary clutter from existing
  assets or simple project-owned geometry.
- Correct lighting/reflections: motivated warm practicals, cold window fill,
  restrained magenta/cyan accents, readable Returner silhouette, no global
  mirror-wet floor/ceiling.
- Exact runtime camera comparison at about `(1969.78, 1300, 1390)`, rotation
  `(0, 180, 0)`, FOV `37.5`.
- Clean-save map, captures, build, full tests, standard PIE traversal and
  interaction reachability, durable documentation, archive and PR.

### Out of Scope

- No gameplay C++ or gameplay actor-location changes.
- No radio, puzzle, loop, player, input or camera changes.
- No topology expansion or work in other spaces.
- No edit to `docs/TASK_IDS.md`, parent wave files or parent charter.
- No edit to the owner's canonical untracked
  `Rewind/ArtSource/Props/Chapter1/` or `dev/`.
- No speculative Meshy calls and no secret output.

### Exclusive Ownership

Only these paths may change:

- `Rewind/Content/Maps/FiveLoops_Stairwell_Blockout.umap`
- `Rewind/Content/Props/Chapter1/`
- `Rewind/Content/Art/Materials/BuildingSlice/`
- new content under `Rewind/Content/Art/Apartment4C/REW0039/`
- new sources under `Rewind/ArtSource/Apartment4C/REW-0039/`
- `Rewind/ArtSource/Scripts/REW-0039_*.py`
- `Rewind/ArtSource/Screenshots/REW-0039_*`
- `docs/CURRENT_TASK.md`, `docs/CURRENT_STATUS.md`, `docs/SYSTEMDOC.md`,
  `docs/JOURNAL.md`, and the REW-0039 archive

Any exact need outside this list is reported to the operator and waits for an
explicit scope decision.

### Definition of Done

- All six zones are recognizable from the exact camera.
- No major prop is embedded, floating, hidden, absurdly scaled or blocks the
  accepted walk/interactions.
- Window reads as rainy neon city; radio, door and keypad remain focal and
  reachable; the right side reads as kitchen.
- Room stays near-black framed but readable; ceiling/floor reflection no
  longer dominates.
- Shell and prop materials use tracked project-owned assets with correct
  detail maps, and decals survive.
- Before/after exact-camera frames, asset/material/bounds/provenance inventory,
  clean-save, editor-closed build, full `Rewind.*`, PIE traversal/interactions,
  `git diff --check`, LFS closure, archive, signed journal and PR exist.

### Minimum Verification Gates

- [ ] Exact-camera before/after captures use approximately
  `(1969.78,1300,1390)`, `(0,180,0)`, FOV `37.5`, after shader warmup.
- [ ] Every capture is reviewed against all eleven visual-rubric items.
- [ ] Asset/material/bounds inventory names every placed or changed asset,
  material binding, transform, bounds, collision risk and provenance.
- [ ] The map clean-saves, closes and reopens without a dirty package or
  missing dependency.
- [ ] Editor-closed `RewindEditor Win64 Development` succeeds.
- [ ] Complete `Rewind.*` automation suite succeeds.
- [ ] Standard PIE verifies exact camera state, traversal and radio/door
  reachability; every held project key is released before PIE stops.
- [ ] `git diff --check`, ownership-path audit and Git LFS object/attribute
  checks pass.
- [ ] REW-0039 is archived, the task template is restored, a signed newest-
  first journal entry is added, the branch is pushed and a PR against `main`
  is opened without merging.
- [ ] Every Unreal editor/commandlet launched by this child is stopped and the
  global Unreal lease is explicitly released at handoff.

## References

- Parent/operator brief for REW-0038 and REW-0039.
- Authority listed under Read First.
- `/Game/Maps/FiveLoops_Stairwell_Blockout`.
- `/Game/Props/Chapter1/`.
- `/Game/Art/Materials/Surfaces/` and `/Game/Art/Textures/Surfaces/`.
- `/Game/Art/Materials/Decals/Instances/`.
- `Rewind/ArtSource/Scripts/capture_region.py`.

## Checklist

- [x] Verify branch `codex/rew-0039-4c-concept-art`, base `90d3d62`, clean
  worktree, Unreal port ownership and required reading.
- [x] Freeze and push this charter with exact-camera, asset, material and
  bounds baseline evidence.
- [ ] Correct shell materials and floor/ceiling reflection; capture, review,
  commit and push checkpoint 2.
- [ ] Compose window/city and the major prop zones; capture, review, commit and
  push checkpoint 3.
- [ ] Dress kitchen, technical wall and secondary details; capture, review,
  commit and push checkpoint 4.
- [ ] Tune the warm/cold/magenta/cyan hierarchy; create the final exact-camera
  art capture; review, commit and push checkpoint 5.
- [ ] Clean collision/traversal and verify radio/door interaction access;
  commit and push checkpoint 6.
- [ ] Clean-save/reopen, editor-closed build, full tests, standard PIE,
  provenance and closure audits, documentation/archive, push and PR.

## Decisions and Notes

- The target is visual direction, not a game-rule authority. Accepted design
  wins on every conflict; visual compromises are reported.
- Gameplay actor transforms and all camera data are preservation boundaries.
- Meshy use is gated by a separate pushed checkpoint and explicit operator
  approval after reporting the missing object, dimensions, rejected existing
  alternatives, expected credits and intended paths. No Meshy call is implied
  by this charter.
- The global Unreal lease belongs to this isolated clone until handoff. Work is
  sequential; held PIE keys are explicitly released before every stop.
- Discoveries are routed through `docs/TASK_WORKFLOW.md`; the frozen scope is
  not expanded in place.

## Charter Amendment Log

- None.

## Verification

- [x] Checkpoint 1 baseline capture and inventory.
- [ ] Checkpoints 2-5 matched exact-camera visual evidence.
- [ ] Checkpoint 6 traversal and interaction evidence.
- [ ] Final clean-save/reopen, build, automation, PIE and repository closure.

## Documentation Updates

- [ ] `docs/CURRENT_STATUS.md`
- [ ] `docs/SYSTEMDOC.md`
- [ ] `docs/JOURNAL.md`
- [ ] REW-0039 archive and restored task template
- [ ] `docs/FILESTRUCTURE.md` is outside exclusive ownership and will not be
  edited; structural changes are described in owned status/system documents.
- [ ] No ADR change expected.

## Handoff and Follow-ups

- Current state: mandatory reading, exact-camera capture and read-only
  asset/material/bounds inventory complete; checkpoint 1 is ready to push.
- Next recommended step: after checkpoint 1 is pushed, correct shell/prop
  material bindings and reflection response without moving gameplay actors.
- Blockers: none.
- Child tasks: none. This actor is the sole implementation child.
- Resume condition: not applicable.
- Open questions: none within the frozen scope.

## Finalize When Complete

- Archive this file under
  `docs/finished/REW-0039_apartment-4c-concept-match-art-pass.md`.
- Restore `docs/CURRENT_TASK.md` from `docs/template_CURRENT_TASK.md`.
- Prepend a signed `docs/JOURNAL.md` entry.
- Push and open a PR against `main`; do not merge.
