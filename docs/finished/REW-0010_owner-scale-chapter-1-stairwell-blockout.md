# Current Task

Task ID: REW-0010
Parent Task: None
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
- `docs/adr/ADR-0005_asset-storage.md`
- `docs/adr/ADR-0007_camera-and-perspective.md`
- `docs/design/camera-and-movement.md`
- `docs/design/chapter-1-authored.md`
- `docs/backlog/handmade-level-adoption.md`

## Task Summary

The owner replaced the original oversized stair sketch with
`FiveLoops_Handmade2`, which clarifies the intended four-floor switchback but
still measures about 76.8 metres from entrance to 4C. Rebuild that stairwell at
human scale while preserving an untouched stable copy of the owner's layout.

## Task Charter

The charter is frozen. Goal, primary deliverable, scope, out-of-scope,
definition of done and minimum verification gates do not change.

### Goal

Produce a capsule-playable four-floor Chapter 1 stairwell whose layout follows
the owner-authored switchback and whose floor-to-floor scale is 300 cm.

### Primary Deliverable

A tracked Unreal blockout map at
`/Game/Maps/FiveLoops_Stairwell_Blockout`, together with an unchanged tracked
reference copy at `/Game/Maps/Reference/FiveLoops_Handmade2_Reference`.

### In Scope

- Preserve the owner-authored `FiveLoops_Handmade2` geometry and stair camera
  in a stable reference map before rebuilding.
- Build entrance, floors 1 through 3 and 4C at Z = 0, 300, 600, 900 and
  1200 cm.
- Build four alternating flights, end landings and a blockout enclosure sized
  for the existing 84 cm wide, 192 cm tall player capsule.
- Use approximately 17.65 cm risers, 28 cm treads, 170 cm flight width,
  120 cm floor openings and 20 cm structural wall thickness.
- Place one authored stairwell overview camera after the geometry is fixed.
- Add a PlayerStart and verify collision traversal in PIE when the current
  runtime permits it.
- Update durable documentation and resolve or narrow the Handmade-adoption
  backlog proposal truthfully.

### Out of Scope

- Rebuilding 4C, the lift, entrance hallway, courtyard, street or Transit Hub.
- Implementing ADR-0009, gameplay actors, camera-region runtime switching or
  the Chapter 1 puzzle chain.
- Final materials, lighting, rail meshes, environmental art or sound.
- Replacing the owner's wider level plan or concept-art composition.

### Definition of Done

- The reference and rebuilt maps are tracked through Git LFS and reopen in the
  Unreal Editor.
- Five named floor surfaces are separated by 300 cm, for 1200 cm total.
- Four alternating flights continuously connect those surfaces and fit the
  existing capsule by measurement and collision evidence.
- The reference map's recorded actor inventory and camera transform are
  unchanged by the rebuild.
- A viewport capture demonstrates the four-floor composition at human scale.
- Status, system, file map and journal describe the implemented boundary.

### Minimum Verification Gates

- [x] Reference map actor inventory and camera transform recorded before work
- [x] Five floor elevations and four 300 cm intervals enumerated after work
- [x] Step, landing, wall and opening dimensions enumerated after work
- [x] Capsule traversal attempted in MCP-driven PIE with held keys released
- [x] Target map saved, reopened and actor inventory repeated
- [x] Git LFS tracking and binary size verified
- [x] Manual link and fence review
- [x] `git diff --check` clean

## References

- `docs/backlog/handmade-level-adoption.md`
- `docs/design/camera-and-movement.md`
- `docs/design/chapter-1-authored.md`
- `docs/adr/ADR-0005_asset-storage.md`
- `docs/adr/ADR-0007_camera-and-perspective.md`
- `Rewind/Source/Rewind/RewindCharacter.cpp`

## Checklist

