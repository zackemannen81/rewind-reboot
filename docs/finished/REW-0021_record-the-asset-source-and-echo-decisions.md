# Current Task

Task ID: REW-0021
Parent Task: None
Status: Complete
Owner: Claude
Created: 2026-08-25
Last updated: 2026-08-25
Charter frozen at: 2026-08-25

## Task Summary

Two questions had been blocking task decomposition. The tracked default map
referenced 342 MB of untracked content, so the repository claimed a map it
could not reproduce. Echo had been the first open decision in
`docs/PROJECT_BRIEF.md` since the reboot began.

The owner decided both on 2026-08-25. This task records those decisions where
the project can be built against them, and corrects every document that
described them as open.

## Task Charter

### Goal

Both decisions exist as accepted decision records, and no live document still
describes them as open.

### Primary Deliverable

`ADR-0011_asset-source-and-provenance.md` and `ADR-0012_echo-semantics.md`,
both Accepted and indexed, with the documents that cited the open questions
corrected in the same change.

### In Scope

- The two ADRs, each stating context, decision, alternatives and consequences.
- `docs/adr/README.md` index rows and its open-decision paragraph.
- `docs/PROJECT_BRIEF.md` open-decision list.
- Both backlog proposals and their index rows, marked resolved.
- `docs/CURRENT_STATUS.md` rows describing the ADRs and Echo.

### Out of Scope

- Implementing either decision. ADR-0011's consequence is chartered as
  REW-0022; ADR-0012's consequence is that nothing is built.
- The licence decision and the Unity project's retirement, which stay open.
- Any change to `docs/design/`. Neither decision amends a game rule:
  ADR-0012 was chosen specifically so `loop-and-determinism.md` needs no
  amendment.

### Definition of Done

- Both ADRs exist, are Accepted, and are indexed.
- No live document lists Echo or asset provenance as an open question.
- Both backlog proposals declare what resolved them.
- `git diff --check` is clean and links resolve.

### Minimum Verification Gates

- [x] Documentation review: one owning record per decision, no rule duplicated.
- [x] `git diff --check` clean.

## Checklist

- [x] Write ADR-0011 and ADR-0012.
- [x] Index both in `docs/adr/README.md` and correct its open-decision text.
- [x] Drop Echo from `docs/PROJECT_BRIEF.md`, renumbering what remains.
- [x] Mark both backlog proposals resolved, in the file and in the index.
- [x] Correct `docs/CURRENT_STATUS.md`, including the row that called Echo
      unwritten rather than decided against.
- [x] Journal entry.

## Decisions and Notes

- ADR-0011 chose project-owned and generated assets over committing the
  existing Fab imports. The alternatives are recorded, and the hybrid option
  is named as the most likely future amendment.
- ADR-0012 chose narrative-only Echo. Presentation-only replay is recorded as
  the starting point if the record is ever amended, because it is safe under
  the accepted determinism rule without changing it.
- Neither ADR touches `docs/design/`. That is a property of the choices, not
  an oversight: an Echo that was world would have amended ADR-0002.

## Verification

- [x] Documentation review by hand: both ADRs state context, decision,
      alternatives and consequences; each owns one area; no rule is duplicated.
- [x] `git diff --check` clean.
- [ ] Not run: no build, no automation tests, no editor. Nothing executable
      changed, so none would be evidence about anything.

## Documentation Updates

- [x] `docs/CURRENT_STATUS.md`
- [x] `docs/JOURNAL.md`
- [x] `docs/PROJECT_BRIEF.md`
- [x] `docs/adr/` and its index
- [x] `docs/backlog/` and its index
- [ ] `docs/SYSTEMDOC.md` — no implemented system changed
- [ ] `docs/FILESTRUCTURE.md` — no structural change

## Handoff and Follow-ups

- Current state: complete. Both decisions are recorded and every live document
  agrees with them.
- Next recommended step: REW-0022, which unbinds the authored map from
  untracked content and is ADR-0011's first consequence.
- Blockers: none.
- Child tasks: REW-0022 follows from ADR-0011. ADR-0012 generates no task, by
  design.
- Resume condition: not paused.
- Open questions: the licence, and whether the Unity project is retired.

## Finalize When Complete

- Archived here. `docs/CURRENT_TASK.md` was never populated for this task; the
  charter was written directly into this archive on completion, because the
  work was a single documentation pass performed by the operator.
