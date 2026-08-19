# Current Task

Task ID: REW-0002
Parent Task: None
Status: Complete
Owner: Grok
Created: 2026-08-19
Last updated: 2026-08-19
Charter frozen at: 2026-08-19

## Read First

- `AGENTS.md`
- `docs/TASK_WORKFLOW.md`
- `docs/PROJECT_BRIEF.md`
- `docs/CONTRIBUTING.md`
- `docs/CURRENT_STATUS.md`
- `docs/SYSTEMDOC.md`
- `docs/JOURNAL.md`
- `docs/FILESTRUCTURE.md`
- `docs/adr/ADR-0001_engine-unreal-engine-5.md`
- `docs/backlog/five-loops-test.md`
- `docs/concepts_sandbox/legacy-rewind/design-conflicts.md`
- `docs/concepts_sandbox/legacy-rewind/code-inventory.md`
- `docs/finished/REW-0001_establish-canonical-project-state.md`

## Task Summary

Write the authority the Five Loops Test depends on: the decisions, the design
rules, and the acceptance criteria. Do not implement the test, and do not
create an Unreal project.

The backlog proposal's twelve-step sequence is input, not acceptance. This
task turns that sequence into testable criteria owned by named documents, and
settles the open decisions those criteria cannot be written without.

## Task Charter

The charter is editable while status is `Draft` and immutable once status is
`Ready`. The `REW-0002` identity is on `main` as of `9d53ebf`.

### Goal

Make the Five Loops Test implementable without inventing rules during
implementation. A later task must be able to build against written decisions,
design documents and acceptance criteria, and nothing else.

### Primary Deliverable

Accepted ADRs and owning design documents for the rules the test depends on,
plus `docs/acceptance/five-loops-test.md` whose every criterion is testable
and cites exactly one owning design document.

### In Scope

- Decide world determinism for this project, as ADR-0002, answering conflict
  C1: what is identical across loops, and what may change.
- Decide Anchor semantics for the first product proof, as ADR-0003, answering
  conflicts C3 and C4 far enough to say what the first Anchor overrides, when
  it is created, what it costs, and whether Insight exists in this proof.
- Decide the save and session model far enough for a clean save, as ADR-0004:
  what persists across loops, what persists across sessions, and how a
  playtest starts from nothing.
- Write design documents, one ownership area each, covering at least:
  loop and determinism rules; world-state reset and apply order; the authored
  Chapter 1 facts the proof needs (radio code, fuse/generator/gate, patrol,
  turnstile). Do not write documents for areas the proof does not need.
- Write `docs/acceptance/five-loops-test.md` from the backlog sequence.
  Keep a step only if it is testable against a written rule. Drop or defer a
  step in that document, with a reason, rather than leaving it vague.
- Restate each accepted decision's chosen rule in its owning design document
  in the same change. The ADR records the choice; the design document is
  the rule.
- Update `docs/PROJECT_BRIEF.md` so decided items leave the open-decisions
  list.
- Update `docs/CURRENT_STATUS.md`, `docs/FILESTRUCTURE.md`, `docs/adr/README.md`,
  `docs/design/README.md` and `docs/acceptance/README.md`.

### Out of Scope

- Any Unreal project, C++, Blueprint, map, asset or implementation.
- Deciding Echo semantics, asset storage, the license, or retirement of the
  Unity project.
- Chapters 2 to 5, later temporal mechanics, a general-purpose time engine.
- Environment polish, final art, sound design beyond what an acceptance
  criterion names as a signal, a narrative script.
- Implementing or copying legacy GDD text as a rule. A restatement is a
  rewrite with this project's reasoning.
- Editing `docs/baseline/` or imported files under
  `docs/concepts_sandbox/legacy-rewind/`.

### Definition of Done

- ADR-0002, ADR-0003 and ADR-0004 are Accepted and indexed.
- Every acceptance criterion cites exactly one owning design document, and
  that document states the rule in testable terms.
- No two design documents define the same rule.
- Echo, asset storage and the license remain listed as open in
  `docs/PROJECT_BRIEF.md`.
- Determinism, Anchor semantics and the clean-save model are no longer listed
  as open.
- `docs/CURRENT_STATUS.md` no longer says design authority and acceptance
  criteria do not exist.
- A later implementation task can be chartered without first inventing a
  game rule.
- `docs/JOURNAL.md` has a signed entry and this task is archived.

### Minimum Verification Gates

- [x] Every acceptance criterion maps to exactly one owning design document
- [x] Every rule an acceptance criterion depends on is stated in testable
      terms in that owning document
