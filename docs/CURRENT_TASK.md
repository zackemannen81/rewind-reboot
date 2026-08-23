# Current Task

Task ID: REW-0004
Parent Task: None
Status: Draft
Owner: Claude
Created: 2026-08-23
Last updated: 2026-08-23
Charter frozen at: not frozen

## Read First

- `AGENTS.md`
- `docs/TASK_WORKFLOW.md`
- `docs/PROJECT_BRIEF.md`
- `docs/CONTRIBUTING.md`
- `docs/CURRENT_STATUS.md`
- `docs/SYSTEMDOC.md`
- `docs/FILESTRUCTURE.md`
- `docs/adr/ADR-0007_camera-and-perspective.md`
- `docs/design/chapter-1-authored.md`
- `docs/design/loop-and-determinism.md`
- `docs/playtests/five-loops-2026-08-22.md`
- `docs/backlog/loop-pressure-and-interaction.md`
- `docs/backlog/character-animation-source.md`

## Task Summary

Rebuild the authored space under ADR-0007's camera, at a scale where the
loop timer means something, and test the camera grammar across one
connective space. Do not build Chapter 1. Do not start art direction.

## Task Charter

The charter is editable while status is `Draft` and immutable once status
is `Ready`.

### Goal

An authored space that is played through a camera the author controls, and
in which a wasted loop costs the player something measurable.

### Primary Deliverable

The FiveLoops space rebuilt so that every playable region declares its
camera, plus a design document owning camera grammar, plus a re-measured
golden path and a re-run of every FL criterion the rebuild touches.

### In Scope

- Write the design document that owns camera grammar: regions, follow
  behaviour, dead zone, bounds, handoff between regions, and interaction
  framing. ADR-0007 decides the camera; this document states the rules.
- Decide the player's movement space, which ADR-0007 left silent: a plane,
  a bounded band, or free within the region. Record it in the same
  document, because it is inseparable from the camera that frames it.
- Implement the camera in C++ per ADR-0006. It reads world state and never
  writes it, so that it cannot enter the determinism model.
- Decide loop duration and space scale **together**, per
  `docs/backlog/loop-pressure-and-interaction.md`. Amend
  `docs/design/chapter-1-authored.md`, which owns both, or supersede it.
- Rebuild Apartment 4C, courtyard, street and Transit Hub at the decided
  scale, each with an authored camera.
- Add exactly one connective space between two existing areas, as the test
  of whether region handoff works.
- Import the Tier 1 animation subset and the pack mannequin per
  `docs/backlog/character-animation-source.md`. Measure the size on disk
  and record it against the LFS budget.
- Re-verify every FL criterion the rebuild touches, at minimum FL-01,
  FL-09, FL-12, FL-13 and FL-14, and update
  `docs/playtests/five-loops-2026-08-22.md` or write a new record.
- Update `docs/CURRENT_STATUS.md`, `docs/SYSTEMDOC.md`,
  `docs/FILESTRUCTURE.md` and `docs/JOURNAL.md` in the same change as what
  they describe.

### Out of Scope

- Art direction. `docs/backlog/art-direction.md` stays gated on the
  non-goal list in `docs/PROJECT_BRIEF.md`.
- The remaining connective spaces. One is the test; seven is the traversal
  proposal at full scale and is not chartered.
- Branching routes, alternate paths and route knowledge. That is
  `docs/backlog/traversal-as-knowledge.md` and it is not activated.
- Consequence, death rules and anything that lets the patrol harm the
  player. Nothing defines what kills the player, and inventing it here
  would be filling a hole in passing.
- Echo, Insight, Chapters 2 to 5, later temporal mechanics.
- Porting Unity C#. The legacy project is design reference under ADR-0001.
- Retiring `C:\code\reWiND`, the license, making the repository public.

### Definition of Done

- A design document under `docs/design/` owns camera grammar and the
  player's movement space, and no rule in it is duplicated elsewhere.
- Every playable region in the rebuilt space declares a camera. A region
  without one is not done.
- The golden path is re-measured and stated, and the loop duration is
  stated against it, so that the ratio is an authored decision rather than
  a consequence of implementation defaults.
- A wasted loop has a measurable cost: the turnstile crossing time is no
  longer determined by the gate cycle alone. Stated as a measurement, not
  as an impression.
- FL-01, FL-09, FL-12, FL-13 and FL-14 are re-verified under the new
  camera and the new scale, with named evidence.
- `docs/JOURNAL.md` has a signed entry and this task is archived.

### Minimum Verification Gates

