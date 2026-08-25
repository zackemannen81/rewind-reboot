# Current Task

Task ID: REW-0013
Parent Task: REW-0012
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
- `docs/design/camera-and-movement.md`
- `docs/design/stairwell-visual-direction.md`

## Task Summary

Correct REW-0012's wrong-side interpretation of the owner blockout. The
fourth-floor corridor already has three door openings in one wall, ordered
stairs, lift and Apartment 4C, with the intended spaces behind them. REW-0012
instead duplicated 4C and the lift on the corridor's opposite side. The owner
has now explicitly confirmed that the existing three-opening arrangement is
the layout authority and that primitive furniture props must not be added.

## Task Charter

The charter is editable while status is `Draft` and immutable once status is
`Ready`.

### Goal

Make the authored building slice follow the owner's existing three-door plan:
stairs, lift and 4C share one corridor wall and occupy the spaces behind their
respective openings, with no duplicate rooms or placeholder furniture.

### Primary Deliverable

A corrected saved `/Game/Maps/FiveLoops_Stairwell_Blockout` in which the
accepted stairwell leads into the existing corridor, the lift shaft aligns to
the middle opening, Apartment 4C occupies the existing room behind the third
opening, and the REW-0012 gameplay, camera and lighting actors are relocated to
that arrangement rather than rebuilding it on the opposite side.

### In Scope

- Restate the owner correction in live visual/status/system documentation.
- Preserve the existing corridor wall at X approximately `-260` and its three
  openings centred near Y `328`, `630` and `1043`, in the order stairs, lift,
  4C.
- Preserve the accepted stairwell, its door, stairs, rails, collision and
  three existing camera regions.
- Remove the wrong-side REW-0012 4C, hall, lift-shaft and entrance-hall
  geometry without deleting the owner's pre-existing floors or openings.
- Use the existing fourth-floor 4C floor/room footprint and align a vertical
  lift shaft to the middle opening at both floor 4 and entrance.
- Relocate PlayerStart, radio, code lock, one fuse, both sockets, lift actor,
  practical lights and the four extension camera regions to the corrected
  spaces.
- Make the lift's hall-side exit direction configurable if the corrected shaft
  requires the opposite handoff from the procedural proof map.
- Keep radio, code, one-fuse/two-socket exclusivity, powered bidirectional lift
  and default-map behavior unchanged.
- Remove primitive blockout furniture/prop dressing. Functional gameplay
  actors may retain their minimal interaction representation.
- Inventory locally imported Fab content. Use no imported asset unless a small
  selected dependency subset has verified provenance, usable license and
  measured repository cost; leaving the corrected rooms undressed is an
  acceptable outcome for this task.
- Update durable documentation and archive the correction when verified.

### Out of Scope

- Courtyard, streets, Transit, generator/patrol dressing or the rest of the
  Handmade map.
- Broad Fab/Marketplace pack ingestion, committing an entire environment pack,
  or establishing a general asset pipeline.
- Final environment art, full apartment furnishing, decals, rain, neon city
  dressing or final audio.
- Changing the stair scale, radio rule, code, fuse rule, six-second lift time,
  global loop implementation or ADR-0009.
- Editing the immutable REW-0012 archive or its historical playtest record;
  live documents record the correction.

### Definition of Done

- Saved-map inspection shows the corridor and all three original openings
  preserved, with stairwell, lift and 4C behind them in the owner-confirmed
  order and no duplicate 4C/lift geometry on the opposite side.
- 4C uses the existing room footprint at plausible 300 cm height; the lift
  shaft aligns vertically with the middle doorway at floor 4 and entrance.
- No primitive couch, desk, kitchen or other placeholder furniture remains.
- Clean standard PIE starts in corrected 4C with exactly one authored camera
  view, accepts `7312`, reaches both stairs and lift through the common
  corridor, refuses the unpowered lift and completes powered travel both ways.
- Exactly-one camera coverage exists throughout 4C, corridor, lift and the
  handoffs to the unchanged stair regions; the required routes and interaction
  landmarks remain readable under the first-pass light/material grammar.
- Imported assets, if any are selected, have recorded provenance/license,
  measured dependency closure and explicit LFS cost; otherwise the map has no
  new third-party dependency.
- Required build, automation, saved reopen, PIE route, documentation and staged
  diff checks pass with held PIE input released before every stop.

### Minimum Verification Gates

- [x] Editor actor/component inventory records all three door openings, corrected room/shaft bounds and absence of wrong-side duplicates.
- [x] Saved reopen records map path, actor count, critical transforms and no dirty package.
- [x] Clean standard PIE proves correct spawn, wrong/correct code, apartment-to-corridor and corridor-to-stairs handoffs.
- [x] PIE proves unpowered refusal and powered lift descent/ascent with clean hall-side handoffs.
- [x] Camera state names every active region, travel axis and FOV and finds no gap, overlap, fallback or duplicate view.
- [x] Clean PIE frames cover corrected 4C, the three-opening corridor and the cutaway lift composition.
- [x] Held PIE keys are empty before every evidence run stops.
- [x] All discoverable `Rewind.*` automation tests pass together.
- [x] `RewindEditor Win64 Development` builds editor-closed if C++ changes.
- [x] Dependency, provenance, license, size and Git LFS checks cover every selected binary asset; broad local Fab content remains excluded.
- [x] Live documentation links/fences and `git diff --check` pass.

