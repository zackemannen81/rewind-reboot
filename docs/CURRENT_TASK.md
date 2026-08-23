# Current Task

Task ID: REW-0009
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
- `docs/adr/ADR-0002_world-determinism.md`
- `docs/adr/ADR-0003_anchor-semantics.md`
- `docs/adr/ADR-0008_what-an-anchor-is-worth.md`
- `docs/design/loop-and-determinism.md`
- `docs/design/chapter-1-authored.md`
- `docs/acceptance/five-loops-test.md`

## Task Summary

Decide whether RE:WIND's default loop ends on a global duration or on authored
causal events. The owner has supplied a Chapter 1 example: crossing the
building threshold without first powering the required ground-floor fuse box
should telegraph failure for one to three seconds and then return the player to
4C. Time remains available where a situation itself is timed, but must not
silently govern exploration.

The decision precedes further level construction because the current accepted
Chapter 1 chain and implementation use a 240-second timeout to make Loop B
fail. Rebuilding the owner's spatial blockout before deciding what ends that
loop would encode the old rule into a new level.

## Task Charter

The charter is editable while status is `Draft` and immutable once status is
`Ready`.

### Goal

Replace the global loop timer as RE:WIND's default reset rule with a testable,
deterministic event-driven rewind policy while preserving local timed
situations and the three-loop Chapter 1 knowledge chain.

### Primary Deliverable

An accepted loop-termination ADR, with every affected owning product, design,
acceptance and current-state document made consistent with it.

### In Scope

- Decide the allowed loop-end causes and the role of elapsed loop time.
- Decide how an authored causal contract detects failure and telegraphs rewind.
- Restate the Chapter 1 fuse, Anchor and route chain without relying on a global
  timeout.
- Amend affected live authority and acceptance criteria.
- Record that the current timer-driven build does not yet implement the new
  rule and hand off the bounded implementation work.

### Out of Scope

- C++, Blueprint, `.umap`, camera or blockout edits.
- Moving, committing or modifying the owner's Handmade level.
- Final rewind visual effects, final audio or environment art.
- Rebalancing patrol, radio or turnstile timings except where consistency with
  the end-condition decision requires wording changes.
- PIE, packaged-build or player-experience verification.

### Definition of Done

- An accepted ADR states why event-driven rewind is the default, what may still
  use time, and how the policy remains deterministic.
- `loop-and-determinism.md` owns one consistent set of loop-end rules.
- `chapter-1-authored.md` states testable event gates that preserve Loops A, B
  and C without a global duration.
- The Five Loops acceptance surface no longer requires timeout as the normal
  loop end and records its amendment without renumbering identifiers.
- Current-state documents distinguish accepted rules from the still
  timer-driven implementation.
- Documentation verification passes and no gameplay or level file changes.

### Minimum Verification Gates

- [ ] Search live authority and acceptance for conflicting global-duration or
  timer-only end-condition claims.
- [ ] Review ownership, relative links and fenced blocks manually.
- [ ] `git diff --check` is clean.
- [ ] Confirm the diff contains no C++, Blueprint, `.uasset` or `.umap` change.

## References

- `docs/PROJECT_BRIEF.md`
- `docs/adr/ADR-0002_world-determinism.md`
- `docs/adr/ADR-0003_anchor-semantics.md`
- `docs/adr/ADR-0008_what-an-anchor-is-worth.md`
- `docs/design/loop-and-determinism.md`
- `docs/design/anchors.md`
- `docs/design/chapter-1-authored.md`
- `docs/acceptance/five-loops-test.md`
- `docs/playtests/chapter-1-three-loops-2026-08-23.md`

## Checklist

- [x] Inspect the live Handmade level, its cameras and the accepted timer-driven
  implementation boundary without changing them.
- [ ] Write and accept the loop-termination ADR.
- [ ] Amend product, loop, Chapter 1 and acceptance authority.
- [ ] Update current reality, durable system description and repository map.
- [ ] Verify the complete documentation change.
- [ ] Archive REW-0009, restore the task template and add the signed journal
  entry.

## Decisions and Notes

- The owner-authored Handmade level was inspected live in Unreal before the
  charter froze. It has four stair flights, a substantially taller vertical
  stack than the current proof, and three 35 mm CineCameraActors for 4C, the
  upper hallway and the stairwell. It remains owner work and is not changed or
  committed by this decision task.
- ADR-0002 does not require supersession: event gates remain deterministic
  functions of baseline, Anchors, elapsed time and this-loop player actions.
- A checkpoint after each step or substep is required. Checklist is therefore
  updated along the work and `CURRENT_STATUS.md` is always updated when changes
  affect behaviour.
- Record decisions and assumptions within the frozen charter.
- Classify discoveries using `docs/TASK_WORKFLOW.md`.

## Charter Amendment Log

- none

## Verification

- [ ] Conflicting-rule search.
- [ ] Manual authority, link and fence review.
- [ ] `git diff --check`.
- [ ] Diff-type audit.
- Unreal build, PIE and automation are intentionally not run: this task changes
  authority only and explicitly leaves implementation to a successor.

## Documentation Updates

- [ ] `docs/CURRENT_STATUS.md`
- [ ] `docs/SYSTEMDOC.md`
- [ ] `docs/JOURNAL.md`
- [ ] `docs/FILESTRUCTURE.md` when structure changes
- [ ] ADRs when long-lived decisions change

## Handoff and Follow-ups

- Current state: authority task in progress; build still timer-driven.
- Next recommended step: implement the accepted event gates against the
  owner-authored Handmade spatial source in a separately chartered task.
- Blockers: none for the decision task.
- Child tasks: none.
- Resume condition: not applicable.
- Open questions: final presentation of static, audio collapse and visual
  artifacts remains implementation/art scope beyond the mechanical prelude.

## Finalize When Complete

- Archive this file under `docs/finished/`.
- Restore this template or populate the next approved task.
- Add a signed `docs/JOURNAL.md` entry.
- If Goal or Definition of Done changed, supersede this task instead of
  rewriting it.