- [ ] The camera never writes world state, checked by review
- [ ] Every region declares a camera, checked by enumeration
- [ ] Golden path re-measured and recorded next to the loop duration
- [ ] FL-01, FL-09, FL-12, FL-13, FL-14 re-run from a clean save with
      named evidence from `LogRewind`
- [ ] Animation import size measured on disk and recorded
- [ ] Manual link and fence review
- [ ] `git diff --check` clean

## References

- `docs/adr/ADR-0007_camera-and-perspective.md`
- `docs/design/chapter-1-authored.md`
- `docs/playtests/five-loops-2026-08-22.md`
- `docs/backlog/loop-pressure-and-interaction.md`
- `docs/backlog/character-animation-source.md`
- `docs/backlog/traversal-as-knowledge.md`, for what this task deliberately
  does not do
- `C:\Users\zakri\Documents\Unreal Projects\MyProject`, Beach Karate. A
  working constrained side-camera in Unreal, owned by the same author.
  `Source/MyProject/Stage/StageCameraActor.cpp` is a dead zone, a clamp and
  a frame-rate independent ease, and its header states it owns nothing but
  the camera's X position. Reference for the follow mechanism only; it has
  one stage and one camera, and no region model.
- `C:\code\reWiND\Assets\Scripts`, legacy Unity. Design reference, never
  ported.

## Checklist

- [ ] Merge the `REW-0004` identity claim to `main` before moving this
      charter to `Ready`
- [ ] Write the camera grammar design document, including movement space
- [ ] Decide loop duration and space scale together, and amend or supersede
      `chapter-1-authored.md`
- [ ] Implement the camera in C++
- [ ] Rebuild the four areas at the decided scale, each with a camera
- [ ] Add one connective space and prove the handoff
- [ ] Import the Tier 1 animation subset and the mannequin, measure the size
- [ ] Re-run the affected FL criteria and record the evidence
- [ ] Update status, system document, file map
- [ ] Add a signed journal entry and archive this task

## Decisions and Notes

- The scale question is not answered by copying the legacy layout. Read
  from `Chapter1Bootstrap.cs`, the Unity areas were 8x8, 12x14, 20x12 and
  12x10 metres at 12 m spacing, roughly 40 m end to end. The reboot's
  measured golden path is 34 m. **The legacy space had the same defect.**
  It is a starting point for proportions and not for scale.
- Legacy did author connective geometry: three bridges, 4.5 m wide and
  about 8.4 m long, between the four areas. That is evidence that
  transitions were intended as space, and it is thinner than what a
  playable connective space would be.
- The legacy `ThirdPersonCamera.cs` is a full free-orbit camera with mouse
  look, pitch limits and zoom. It is what ADR-0007 rejected, and it is not
  a reference for this task.
- Beach Karate constrains its fighter with `bConstrainToPlane` and a Y
  normal. That is one answer to the movement-space question and not
  automatically the right one here: the patrol paces across Y, and a hard
  plane changes how FL-09 reads.

## Charter Amendment Log

Only non-semantic corrections are allowed after `Ready`.

- none

## Verification

- [ ] Camera reviewed for write access to world state
- [ ] Region enumeration
- [ ] Golden path measurement
- [ ] FL re-run evidence from a clean save
- [ ] Import size on disk
- [ ] Manual link and fence review
- [ ] `git diff --check`
- [ ] Document skipped checks and reasons

## Documentation Updates

- [ ] `docs/CURRENT_STATUS.md`
- [ ] `docs/SYSTEMDOC.md`
- [ ] `docs/JOURNAL.md`
- [ ] `docs/FILESTRUCTURE.md` when structure changes
- [ ] `docs/design/chapter-1-authored.md`, amended or superseded
- [ ] A new design document for camera grammar
- [ ] `docs/backlog/character-animation-source.md`, resolved when imported

## Handoff and Follow-ups

- Current state: Draft. Not yet claimed on `main`, so not yet `Ready`.
- Next recommended step: claim `REW-0004` on `main`, then write the camera
  grammar document, because every other item depends on the rules it
  states.
- Blockers: none.
- Child tasks: none.
- Resume condition: not applicable while Draft.
- Verification gaps: none yet; nothing has been done.
- Open questions: the movement-space question is inside this charter and is
  the first thing the design document must answer.

## Finalize When Complete

- Archive this file under `docs/finished/`.
- Restore this template or populate the next approved task.
- Add a signed `docs/JOURNAL.md` entry.
- If Goal or Definition of Done changed, supersede this task instead of
  rewriting it.
