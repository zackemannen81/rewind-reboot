# Current Task

Task ID: REW-0020
Parent Task: None
Status: Ready
Owner: grok-courtyard
Created: 2026-08-25
Last updated: 2026-08-25
Charter frozen at: 2026-08-25

## Read First

- `AGENTS.md`
- `docs/TASK_WORKFLOW.md`
- `docs/PROJECT_BRIEF.md`
- `docs/CONTRIBUTING.md`
- `docs/CURRENT_STATUS.md`
- `docs/SYSTEMDOC.md`
- `docs/JOURNAL.md`
- `docs/FILESTRUCTURE.md`
- `docs/design/chapter-1-authored.md`
- `docs/design/camera-and-movement.md`
- `docs/design/stairwell-visual-direction.md`
- `docs/adr/ADR-0009_event-driven-loop-termination.md`
- `docs/adr/ADR-0010_renderer-configuration.md`
- `docs/adr/ADR-0011_asset-source-and-provenance.md`
- `docs/acceptance/five-loops-test.md` criteria FL-17 and FL-18
- `docs/concept/README.md`
- `docs/finished/REW-0017_event-driven-rewind-and-the-loop-break-signature.md`
- Relevant ADRs under `docs/adr/`

## Task Summary

The authored default map has no courtyard, so `GroundFuseGate` never exists in
play and REW-0017's event-driven rewind cannot be seen. This task builds the
courtyard the ground-floor power contract is about, places the checkpoint at
that threshold, and produces PIE evidence for the three FL-17 outcomes.

## Task Charter

The charter is editable while status is `Draft` and immutable once status is
`Ready`.

### Goal

The authored default map contains the courtyard, and crossing its
threshold without ground-floor power latches a visible rewind.

### Primary Deliverable

/Game/Maps/FiveLoops_Stairwell_Blockout extended with an authored
courtyard carrying the ground fuse socket, generator, gate and Transit
exit, with GroundFuseGate at its threshold, and PIE evidence that the
prelude and its interference signature are visible there.

### In Scope

- The courtyard as chapter-1-authored.md and the settled ground-floor plan
  define it: entrance hall to courtyard threshold, ground fuse socket,
  generator, courtyard gate, patrol yard and the Transit exit. Blockout
  geometry at the same standard as the existing authored slice.
- Camera regions covering every new playable location, each declaring
  rotation, travel axis, bounds, dead zone, player volume and explicit
  FOV, per camera-and-movement.md. Volumes must abut without a gap:
  REW-0015 showed what a gap costs.
- GroundFuseGate placed at the authored courtyard threshold rather than
  spawned at the procedural proof's X = 4110, without breaking the
  procedural proof map, which keeps its own.
- First-pass light and material treatment consistent with
  stairwell-visual-direction.md and ADR-0010. The courtyard is exterior,
  so it may introduce a sky or moon key; it may not introduce rain, neon
  dressing or final art.
- PIE evidence: crossing the threshold without ground-floor power latches
  rewind, shows the interference, and starts the next loop in 4C; with
  power it passes; with an active courtyard_gate_open Anchor it passes.
- Named automation coverage for whatever new spatial constants this
  introduces.

### Out of Scope

- The street and Transit Hub interiors beyond the courtyard's Transit
  exit. They are a separate bounded task.
- Final environment art, textures, prop meshes, an environment kit or any
  third-party asset import. ADR-0011 put third-party packs outside the
  tracked dependency closure and PROJECT_BRIEF.md still lists final art as
  a non-goal.
- Audio of any kind.
- Echo. ADR-0012 decided it is narrative only.
- The player character model.
- The Apartment 4C lens, which the owner is judging separately.
- Changing the accepted walk speed, or any rule in docs/design/.
- docs/TASK_IDS.md. REW-0020 is already claimed.

### Definition of Done

- The authored default map contains a walkable courtyard reachable from
  the entrance hall, with the ground fuse socket, generator, gate and
  Transit exit placed per the settled ground-floor plan.
- Every new playable location belongs to exactly one camera region, and no
  position reachable by walking leaves the player without movement
  control.
- Crossing the courtyard threshold with no ground-floor power and no
  active Anchor latches rewind, presents the failure for between 1.0 and
  3.0 seconds of loop time with the interference visible, and starts the
  next loop in 4C.
- Ground-floor power passes the same threshold without rewind.
- An active courtyard_gate_open Anchor passes it without ground-floor
  power.
- RewindEditor Win64 Development builds with the editor closed, and all
  discoverable Rewind.* automation tests pass together.

### Minimum Verification Gates

- [ ] Editor-closed build succeeds.
- [ ] All discoverable Rewind.* tests pass together, run headless.
- [ ] Three PIE runs covering the three FL-17 outcomes, with held keys
      released before every stop.
- [ ] A clean viewport capture showing the interference during a prelude.
- [ ] The journal entry names what was verified and what was not.

## References

