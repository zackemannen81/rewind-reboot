# Current Task

Task ID: REW-0006
Parent Task: None
Supersedes: REW-0004
Status: Superseded
Owner: Claude
Created: 2026-08-23
Last updated: 2026-08-23
Charter frozen at: 2026-08-23
Superseded by: REW-0007, 2026-08-23

## Read First

- `AGENTS.md`
- `docs/TASK_WORKFLOW.md`
- `docs/PROJECT_BRIEF.md`
- `docs/CURRENT_STATUS.md`
- `docs/SYSTEMDOC.md`
- `docs/design/chapter-1-authored.md`
- `docs/design/camera-and-movement.md`
- `docs/design/anchors.md`
- `docs/design/world-state-model.md`
- `docs/adr/ADR-0007_camera-and-perspective.md`
- `docs/adr/ADR-0008_what-an-anchor-is-worth.md`
- `docs/acceptance/five-loops-test.md`
- `docs/concept/README.md`
- `docs/finished/REW-0004_chapter-1-spatial-and-camera-foundation.md`
- `docs/finished/REW-0005_rewind-consequence-and-chapter-1-chain.md`

## Task Summary

Build the space and the mechanics that make the Chapter 1 chain playable:
the radio's cost, the carried fuse with two sockets, the lift and the
stairs, each region framed by an authored camera. Do not invent rules.
REW-0005 wrote them.

## Task Charter

The charter is editable while status is `Draft` and immutable once status
is `Ready`.

### Why this task exists and REW-0004 does not

REW-0004 held the same goal and was superseded rather than resumed. Its
frozen out-of-scope list excluded branching routes, and REW-0005 decided a
lift-or-stairs choice, which is one. Reading that exclusion narrowly to make
it fit would be the tightening a freeze exists to prevent.

REW-0004's camera work is merged and is not repeated here.

### Goal

An authored space in which the three loops of `chapter-1-authored.md`
actually happen: information costs a loop, matter costs a route, and a held
change makes a route possible.

### Primary Deliverable

The FiveLoops space rebuilt to hold the chain, with the radio sequence, the
carried fuse, the two sockets, the lift and the stairs implemented, every
region declaring a camera, and a playtest record showing the three loops.

### In Scope

- Implement the radio as a loop-clocked broadcast: a 45-second sequence
  repeating every 60, with channel selection, and `radio_code_7312`
  becoming true at the end of a sequence the player heard throughout.
- Implement the fuse as a carried LoopWorld object with two exclusive
  sockets, per `chapter-1-authored.md` and `world-state-model.md`.
- Implement the lift, which runs only while the fuse is in the building
  socket, and the stairs, which always work.
- Build the space the chain needs: Apartment 4C, the landing, the stairwell
  and lift shaft, the courtyard, the street and Transit Hub. Every region
  declares a camera per `camera-and-movement.md`.
- Meet the two timing rules by construction and then by measurement: the
  stairs cost at least one turnstile period more than the lift, and a naive
  run reaches the turnstile at least one turnstile period after a learned
  one.
- Import the Tier 1 animation subset and the pack mannequin per
  `docs/backlog/character-animation-source.md`, and measure the size on
  disk against the LFS budget.
- Demonstrate the three loops and re-verify every FL criterion the rebuild
  touches, at minimum FL-01, FL-04, FL-06, FL-07, FL-09, FL-12, FL-13 and
  FL-14. Write a new playtest record; the existing one belongs to a
  different build and different criteria.
- Settle the `fuse-box-detail.png` labelling question recorded in
  `docs/concept/README.md`, either by moving the building socket or by
  changing what the art says.
- Update `docs/CURRENT_STATUS.md`, `docs/SYSTEMDOC.md`,
  `docs/FILESTRUCTURE.md` and `docs/JOURNAL.md` in the same change as what
  they describe.

### Out of Scope

- Inventing or changing game rules. REW-0005 wrote them, and if one cannot
  be built, pause and charter a child rather than filling the hole.
- Art direction. `docs/backlog/art-direction.md` stays gated on the
  non-goal list in `docs/PROJECT_BRIEF.md`. The concept images are targets
  to aim at, not a licence to start the art phase.
- Route knowledge as a progression system, alternate routes beyond the one
  the chain requires, and shortcuts. That is
  `docs/backlog/traversal-as-knowledge.md` and it is not activated.
- Consequence and death. The stairwell drone in the imported GDD is not
  built here, because nothing defines what killing the player means.
- Echo, Insight, Chapters 2 to 5, later temporal mechanics.
- Porting Unity C#.

### Definition of Done

- The three loops of `chapter-1-authored.md` are demonstrated from a clean
  save, with named evidence from `LogRewind` for each.
- Loop B provably cannot reach Transit Hub, and Loop C provably can, and
  the difference is the Anchor rather than the player's speed.
- Both timing rules are satisfied by measurement, not by argument.
- Every playable region declares a camera, checked by enumeration.
- `courtyard_gate_open` passes ADR-0008's test in the built chain: the
  resource it releases is named and its two uses are named.
