# Current Task

Task ID: REW-0010
Parent Task: None
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

- [ ] Reference map actor inventory and camera transform recorded before work
- [ ] Five floor elevations and four 300 cm intervals enumerated after work
- [ ] Step, landing, wall and opening dimensions enumerated after work
- [ ] Capsule traversal attempted in MCP-driven PIE with held keys released
- [ ] Target map saved, reopened and actor inventory repeated
- [ ] Git LFS tracking and binary size verified
- [ ] Manual link and fence review
- [ ] `git diff --check` clean

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
- [ ] Create the stable untouched reference map
- [ ] Create and clear the rebuilt stairwell map
- [ ] Build the measured floors, flights, landings and enclosure
- [ ] Place PlayerStart and overview camera
- [ ] Save, reopen, enumerate and visually inspect the result
- [ ] Attempt capsule traversal in PIE
- [ ] Update durable documentation and backlog routing
- [ ] Verify, archive and merge the task

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

- [ ] Unreal actor and bounds enumeration
- [ ] Viewport capture
- [ ] MCP-driven PIE traversal attempt
- [ ] Map reopen
- [ ] LFS and disk-size check
- [ ] Manual link and fence review
- [ ] `git diff --check`
- [ ] Skipped checks and reasons recorded

## Documentation Updates

- [ ] `docs/CURRENT_STATUS.md`
- [ ] `docs/SYSTEMDOC.md`
- [ ] `docs/JOURNAL.md`
- [ ] `docs/FILESTRUCTURE.md`
- [ ] `docs/backlog/handmade-level-adoption.md`
- [ ] `docs/backlog/README.md`

## Handoff and Follow-ups

- Current state: In Progress. Scale and source measurements are known.
- Next recommended step: duplicate the reference map before editing geometry.
- Blockers: none.
- Child tasks: none.
- Resume condition: met.
- Open questions: whether a later adoption task retains this isolated blockout
  map or folds it into the complete Chapter 1 map.

## Finalize When Complete

- Archive this file under `docs/finished/`.
- Restore this template or populate the next approved task.
- Add a signed `docs/JOURNAL.md` entry.
- If Goal or Definition of Done changed, supersede this task instead of
  rewriting it.
