# Current Task

Task ID: REW-0034
Parent Task: None
Status: Complete
Owner: codex-gdd
Created: 2026-08-26
Last updated: 2026-08-26
Charter frozen at: 2026-08-26

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
- `docs/design/loop-and-determinism.md`
- `docs/design/world-state-model.md`
- `docs/design/anchors.md`
- `docs/design/save-and-session.md`
- `docs/design/player-messages.md`
- `docs/acceptance/five-loops-test.md`
- `docs/adr/ADR-0002_world-determinism.md`
- `docs/adr/ADR-0003_anchor-semantics.md`
- `docs/adr/ADR-0008_what-an-anchor-is-worth.md`
- `docs/adr/ADR-0009_event-driven-loop-termination.md`
- `docs/adr/ADR-0012_echo-semantics.md`
- `docs/concepts_sandbox/legacy-rewind/old-task-material/task_chapter_2.md`

## Task Summary

Define Chapter 2, "Conduit", as a substantially larger deterministic authored
space whose overlapping transit schedules, predictable misinformation and
fixed antagonist behaviour advance the player's causal knowledge without
letting the world remember earlier loops.

## Task Charter

The charter is editable while status is `Draft` and immutable once status is
`Ready`.

### Goal

Establish testable design authority for Chapter 2 that expands the Chapter 1
knowledge-progression grammar while remaining compliant with ADR-0002 and the
accepted world-state model.

### Primary Deliverable

An accepted `docs/design/chapter-2-authored.md` and a corresponding checkable
Chapter 2 acceptance document under `docs/acceptance/`.

### In Scope

- Chapter fiction, theme, connected spaces and route topology.
- At least eight distinct puzzles or events and at least three independent,
  fully numbered loop-clock schedules.
- A predictable false-information mechanic.
- A deterministic route-1 resolution of the legacy "reactive AI" seed.
- A more-than-five-loop intended learning sequence in which every loop has a
  stated discovery or application.
- Persistence, reset, failure and rewind rules grounded in accepted authority.
- Acceptance criteria, collection-index updates and the minimal project-brief
  clarification needed to distinguish Chapter 2 design authority from Chapter
  2 implementation authorization.
- Open owner questions with a recommendation for each.

### Out of Scope

- Unreal code, content, assets, maps, editor work, builds and playtests.
- Changing Chapter 1 authority or accepted determinism, Echo, Anchor or save
  decisions.
- A second Anchor, Insight, reactive cross-loop world memory or ADR-0013.
- Chapter 2 implementation authorization, final narrative script, final art,
  sound production or camera implementation.
- `docs/CURRENT_STATUS.md`, `docs/FILESTRUCTURE.md`, `docs/SYSTEMDOC.md`,
  `Rewind/`, `docs/baseline/` and imported legacy files.

### Definition of Done

- `docs/design/chapter-2-authored.md` owns the Chapter 2 authored facts and
  satisfies every concrete scale, timing, misinformation and loop requirement.
- Chapter 2 acceptance criteria form a numbered, playable checklist and cite
  their owning authority.
- The ADR-0002 tension is resolved explicitly through fixed deterministic
  antagonist schedules; no ADR-0013 draft is created.
- Every timed rule has a number, schedule intersections are arithmetically
  demonstrated, and all named state fits the accepted buckets and apply order.
- Open questions are few, decision-shaped and carry recommendations.
- Design and acceptance indexes are current; links, fences, ownership and
  whitespace checks pass.
- The task is archived Complete, the current-task template is restored, a
  signed journal entry is added at the top, and a PR is opened against `main`.

### Minimum Verification Gates

- [x] Manually audit every duration, period, phase, window and threshold for
  internal consistency, including each claimed schedule intersection.
- [x] Audit every world-memory and persistence statement against ADR-0002,
  `loop-and-determinism.md` and `world-state-model.md`.
- [x] Confirm every acceptance criterion cites an owning design document and
  every cited relative path resolves.