- The animation import size is measured and recorded.
- A new playtest record exists, and `docs/CURRENT_STATUS.md` no longer says
  the rules are ahead of the build.
- `docs/JOURNAL.md` has a signed entry and this task is archived.

### Minimum Verification Gates

- [ ] Three loops demonstrated from a clean save with `LogRewind` evidence
- [ ] Loop B fails to reach the hub, and the reason is the fuse and not the
      clock running out for some other cause
- [ ] Stairs minus lift, measured, at least one turnstile period
- [ ] Naive minus learned, measured, at least one turnstile period
- [ ] Every region declares a camera
- [ ] Animation import size on disk
- [ ] Manual link and fence review
- [ ] `git diff --check` clean

## References

- `docs/design/chapter-1-authored.md`, which owns every rule this task
  builds
- `docs/concept/README.md`, target images, never rules
- `docs/backlog/character-animation-source.md`
- `docs/playtests/five-loops-2026-08-22.md`, evidence for a different build
  and for criteria that have since been amended

## Checklist

- [x] Merge the `REW-0006` identity claim to `main` before moving this
      charter to `Ready`
- [x] Implement the radio sequence and channels
- [x] Implement the carried fuse and the two sockets
- [ ] Implement the lift and the stairs
- [ ] Build 4C, landing, stairwell and shaft, courtyard, street, hub
- [ ] Declare a camera for every region
- [ ] Import the Tier 1 animations and the mannequin, measure the size
- [ ] Measure both timing rules and adjust the layout until they hold
- [x] Settle the fuse box labelling question: design flow wins, style stands
- [ ] Demonstrate the three loops and write the playtest record
- [x] Update status and system document; the file map did not change
- [x] Add a signed journal entry and archive this task as Superseded

## Decisions and Notes

- The rules are ahead of the build on purpose. This task closes the gap and
  writes no new rules.
- The concept images arrived after the rules and agree with them, which is
  reassuring and not evidence. `stairwell-4c-landing.png` shows the
  connective space `chapter-1-authored.md` had already decided.
- One discrepancy is already known: the fuse box in the concept art is
  labelled with an apartment's own circuits rather than the lift and the
  courtyard. It is in scope to settle and should be settled deliberately.
- This task is large because the milestone is. If a discovery blocks it,
  pause and charter a child rather than dropping part of the chain.

## Charter Amendment Log

Only non-semantic corrections are allowed after `Ready`.

- none

## Verification

- [ ] Three-loop demonstration from a clean save
- [ ] Loop B failure attributed to the fuse
- [ ] Both timing measurements
- [ ] Region enumeration
- [ ] Import size on disk
- [x] Manual link and fence review
- [x] `git diff --check`
- [x] Document skipped checks and reasons

## Documentation Updates

- [x] `docs/CURRENT_STATUS.md`
- [x] `docs/SYSTEMDOC.md`
- [x] `docs/JOURNAL.md`
- [x] `docs/FILESTRUCTURE.md` reviewed; no map change required
- [ ] A new record under `docs/playtests/`
- [ ] `docs/backlog/character-animation-source.md`, resolved when imported
- [x] `docs/concept/README.md`, fuse box question settled

## Handoff and Follow-ups

- Current state: **Superseded 2026-08-23 by REW-0007.** The frozen scope
  requires a 45-second sequence repeating every 60 seconds. The played
  implementation established that 20 seconds repeating every 50 is the
  better rule, and the owner confirmed that rule. Continuing here would
  silently rewrite frozen scope, so the task is superseded instead.
- Delivered before supersession: the four-channel, loop-clocked 20/50 radio;
  individual digit delivery at phases 4, 9, 14 and 19; the full-listen
  knowledge grant; one carried LoopWorld fuse; two exclusive sockets; the
  generator's courtyard-socket dependency; the 240-second loop duration in
  code; and the fuse-box labelling decision.
- Next recommended step: merge the supersession and `REW-0007` identity claim
  to `main`, then move the successor charter from `Draft` to `Ready` on its
  own branch and implement the lift and stairs.
- Blockers: none in the product rules. REW-0007 cannot move to `Ready` until
  its identity claim lands on `main`.
- Child tasks: none.
- Resume condition: `REW-0007` is claimed on `main`.
- Verification gaps: no new playtest record exists; the three-loop chain,
  lift, stairs, timing relationships, region enumeration and animation import
  were not completed. The branch contains played observations in commit
  history, not a durable playtest artifact.
- Open questions: none. The owner confirmed 20/50 after the played test. The
  fuse-box question remains settled in `docs/concept/README.md`: the design's
  flow wins and the image's style stands.

## Finalize When Complete

- Archive this file under `docs/finished/`.
- Restore this template or populate the next approved task.
- Add a signed `docs/JOURNAL.md` entry.
- If Goal or Definition of Done changed, supersede this task instead of
  rewriting it.