- `docs/design/chapter-1-authored.md` (`GroundFuseGate`, ground-floor power)
- `docs/design/camera-and-movement.md`
- `docs/design/stairwell-visual-direction.md`
- `docs/adr/ADR-0009_event-driven-loop-termination.md`
- `docs/adr/ADR-0010_renderer-configuration.md`
- `docs/adr/ADR-0011_asset-source-and-provenance.md`
- `docs/acceptance/five-loops-test.md` FL-17, FL-18
- `docs/concept/chapter-1-courtyard-target.png`
- `docs/concept/chapter-1-ground-hallway-target.png`
- `docs/concept/chapter-1-ground-floor-plan.png`
- `docs/finished/REW-0017_event-driven-rewind-and-the-loop-break-signature.md`
- `Rewind/Source/Rewind/RewindProofLayout.cpp`
- `Rewind/Source/Rewind/RewindWorldStateSubsystem.cpp`
- `Rewind/Content/Maps/FiveLoops_Stairwell_Blockout.umap`

## Checklist

- [x] Sync `main`, branch `grok/rew-0020-authored-courtyard`.
- [x] Transcribe the frozen charter into `docs/CURRENT_TASK.md`.
- [ ] Implement `ARewindAuthoredCourtyard` blockout, regions, gameplay and
      threshold checkpoint, attached to `GroundHall_Region` without a gap.
- [ ] Spawn that courtyard on `Rewind.SkipProofLayout` maps only; keep the
      procedural proof's GroundFuseGate at X = 4110.
- [ ] Relocate the existing courtyard/ground fuse socket to the authored
      threshold; wire generator to gate on the authored map.
- [ ] Named `Rewind.*` tests for the new spatial constants and region abutment.
- [ ] Developer command so the Anchor pass can be evidenced without FL-18.
- [ ] Editor-closed `RewindEditor Win64 Development` build.
- [ ] Headless `Rewind.*` automation together.
- [ ] Three FL-17 PIE runs and an interference capture; release held keys.
- [ ] Update `CURRENT_STATUS.md`, `SYSTEMDOC.md`, `FILESTRUCTURE.md` if needed,
      playtest record, journal; archive and restore the template.

## Decisions and Notes

- A checkpoint after each step or substep is required. Checklist is therefore updated along the work and `CURRENT_STATUS.md` is always updated when changes affect the behavior.
- Record decisions and assumptions within the frozen charter.
- Classify discoveries using `docs/TASK_WORKFLOW.md`.
- Concept images are targets, not specifications. The courtyard is side-on in
  `chapter-1-courtyard-target.png`; rain and neon in that picture are not
  authorised. The foreground fence is geometry, not a post-process vignette.
- Proof-layout coordinates do not transfer. REW-0013 corrected the building
  side; REW-0015 corrected player volumes. The courtyard attaches to the
  authored `GroundHall_Region`'s positive-Y face so volumes abut.
- The procedural proof keeps `EnsureCausalCheckpoints` at X = 4110.
  `Rewind.SkipProofLayout` maps do not take that spawn; they receive
  `ARewindAuthoredCourtyard` instead, which places `GroundFuseGate` at its
  own hall-to-courtyard seam.
- `Rewind.SetActiveAnchor` is a developer verification command, not a player
  action and not a new Anchor rule. It only accepts `courtyard_gate_open` or
  none, matching `URewindSessionSubsystem::LoadSession`.
- Courtyard camera FOV, offset and moon key are first-pass authored values
  required by `camera-and-movement.md` (every region must declare a lens) and
  by this charter's lighting permission. They are not new design rules.
- Routed, not decided: street and Transit Hub interiors past the Transit
  exit remain a later task. FL-18 (Anchor commit as a loop boundary) is not
  this charter's PIE gate.

## Charter Amendment Log

Only non-semantic corrections are allowed after `Ready`.

-none

## Verification

- [ ] Editor-closed build succeeds.
- [ ] All discoverable Rewind.* tests pass together, run headless.
- [ ] Three PIE runs covering the three FL-17 outcomes, with held keys
      released before every stop.
- [ ] A clean viewport capture showing the interference during a prelude.
- [ ] The journal entry names what was verified and what was not.

## Documentation Updates

- [ ] `docs/CURRENT_STATUS.md`
- [ ] `docs/SYSTEMDOC.md`
- [ ] `docs/JOURNAL.md`
- [ ] `docs/FILESTRUCTURE.md` when structure changes
- [ ] `docs/playtests/` index and FL-17 record
- [ ] ADRs when long-lived decisions change

## Handoff and Follow-ups

- Current state: charter frozen; implementation starting.
- Next recommended step: courtyard actor, checkpoint placement, tests.
- Blockers: none
- Child tasks: none
- Resume condition: not paused
- Open questions: none that this charter may decide

## Finalize When Complete

- Archive this file under `docs/finished/`.
- Restore this template or populate the next approved task.
- Add a signed `docs/JOURNAL.md` entry.
- If Goal or Definition of Done changed, supersede this task instead of
  rewriting it.
