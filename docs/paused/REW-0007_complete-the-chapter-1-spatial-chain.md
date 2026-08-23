# Paused Task

Task ID: REW-0007
Parent Task: None
Supersedes: REW-0006
Status: Paused
Owner: Codex
Created: 2026-08-23
Last updated: 2026-08-23
Charter frozen at: 2026-08-23

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
- `docs/design/anchors.md`
- `docs/design/world-state-model.md`
- `docs/adr/ADR-0007_camera-and-perspective.md`
- `docs/adr/ADR-0008_what-an-anchor-is-worth.md`
- `docs/acceptance/five-loops-test.md`
- `docs/finished/REW-0006_chapter-1-space-the-chain-made-playable.md`

## Task Summary

Complete the authored Chapter 1 chain after REW-0006 established the radio
and contested fuse but had to be superseded when its frozen 45/60 radio rule
was replaced by the played and owner-confirmed 20/50 rule.

## Task Charter

The charter is frozen. Goal, deliverable, scope, out-of-scope, definition of
done and minimum verification gates do not change while this task is paused.

### Goal

An authored space in which the three loops of `chapter-1-authored.md`
actually happen: information costs a loop, matter costs a route, and a held
change makes a route possible.

### Primary Deliverable

The FiveLoops space rebuilt to hold the accepted 20/50 radio sequence, the
carried fuse, the two sockets, the lift and the stairs, with every region
declaring a camera and a playtest record demonstrating the three loops.

### In Scope

- Preserve and integrate REW-0006's implemented radio and fuse behaviour:
  a 20-second sequence repeating every 50 seconds, digits at phases 4, 9,
  14 and 19, one carried LoopWorld fuse and two exclusive sockets.
- Implement the lift, which runs only while the fuse is in the building
  socket, and the stairs, which always work.
- Build Apartment 4C, the landing, the stairwell and lift shaft, the
  courtyard, the street and Transit Hub. Every region declares a camera per
  `camera-and-movement.md`.
- Meet the timing rules by construction and measurement: the stairs cost at
  least one turnstile period more than the lift, and a naive run reaches the
  turnstile at least one turnstile period after a learned one.
- Import the Tier 1 animation subset and the pack mannequin per
  `docs/backlog/character-animation-source.md`, and measure its size against
  the LFS budget.
- Demonstrate the three loops and re-verify every FL criterion the rebuild
  touches, at minimum FL-01, FL-04, FL-06, FL-07, FL-09, FL-12, FL-13 and
  FL-14, in a new playtest record.
- Update `docs/CURRENT_STATUS.md`, `docs/SYSTEMDOC.md`,
  `docs/FILESTRUCTURE.md` and `docs/JOURNAL.md` in the same change as what
  they describe.

### Out of Scope

- Inventing or changing game rules. The accepted design documents own them;
  if one cannot be built, pause and charter a bounded child.
- Art direction. The concept images are targets, not permission to begin the
  art phase.
- Alternate routes beyond the lift-or-stairs choice required by the chain,
  route knowledge as a progression system, and shortcuts.
- Consequence and death. Nothing defines what killing the player means.
- Echo, Insight, Chapters 2 to 5, later temporal mechanics, or Unity C#.

### Definition of Done

- The three loops of `chapter-1-authored.md` are demonstrated from a clean
  save with named `LogRewind` evidence.
- Loop B provably cannot reach Transit Hub, Loop C provably can, and the
  difference is the Anchor rather than player speed.
- Both timing rules are satisfied by measurement.
- Every playable region declares a camera, checked by enumeration.
- `courtyard_gate_open` passes ADR-0008's resource-and-two-uses test in the
  built chain.
- The animation import size is measured and recorded.
- A new playtest record exists, the durable status documents describe the
  build, `docs/JOURNAL.md` has a signed entry and this task is archived.

### Minimum Verification Gates

- [ ] Three loops demonstrated from a clean save with `LogRewind` evidence
- [ ] Loop B failure attributed to the fuse and route, not another timeout
- [ ] Stairs minus lift measured at one turnstile period or more
- [ ] Naive minus learned measured at one turnstile period or more
- [ ] Every region declares a camera
- [ ] Animation import size measured on disk
- [ ] Manual link and fence review
- [ ] `git diff --check` clean

## References

- `docs/design/chapter-1-authored.md`
- `docs/design/camera-and-movement.md`
- `docs/design/anchors.md`
- `docs/design/world-state-model.md`
- `docs/adr/ADR-0007_camera-and-perspective.md`
- `docs/adr/ADR-0008_what-an-anchor-is-worth.md`
- `docs/acceptance/five-loops-test.md`
- `docs/finished/REW-0006_chapter-1-space-the-chain-made-playable.md`
- `docs/backlog/character-animation-source.md`

## Checklist

- [x] Merge the `REW-0007` identity claim to `main`
- [x] Move this charter to `Ready` and freeze it after the claim lands
- [ ] Implement the lift and the stairs
- [ ] Build 4C, landing, stairwell and shaft, courtyard, street and hub
- [ ] Declare a camera for every region
- [ ] Import the Tier 1 animations and mannequin; measure the size
- [ ] Measure both timing rules and adjust the layout until they hold
- [ ] Demonstrate the three loops and write the playtest record
- [ ] Update status, system document and file map
- [ ] Add a signed journal entry and archive this task

## Decisions and Notes

- The 20/50 radio rule is accepted. REW-0006 was superseded rather than
  editing its frozen 45/60 scope.
- REW-0006's radio and fuse implementation is inherited work, not work to
  repeat. This task completes and verifies the chain around it.
- The rules are still ahead of the complete build. This task closes that gap
  and writes no new rules.
- Agent-driven PIE input and repeatable editor acceptance are a tooling
  prerequisite, routed to bounded child REW-0008 rather than added to this
  charter.

## Charter Amendment Log

Only non-semantic corrections are allowed after `Ready`.

- none

## Verification

- [ ] Three-loop demonstration from a clean save
- [ ] Loop B failure attributed to the fuse and route
- [ ] Both timing measurements
- [ ] Region enumeration
- [ ] Import size on disk
- [ ] Manual link and fence review
- [ ] `git diff --check`
- [ ] Document skipped checks and reasons

## Documentation Updates

- [ ] `docs/CURRENT_STATUS.md`
- [ ] `docs/SYSTEMDOC.md`
- [ ] `docs/JOURNAL.md`
- [ ] `docs/FILESTRUCTURE.md` when structure changes
- [ ] A new record under `docs/playtests/`
- [ ] `docs/backlog/character-animation-source.md` when resolved

## Handoff and Follow-ups

- Current state: Paused before implementation. The charter is frozen and the
  identity claim is on `main`.
- Next recommended step: complete bounded child REW-0008, restore this file
  unchanged to `docs/CURRENT_TASK.md`, then implement lift and stairs.
- Blockers: agents cannot yet supply PIE player input or run repeatable editor
  acceptance without a human at the keyboard.
- Child tasks: REW-0008, agent-driven PIE and editor automation.
- Resume condition: REW-0008 is complete and its editor automation is verified.
- Verification gaps: all implementation and playtest gates remain open.
- Open questions: none.

## Finalize When Complete

- Archive this file under `docs/finished/`.
- Restore the template or populate the next approved task.
- Add a signed `docs/JOURNAL.md` entry.
- If Goal or Definition of Done changes, supersede this task instead of
  rewriting it.