- [x] Confirm design ownership does not duplicate an existing document.
- [x] Confirm Markdown fences close and `git diff --check` is clean.
- [x] Confirm the diff contains no file under `Rewind/`, `docs/baseline/` or
  imported `docs/concepts_sandbox/legacy-rewind/`.

## References

- The Read First list above.
- `docs/TASK_IDS.md` claim for REW-0034.
- Owner brief supplied with this task on 2026-08-26.

## Checklist

- [x] Read the governing workflow, live status and system documents in full.
- [x] Read the named design, acceptance, ADR and legacy-seed sources in full.
- [x] Confirm REW-0034 is claimed and the requested branch is active.
- [x] Draft and arithmetically audit the Chapter 2 authored design.
- [x] Draft and trace the numbered Chapter 2 acceptance criteria.
- [x] Update collection indexes and clarify the project brief boundary.
- [x] Run all documentation verification gates.
- [x] Complete and archive the charter; restore the template.
- [x] Add the signed newest-first journal entry.
- [x] Commit explicit paths, push and open PR #37 against `main`.

## Decisions and Notes

- Route 1 is selected: Conduit's antagonist only appears to learn. Its scan,
  route-choice and lockdown behaviours are fixed functions of Baseline,
  active Anchors, elapsed loop time and this-loop player actions. It reads no
  loop count and retains no earlier-loop observation.
- The owner's activation authorizes Chapter 2 design authority. The approved
  brief still blocks Chapters 2 to 5 as implementation scope, so this task will
  clarify that distinction without authorizing a build.
- Chapter 1 remains unchanged. Chapter 2 begins at Transit Hub with the
  Chapter 1 knowledge and legal Anchor state supplied by a session; clean
  Chapter 2 acceptance setup will state those prerequisites explicitly.

## Charter Amendment Log

- none

## Verification

- [x] Timing and arithmetic audit: `lcm(72,60,48,180) = 720`; travel-time,
  window, response, signal-sequence, diversion and platform-lead calculations
  all passed explicit PowerShell assertions.
- [x] Determinism, persistence and ownership audit: route 1 uses only elapsed
  time and this-loop input; state maps to accepted buckets; all 25 criteria
  have one cited owner and no design ownership overlaps.
- [x] Links and fences checked across all changed live Markdown: every
  relative target exists and every fence count is even.
- [x] `git diff --check origin/main...HEAD` clean before archival.
- [x] Prohibited-path diff audit clean: only approved `docs/` paths changed;
  no `Rewind/`, baseline, imported legacy, current-status, system-document or
  file-structure path changed.
- [x] Unreal build, editor, PIE and automation not run because the task is
  documentation-only and forbids editor work.

## Documentation Updates

- [x] `docs/design/chapter-2-authored.md`
- [x] `docs/acceptance/chapter-2-conduit-test.md`
- [x] `docs/design/README.md`
- [x] `docs/acceptance/README.md`
- [x] `docs/PROJECT_BRIEF.md`
- [x] `docs/JOURNAL.md`
- [x] `docs/CURRENT_STATUS.md` not edited: no implementation changed
- [x] `docs/SYSTEMDOC.md` not edited: no implementation changed
- [x] `docs/FILESTRUCTURE.md` not edited: prohibited by the owner

## Handoff and Follow-ups

- Current state: Complete. Design and acceptance authority are committed;
  PR #37 is open against `main` and must not be merged by this actor.
- Next recommended step: Owner review the three open questions, especially the
  project-phase decision required before any Chapter 2 implementation task.
- Blockers: None.
- Child tasks: None.
- Resume condition: Not applicable; activate a new task for implementation or
  a change to accepted authority.
- Open questions: Conductor fiction, campaign completion persistence and the
  implementation phase gate, each with a recommendation in
  `docs/design/chapter-2-authored.md`.

## Finalize When Complete

- Archive this file as `docs/finished/REW-0034_chapter-2-conduit-gdd.md`.
- Restore `docs/CURRENT_TASK.md` from `docs/template_CURRENT_TASK.md`.
- Add a signed `docs/JOURNAL.md` entry.
- If Goal or Definition of Done changed, supersede this task instead of
  rewriting it.
