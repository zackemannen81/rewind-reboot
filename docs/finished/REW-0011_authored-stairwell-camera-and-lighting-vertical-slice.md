# Current Task

Task ID: REW-0011
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
- `docs/backlog/handmade-level-adoption.md`
- `docs/concept/stairwell-4c-landing.png`

## Task Summary

REW-0010 established the owner-shaped stairwell at human scale and proved
capsule traversal, but its single runtime camera cannot follow the player
vertically and its temporary blockout lighting does not express the owner's
fixed-camera reference. The owner explicitly requested a playable camera,
silhouette, shadow, reflection and lighting pass. This task implements that
request as one bounded stairwell vertical slice before the wider authored map
is adopted.

The construction map remains spatial authority for this task. The tracked
stairwell concept image is a visual target, not a source of dimensions or game
rules.

## Task Charter

The charter is frozen. Goal, primary deliverable, scope, out-of-scope,
definition of done and minimum verification gates do not change.

### Goal

Turn the human-scale stairwell into a playable fixed-camera presentation slice
whose camera coverage, silhouette and lighting demonstrate the intended visual
grammar without claiming final Chapter 1 art.

### Primary Deliverable

An updated tracked `/Game/Maps/FiveLoops_Stairwell_Blockout` in which a
possessed player can descend from 4C to the entrance under authored camera
handoffs and vertical following, with a documented, reproducible first-pass
stairwell material and lighting treatment.

### In Scope

- Amend `docs/PROJECT_BRIEF.md` by explicit owner direction to permit one
  bounded environment-presentation vertical slice while retaining final art,
  a general environment kit and an asset pipeline as non-goals.
- Create one owning design document for the stairwell slice's visual rules:
  dirty plaster above, green painted dirty plaster below, worn concrete or
  dark stone circulation surfaces, sparse damage, localized practical light,
  dark readable character silhouette and restrained floor reflection.
- Verify license terms, provenance and measured size before tracking any
  third-party asset. Restore and track only the exact dependency closure the
  slice uses; do not import a complete pack merely because it is available.
- Extend authored camera regions to support Z as their one legal travel axis.
- Give regions an explicit authored lens/FOV and preserve the owner's
  `StairwayCamera` position and 35 mm framing as the starting composition.
- Author complete, non-overlapping camera coverage for the upper threshold,
  stair traversal and entrance threshold, using blends except at real visual
  breaks.
- Tune post process, practical lights, shadows, exposure, silhouette and
  rough/reflection response so stairs, landings, doors and the player remain
  readable in the composed frame.
- Preserve the REW-0010 floor elevations, stair dimensions, door sides and
  capsule collision route except for a measured fix required by play evidence.
- Update durable status, system, file map, design ownership, backlog routing
  and journal in the same change.

### Out of Scope

- Rebuilding or dressing Apartment 4C, the lift shaft, full upper hallway,
  full ground hallway, courtyard, street or Transit Hub.
- Claiming the full Handmade map playable or resolving every Chapter 1 camera
  region.
- Implementing ADR-0009 event-driven rewind, fuse/generator puzzle actors,
  interaction close-ups, rain, neon city dressing, sound or rewind effects.
- Final environment art, a reusable environment kit, procedural dressing,
  broad asset ingestion or a project-wide material pipeline.
- Committing OldWall, PaintedBrickWall or any Fab asset whose license,
  provenance or actual use is not verified.

### Definition of Done

- A possessed player can traverse continuously from floor 4 to entrance and
  back while the active view remains an authored third-person camera.
- Every traversed point belongs to exactly one declared region; the stair
  region follows on Z only, respects dead-zone and bounds, and never changes
  its authored rotation.
- Upper and entrance thresholds hand off predictably and return to the correct
  region in the reverse direction.
- The stair composition starts from the owner camera's 35 mm framing and each
  region declares its lens/FOV explicitly.
- Captured editor and PIE frames show a readable dark silhouette, localized
  practical light, cast shadows, separated wall bands, legible treads and a
  restrained floor reflection without crushed-black navigation surfaces.
- The map reopens without missing references. Every newly tracked binary is
  covered by Git LFS and has recorded provenance, license basis and size.
- REW-0010 geometry and reference-camera evidence remain unchanged except for
  measured, reported adjustments permitted by this charter.
- Status, system, design ownership, file structure, backlog and journal state
  exactly what was and was not achieved.

### Minimum Verification Gates

- [x] Owner camera transform, focal length and target framing recorded before
  implementation
- [x] Selected asset dependency closure, provenance, license basis and byte
  size recorded before staging
- [x] Unreal Editor Win64 Development build
- [x] Saved-map reopen with missing-reference and actor/region enumeration
- [x] Named region coverage audit proving exactly one region per sampled route
  point