- [x] No two design documents own the same rule
- [x] Open decisions that this charter must not close remain open
- [x] Manual link and fence review
- [x] `git diff --check` clean

## References

- `docs/backlog/five-loops-test.md`
- `docs/PROJECT_BRIEF.md`
- `docs/concepts_sandbox/legacy-rewind/design-conflicts.md` (C1, C3, C4, C6,
  C7, C8; not authority)
- `docs/concepts_sandbox/legacy-rewind/code-inventory.md` (not authority)
- `docs/adr/ADR-0001_engine-unreal-engine-5.md`

## Checklist

- [x] Merge the `REW-0002` identity claim to `main` before moving this
      charter to `Ready`
- [x] Write ADR-0002, world determinism
- [x] Write ADR-0003, Anchor semantics for the first proof
- [x] Write ADR-0004, save and session model for a clean save
- [x] Write the design documents named by ownership, restating the chosen
      rules
- [x] Write `docs/acceptance/five-loops-test.md`
- [x] Cross-check criteria against owning documents, both directions
- [x] Update brief, status, file map and collection indexes
- [x] Add a signed journal entry and archive this task

## Decisions and Notes

- The backlog proposal's outcome was a playable proof. This activation is
  narrower: authority only. Implementation is a later task, chartered against
  these documents. Folding implementation into this charter would repeat the
  previous project's failure of marking systems done while the rules were
  still being invented.
- Echo is not required by the candidate sequence and stays out. Adding it
  later is a scope event.
- Asset storage stays open because this task produces no `.uasset`.
- Insight is not a separate ADR. ADR-0003 must say whether it exists in this
  proof, because Anchor cost depends on it.
- Loop length, patrol timings and the turnstile window are design parameters,
  owned by the authored-content document, not ADRs, unless a choice would
  constrain work beyond Chapter 1.
- Legacy numbers (7-minute loop, code `7312`, 30 s / ~2.5 s turnstile,
  20-second guard window) are sandbox claims. They become rules only if a
  design document restates them. The inventory already shows the Unity
  scripts do not match those numbers.

## Charter Amendment Log

Only non-semantic corrections are allowed after `Ready`.

-none

## Verification

- [x] Criteria-to-design cross-check, both directions
- [x] Ownership uniqueness
- [x] Manual link and fence review
- [x] `git diff --check`
- [x] Document skipped checks and reasons

Ran: FL-01 to FL-16 each name one owner; owners are
`chapter-1-authored.md` (FL-01, FL-04, FL-06, FL-09, FL-12, FL-13, FL-14),
`loop-and-determinism.md` (FL-02, FL-03, FL-08),
`save-and-session.md` (FL-05, FL-15, FL-16),
`world-state-model.md` (FL-07),
`anchors.md` (FL-10, FL-11). Design `Owns` / `Does not own` lines do not
overlap. Echo, asset storage and license remain in
`docs/PROJECT_BRIEF.md` open decisions. Relative links and fenced blocks
checked on live docs under `docs/` excluding `baseline/` and imported
legacy files. `git diff --check` clean.

Did not run: any Unreal or Unity session, any playtest, any build. This
charter produces no implementation.

## Documentation Updates

- [x] `docs/CURRENT_STATUS.md`
- [x] `docs/SYSTEMDOC.md` if system shape changes
- [x] `docs/JOURNAL.md`
- [x] `docs/FILESTRUCTURE.md`
- [x] `docs/PROJECT_BRIEF.md`
- [x] `docs/adr/README.md`
- [x] `docs/design/README.md`
- [x] `docs/acceptance/README.md`
- [x] `docs/backlog/five-loops-test.md` and `docs/backlog/README.md`

## Handoff and Follow-ups

- Current state: Complete. ADR-0002, ADR-0003 and ADR-0004 are Accepted.
  Design and FL-01 to FL-16 are written. None of the criteria have been
  played.
- Next recommended step: charter the implementation task against
  `docs/acceptance/five-loops-test.md`. Asset storage must be decided
  before the first `.uasset`.
- Blockers: none for this charter. `main` still carries the protocol
  bootstrap besides `TASK_IDS.md`; RE:WIND content remains on `dev/grok-01`
  and `plan/projectsetup`. That does not block writing the ADRs.
- Child tasks: none.
- Resume condition: not applicable.
- Open questions: none inside this charter. Echo, asset storage and license
  remain project-level open decisions and must stay that way.

## Finalize When Complete

- Archive this file under `docs/finished/`.
- Restore this template or populate the next approved task.
- Add a signed `docs/JOURNAL.md` entry.
- If Goal or Definition of Done changed, supersede this task instead of
  rewriting it.