## References

- `Rewind/Content/Maps/FiveLoops_Stairwell_Blockout.umap`
- `Rewind/Content/Maps/Reference/FiveLoops_Handmade2_Reference.umap`
- `docs/concept/chapter-1-top-floor-plan.png`
- `docs/design/stairwell-visual-direction.md`
- `Rewind/Source/Rewind/RewindLift.cpp`

## Checklist

- [x] Record the known wrong-side defect and owner correction in live authority.
- [x] Inventory the existing three-opening geometry and locally imported Fab content.
- [x] Remove only the REW-0012 wrong-side geometry and primitive prop dressing.
- [x] Rebuild/complete 4C and the shaft behind the existing third/middle openings.
- [x] Relocate gameplay, lighting, PlayerStart and camera actors.
- [x] Adapt lift handoff direction without regressing the procedural proof map.
- [x] Save/reopen, inspect exact layout and complete PIE interaction/traversal/camera/visual gates.
- [x] Run automation, editor-closed build when required, dependency/LFS and documentation checks.
- [x] Update durable documents, archive REW-0013 and restore the task template.
- [x] Commit with a clean worktree; push or open a PR only on owner request.

## Decisions and Notes

- The existing blockout geometry is primary layout authority. The owner's
  annotated correction confirms the observed openings; it is described here
  rather than cited from a disposable clipboard path.
- The three openings are measured from saved components, not inferred from the
  concept image: approximately 120, 110 and 115 cm wide.
- The owner explicitly removed any need for primitive blockout props. Imported
  Fab content is permission to inspect candidates, not permission to commit
  whole packs without ADR-0005 provenance and size evidence.
- A checkpoint after each step or substep is required. Checklist and live
  status stay truthful while the correction proceeds.

## Charter Amendment Log

- none

## Verification

- [x] Unreal build evidence recorded when applicable.
- [x] Named automation result set recorded.
- [x] Saved-map layout, inventory and dependency evidence recorded.
- [x] Interaction, traversal and lift evidence recorded.
- [x] Camera and clean viewport evidence recorded.
- [x] Documentation and staged-diff checks recorded.

- Editor-closed UE 5.8 build: `RewindEditor Win64 Development` succeeded in
  44.27 seconds after adding the local-space lift hall offset.
- Saved reopen: `/Game/Maps/FiveLoops_Stairwell_Blockout`, 51 actors, no dirty
  package. Wrong-side `Building_4C_Geometry`, `Building_4C_Props` and
  `Building_HallLift_Geometry` are absent. Corrected 4C/shaft actors remain.
- Layout: the preserved X `-260` wall retains openings centred near Y `328`,
  `630` and `1043`; 4C uses the existing 400 by 1050 cm floor at 300 cm room
  height and the shaft centres at `(-40, 630)`.
- Clean PIE: spawn `(0, 1580, 1298.15)` in `Apartment4C_Region`, Y, FOV 60.
  `1111` was rejected and `7312` accepted. The common hall reported
  `TopHall_Region`, Y, FOV 58; the stair threshold reported
  `Stairwell_Upper`, Z, FOV 37.5; the cage reported `LiftShaft_Region`, Z,
  FOV 60. Clean runtime frames covered 4C, all three corridor openings and the
  cutaway cage.
- Lift: empty building socket logged refusal. With the one fuse seated, descent
  and ascent each measured 6.00 seconds. Both released at X `-280`, first into
  `GroundHall_Region`, then `TopHall_Region`; held keys were empty before stop.
- Automation after the clean build: all eight discoverable `Rewind.*` tests
  passed together, 8/8 in 0.075142 seconds, with no errors or warnings.
- Dependencies: no new `/Game` Fab/Marketplace asset exists or is selected;
  the only changed binary is the tracked map under the existing Git LFS rule.

## Documentation Updates

- [x] `docs/design/stairwell-visual-direction.md`
- [x] `docs/CURRENT_STATUS.md`
- [x] `docs/SYSTEMDOC.md`
- [x] `docs/JOURNAL.md`
- [x] `docs/FILESTRUCTURE.md` not changed because no dependency structure changed

## Handoff and Follow-ups

- Current state: complete and archived after saved reopen, clean build, PIE and
  automation evidence.
- Next recommended step: owner visual review and selective, separately reviewed
  Fab prop import when specific assets are present in the project Content Browser.
- Blockers: none known.
- Child tasks: none.
- Resume condition: n/a.
- Open questions: no imported prop is required; any selected replacement must
  pass the task's provenance/license/size gate first.

## Finalize When Complete

- Archive this file under
  `docs/finished/REW-0013_correct-authored-building-side-layout.md`.
- Restore `docs/CURRENT_TASK.md` from the template.
- Add a signed `docs/JOURNAL.md` entry.
- If Goal or Definition of Done changed, supersede this task instead of
  rewriting it.