- [x] Claim REW-0010 and merge the claim to `main`
- [x] Measure `FiveLoops_Handmade2` and freeze the 300 cm scale charter
- [x] Create the stable untouched reference map
- [x] Create and clear the rebuilt stairwell map
- [x] Build the measured floors, flights, landings and enclosure
- [x] Place PlayerStart and overview camera
- [x] Prevent the procedural Five Loops proof layout from spawning over the
  standalone stairwell blockout in PIE
- [x] Save, reopen, enumerate and visually inspect the result
- [x] Attempt capsule traversal in PIE
- [x] Update durable documentation and backlog routing
- [x] Verify and archive the task
- [ ] Merge the task

## Decisions and Notes

- The owner confirmed 300 cm floor-to-floor after the 250 cm and 500 cm room
  comparisons.
- `FiveLoops_Handmade2` owns layout intent for this task, not final dimensions.
- The original live asset remains unedited; work happens in a duplicated map.
- A checkpoint after each implementation wave updates this checklist and any
  affected behavior surface.

## Charter Amendment Log

Only non-semantic corrections are allowed after `Ready`.

- none

## Verification

- [x] Unreal actor and bounds enumeration: the saved target reopened with 33
  level actors. Its grouped blockout actors enumerate five named floor
  landings, 68 physical step components, four collision ramps, enclosure and
  guardrails. Floor surfaces are at Z = 0, 300, 600, 900 and 1200 cm.
- [x] Viewport capture: MCP captured the full four-floor composition from the
  owner-placed `StairwayCamera` at `(1520, 0, 1150)`.
- [x] MCP-driven PIE traversal attempt: a standard possessed run descended all
  four flights and reported successive capsule Z values 1298, 998, 698, 398
  and 98 cm. All held keys were released before PIE stopped.
- [x] Map reopen: `/Game/Maps/FiveLoops_Stairwell_Blockout` saved, reference
  map loaded, and target reopened before the repeated inventory.
- [x] Reference preservation: both the original and stable reference contain
  26 actors. Their `stairwayCamera` remains at `(17020, -6330, 1360)` with
  rotation `(0, 180, 0)` and unit scale.
- [x] Dimensions: each floor interval is 300 cm; four flights use 17 steps,
  17.65 cm risers, 28 cm treads and 170 cm width. Structural walls are 20 cm
  and authored door openings are 120 cm wide.
- [x] LFS and disk-size check: the target is 441,048 bytes and the reference
  is 462,724 bytes; both resolve `filter`, `diff` and `merge` to `lfs` with
  text unset.
- [x] Unreal Editor Win64 Development build succeeded after the runtime map
  opt-out and character mesh alignment changes.
- [x] Manual link and fence review.
- [x] `git diff --check` clean.
- [x] Skipped checks recorded: no packaged build or final-art inspection was
  run; both are outside this blockout charter. A clean-clone reopen was not
  run. The owner's later local dressing adds seven `/Game/Fab` dependencies
  that remain deliberately untracked pending ADR-0005 provenance and size
  approval; the tracked blockout geometry itself uses engine cubes.

## Documentation Updates

- [x] `docs/CURRENT_STATUS.md`
- [x] `docs/SYSTEMDOC.md`
- [x] `docs/JOURNAL.md`
- [x] `docs/FILESTRUCTURE.md`
- [x] `docs/backlog/handmade-level-adoption.md`
- [x] `docs/backlog/README.md`

## Handoff and Follow-ups

- Current state: Complete. The isolated human-scale stairwell and untouched
  reference are saved and verified.
- Next recommended step: merge REW-0010, then activate a bounded authored-map
  task for playable camera coverage before an environment-art pass.
- Blockers: none.
- Child tasks: none.
- Resume condition: met.
- Open questions: whether a later adoption task retains this isolated blockout
  map or folds it into the complete Chapter 1 map; how imported Fab asset
  provenance and repository size will be approved under ADR-0005.

## Finalize When Complete

- Archive this file under `docs/finished/`.
- Restore this template or populate the next approved task.
- Add a signed `docs/JOURNAL.md` entry.
- If Goal or Definition of Done changed, supersede this task instead of
  rewriting it.