- [x] MCP-driven possessed PIE descent and ascent with held keys released
- [x] Camera-state evidence for Z-only follow, fixed rotation, dead zone,
  bounds and both threshold handoffs
- [x] Editor and PIE viewport captures reviewed against the owned visual rules
- [x] Git LFS attributes and staged object audit
- [x] Manual link, fence and design-ownership review
- [x] `git diff --check` clean

## References

- `docs/adr/ADR-0005_asset-storage.md`
- `docs/adr/ADR-0007_camera-and-perspective.md`
- `docs/design/camera-and-movement.md`
- `docs/backlog/handmade-level-adoption.md`
- `docs/concept/stairwell-4c-landing.png`
- `Rewind/Content/Maps/FiveLoops_Stairwell_Blockout.umap`
- `Rewind/Content/Maps/Reference/FiveLoops_Handmade2_Reference.umap`
- `Rewind/Source/Rewind/RewindCameraRegion.h`
- `Rewind/Source/Rewind/RewindCameraRig.cpp`

## Checklist

- [x] Claim REW-0011 and merge the claim to `main`
- [x] Read the repository, camera, storage and editor authority
- [x] Freeze the bounded vertical-slice charter
- [x] Record pre-change camera, region, dependency and viewport evidence
- [x] Restate the bounded visual rules and project-phase exception
- [x] Verify and restore only the required licensed asset closure
- [x] Implement Z travel axis and explicit per-region lens
- [x] Author upper, stair and entrance camera coverage
- [x] Author the material, lighting, silhouette, shadow and reflection pass
- [x] Build, reopen and run the complete PIE camera/traversal verification
- [x] Update durable documentation and backlog routing
- [x] Verify and archive the task

## Decisions and Notes

- The owner's 2026-08-24 request explicitly authorizes a bounded presentation
  slice. It does not authorize final Chapter 1 art or an asset pipeline.
- REW-0010 geometry is the spatial source. Concept art cannot silently move
  walls, doors, landings or floor heights.
- The local third-party import backup is discovery input only. Nothing becomes
  repository content until its exact license basis and actual dependency are
  verified.
- A checkpoint after each implementation wave updates this checklist and every
  affected behavior surface.

## Charter Amendment Log

Only non-semantic corrections are allowed after `Ready`.

- none

## Verification

- [x] Pre-change inventory and framing: `StairwayCamera` `(1520, 0, 1150)`,
  rotation `(0, 180, 0)`, 35 mm, about 37.5 degrees horizontal FOV
- [x] Asset license/provenance/size audit: external dependency closure is
  empty; eight project-authored material assets total 108,671 bytes
- [x] Unreal build: final `RewindEditor Win64 Development` succeeded in 7.85 s
- [x] Map reopen and missing-reference audit: 29 actors, three regions, no
  `/Game/Fab` dependency; map size 445,289 bytes
- [x] Region coverage and camera-state audit: half-open Z intervals
  `[-100,180)`, `[180,1180)`, `[1180,1500)` and named automation coverage
- [x] PIE descent and ascent: five level states down and four return states;
  `RewindCameraRig_0` remained view target and held keys ended empty
- [x] Editor and PIE visual review: clean game-viewport PNG reviewed at upper
  and stair states; material breakup was made finer after the first review
- [x] LFS/staging audit: Unreal binaries resolve to the ADR-0005 LFS filter and
  staged objects are LFS pointers
- [x] Links, fences, ownership and `git diff --check`
- [x] Skipped checks and reasons recorded: no package, final art, wider map,
  ADR-0009 implementation, rain, neon, audio or authored decal pass

## Documentation Updates

- [x] `docs/PROJECT_BRIEF.md`
- [x] `docs/CURRENT_STATUS.md`
- [x] `docs/SYSTEMDOC.md`
- [x] `docs/JOURNAL.md`
- [x] `docs/FILESTRUCTURE.md`
- [x] `docs/design/README.md`
- [x] stairwell visual-direction owner
- [x] `docs/design/camera-and-movement.md`
- [x] `docs/backlog/handmade-level-adoption.md`
- [x] `docs/backlog/README.md`

## Handoff and Follow-ups

- Current state: Complete and ready for review.
- Next recommended step: merge this bounded slice before activating either
  event-driven rewind implementation or a separately chartered adjacent space.
- Blockers: none for this task. Third-party environment assets remain outside
  the dependency closure and still require their own provenance/storage gate.
- Child tasks: none.
- Resume condition: not applicable; the task is complete.
- Open questions: whether wider Handmade adoption follows event-driven rewind
  implementation or another bounded presentation slice.

## Finalize When Complete

- Archive this file under `docs/finished/`.
- Restore this template or populate the next approved task.
- Add a signed `docs/JOURNAL.md` entry.
- If Goal or Definition of Done changed, supersede this task instead of
  rewriting it.
