# Current Task

Task ID: REW-0012
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
- `docs/adr/ADR-0006_cpp-and-blueprint.md`
- `docs/adr/ADR-0007_camera-and-perspective.md`
- `docs/design/chapter-1-authored.md`
- `docs/design/camera-and-movement.md`
- `docs/design/stairwell-visual-direction.md`

## Task Summary

Extend the accepted standalone authored stairwell into the next playable
Chapter 1 slice: Apartment 4C, its fourth-floor common hallway and the lift.
The owner explicitly asked for the radio, code panel and contested fuse path
to work in that space and for the stairwell's first-pass visual grammar to
continue into it. The prior play attempt opened the procedural FiveLoops map,
so this task also makes the authored map the explicit editor and game default.

## Task Charter

The charter is editable while status is `Draft` and immutable once status is
`Ready`.

### Goal

Make the authored upper building route playable from the 4C loop-start pose
through its radio and code panel to either the stairs or a fuse-powered lift,
under complete authored camera coverage and the accepted first-pass visual
grammar.

### Primary Deliverable

An extended `/Game/Maps/FiveLoops_Stairwell_Blockout` that contains human-scale
Apartment 4C, the fourth-floor hallway and a working four-floor lift, with the
radio, code lock, one exclusive fuse and its building/ground sockets integrated
as C++ gameplay actors; the map is the explicit editor and game startup target.

### In Scope

- Restate the owner's approved extension of the bounded presentation slice in
  `docs/PROJECT_BRIEF.md` and extend the existing visual-direction owner without
  claiming final art.
- Preserve the accepted stairwell geometry, bidirectional route and camera
  behavior while extending the same saved map.
- Block out human-scale 4C, the fourth-floor hallway, lift shaft, cage and
  circulation openings from the owner-authored plan and tracked references.
- Continue the existing separated wall, floor, metal, door, silhouette,
  localized light, stable exposure, shadow and restrained-reflection grammar.
- Add complete, exactly-one authored camera-region coverage for the apartment,
  common hall, lift and their handoffs to the existing stair regions.
- Integrate the existing deterministic C++ radio and code lock so `7312` opens
  the 4C route and other four-digit attempts do not.
- Integrate exactly one carried fuse, the building socket that powers the lift,
  and the ground-floor socket required by the accepted exclusive two-use rule.
- Make the lift carry the player between floor 4 and entrance only while the
  fuse is seated in the building socket; keep stairs always traversable.
- Set editor startup and game default maps to the extended authored map and
  prove standard PIE possesses the expected player there.
- Add or strengthen named automation coverage where deterministic behavior or
  saved-map configuration can be asserted repeatably.
- Update durable documentation and archive the task when verified.

### Out of Scope

- Courtyard, generator, gate, patrol, street, Transit Hub or the rest of the
  Handmade map.
- Implementing ADR-0009 event-driven rewind, causal checkpoint presentation or
  Anchor-commit termination.
- Final environment art, authored decal dressing, rain, neon city dressing,
  final audio content, a reusable environment kit or an asset pipeline.
- Importing or committing unapproved Fab/marketplace assets.
- Changing the accepted radio code/timing, fuse exclusivity, lift rule, player
  speed, camera grammar or stair dimensions.

### Definition of Done

- The saved authored map reopens with Apartment 4C, fourth-floor hall, lift
  shaft/cage and both accepted fuse-socket locations at plausible measured
  human scale, without loss or regression of the completed stairwell.
- A clean standard PIE starts in 4C on this map with the authored camera rig as
  the sole view target and no procedural FiveLoops layout overlaid.
- The player can use the radio, enter `7312`, leave 4C, carry the one fuse,
  seat it in either socket but never both, power the lift from the building
  socket and complete lift and stair routes between floor 4 and entrance.
- Every playable point in the delivered slice belongs to exactly one declared
  camera region and region handoffs retain the required objects and route in
  readable frames.
- Clean PIE frames demonstrate the stairwell's visual grammar continued into
  4C and the hall/lift: dark readable silhouette, localized warm practicals,
  cool separation, cast shadows, stable exposure and restrained reflections.
- Project configuration names the authored map as both editor startup and game
  default, and a saved reopen confirms that target.
- Required build, automation, PIE route and documentation checks pass with
  named evidence and all held input released before PIE stops.

### Minimum Verification Gates

- [x] `RewindEditor Win64 Development` builds successfully with Unreal Editor closed.
- [x] All discoverable `Rewind.*` automation tests pass together; new tests cover any added deterministic or map-config behavior.
- [x] Saved-map reopen records the target path, actor inventory, critical transforms and representative room/door/lift dimensions.
- [x] Clean standard PIE starts in authored 4C and proves radio, wrong/correct code behavior, one-fuse/two-socket exclusivity and powered/unpowered lift behavior.
- [x] PIE traversal proves apartment-to-hall, hall-to-stairs, floor-4-to-entrance-and-back by stairs, and floor-4-to-entrance-and-back by powered lift.
- [x] Camera evidence names every active region/axis/FOV along those routes and finds no gap, overlap, first-person fallback or duplicate view.
- [x] Clean PIE screenshots cover 4C and the hall/lift composition under the delivered lighting/material state.
- [x] Held PIE keys are empty before stopping every evidence run.
- [x] Default-map configuration resolves to the authored map for both editor and game startup.
- [x] Unreal binary additions/modifications resolve to Git LFS and selected asset dependencies contain no unapproved third-party content.
- [x] Live documentation links/fences and `git diff --check` pass.

## References

- `Rewind/Content/Maps/FiveLoops_Stairwell_Blockout.umap`
- `Rewind/Content/Maps/Reference/FiveLoops_Handmade2_Reference.umap`
- `Rewind/Content/FiveLoops_Handmade2.umap`
- `docs/concept/apartment-4c-interior.png`
- `docs/concept/chapter-1-top-floor-plan.png`
- `docs/concept/fuse-box-detail.png`
- `docs/concept/stairwell-4c-landing.png`
- `Rewind/Source/Rewind/RewindRadio.cpp`
- `Rewind/Source/Rewind/RewindCodeLock.cpp`
- `Rewind/Source/Rewind/RewindFuse.cpp`
- `Rewind/Source/Rewind/RewindFuseSocket.cpp`
- `Rewind/Source/Rewind/RewindLift.cpp`

## Checklist

- [x] Inspect tracked references, owner map transforms, current authored-map inventory, gameplay actor contracts and startup configuration.
- [x] Record the bounded presentation-scope extension in owning documentation.
- [x] Extend and save the human-scale authored geometry without regressing the stairwell.
- [x] Continue project-owned material, light, shadow, silhouette and reflection treatment into the new spaces.
- [x] Place/configure radio, code panel, fuse, sockets and lift against their existing C++ authority.
- [x] Author exactly-one camera regions and validate transitions/reachability.
- [x] Correct editor/game default maps and verify clean standard PIE possession.
- [x] Build, run named automation, reopen the save and complete all PIE interaction/traversal/visual gates.
- [x] Update durable status/system/file-map/journal records, archive REW-0012 and restore the task template.
- [x] Audit staged files, LFS attributes, dependencies and final diff; commit with a clean worktree.

## Decisions and Notes

- The owner request on 2026-08-24 explicitly approves extending the completed
  stairwell presentation grammar into 4C and the lift/hall slice. It does not
  approve final art or broad asset ingestion.
- `/Game/Maps/FiveLoops_Stairwell_Blockout` remains the target map so this is a
  continuation of the accepted model, not a second reconstruction.
- The ground-floor fuse socket is included because the accepted one-fuse rule
  requires two exclusive destinations. Its courtyard/generator consequence is
  outside this task.
- A checkpoint after each step or substep is required. Checklist is therefore
  updated along the work and `CURRENT_STATUS.md` is updated when behavior changes.
- Discoveries are routed using `docs/TASK_WORKFLOW.md`; the frozen charter is not expanded.

## Charter Amendment Log

- 2026-08-24: corrected three mistyped tracked concept-reference filenames;
  no goal, deliverable, scope, definition or verification gate changed.

## Verification

- [x] Unreal build evidence recorded: editor-closed UE 5.8 Development build,
  8.91 seconds after the final C++ change.
- [x] Named automation result set recorded: all eight discoverable `Rewind.*`
  tests passed in 0.077351 seconds with zero failures.
- [x] Saved-map reopen and inventory evidence recorded: expected path, 52
  actors, no dirty package, dimensions and critical transforms.
- [x] Interaction and bidirectional traversal evidence recorded in
  `docs/playtests/authored-building-slice-2026-08-24.md`. The unchanged full
  stair route and ground-socket seating use explicitly identified composite
  evidence; new hall handoffs, powered lift both ways and unpowered refusal
  were observed in this task.
- [x] Camera and clean viewport evidence recorded for all seven regions and
  representative 4C/hall/lift frames.
- [x] Default-map and dependency/LFS evidence recorded; the map and new
  project-owned material resolve through LFS and add no third-party dependency.
- [x] Documentation links/fences and staged-diff checks recorded.

## Documentation Updates

- [x] `docs/PROJECT_BRIEF.md`
- [x] `docs/design/stairwell-visual-direction.md`
- [x] `docs/CURRENT_STATUS.md`
- [x] `docs/SYSTEMDOC.md`
- [x] `docs/JOURNAL.md`
- [x] `docs/FILESTRUCTURE.md` when structure changes
- [x] `docs/backlog/handmade-level-adoption.md`

## Handoff and Follow-ups

- Current state: complete; archived authored building slice with 4C, common
  halls, seven camera regions and bidirectional powered lift.
- Next recommended step: owner playtest of the default map, followed by a
  separately chartered courtyard/event-driven-rewind task if approved.
- Blockers: none known.
- Child tasks: none.
- Resume condition: n/a.
- Open questions: exact small camera and lighting offsets may still be tuned
  from owner playtest without changing the accepted authored plan.

## Finalize When Complete

- Archive this file under `docs/finished/REW-0012_authored-4c-fourth-floor-hall-and-lift-slice.md`.
- Restore `docs/CURRENT_TASK.md` from the template.
- Add a signed `docs/JOURNAL.md` entry.
- If Goal or Definition of Done changed, supersede this task instead of rewriting it.
