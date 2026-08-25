# Current Task

Task ID: REW-0017
Parent Task: None
Status: Complete
Owner: grok-loop
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
- `docs/adr/ADR-0009_event-driven-loop-termination.md`
- `docs/design/loop-and-determinism.md`
- `docs/backlog/event-driven-rewind-implementation.md`
- Relevant ADRs under `docs/adr/`

## Task Summary

Implement the event-driven rewind ADR-0009 already accepted: the running build
must end loops for causal-contract failure, death and successful Anchor commit,
not on a 240-second timer. A latched one-to-three-second prelude, measured on
the loop clock, must run before loop-start apply on contract failure and on
Anchor commit, with rising interference that also reads elapsed loop time.

## Task Charter

The charter is editable while status is `Draft` and immutable once status is
`Ready`.

### Goal

The running build ends loops for the reasons ADR-0009 accepted, not on a
240-second timer.

### Primary Deliverable

A build in which a causal-contract failure at a named checkpoint, player
death, and a successful Anchor commit each end the loop, with a
perceptible one-to-three-second rewind prelude before loop-start apply on
contract failure and on Anchor commit.

### In Scope

- A causal-checkpoint actor with a named predicate, checked at a
  no-return point rather than continuously.
- A latched rewind that cannot be cancelled by stepping back.
- A rewind prelude of at least 1.0 and at most 3.0 seconds, driven by
  elapsed loop time, before loop-start apply.
- A loop-end reason for Anchor commit; a rejected or redundant commit does
  not end the loop.
- Removal of the automatic 240-second timeout as the default end
  condition. A whole-space deadline remains available where an authored
  situation declares one.
- The loop-break signature the owner asked for: rising static or
  interference during the prelude. It must read elapsed loop time, not
  engine time or frame delta.
- Named automation tests for the prelude bounds, the latch, and the
  Anchor-commit boundary.

### Out of Scope

- Echo. Its semantics are an open decision in docs/PROJECT_BRIEF.md.
- Any change to docs/design/ or docs/adr/ accepted rules. They already
  state the rule; you are implementing it.
- Any player-facing message or dialog system. REW-0018 owns that.
- Renderer configuration, lighting and materials. REW-0019 owns those.
- Any .umap or .uasset binary. REW-0019 owns those.
- docs/TASK_IDS.md. The operator owns it and already claimed REW-0017.

### Definition of Done

- URewindLoopSubsystem exposes end reasons for contract failure, death and
  Anchor commit, and no longer ends a loop on elapsed time by default.
- A checkpoint predicate that fails starts a latched prelude, and moving
  back across the checkpoint does not cancel it.
- The prelude's duration is within [1.0, 3.0] seconds measured on the loop
  clock, and is identical across two runs that share baseline and input.
- A successful first-time Anchor commit ends the loop; a rejected one does
  not.
- The interference signature is visible during the prelude and reads
  elapsed loop time.
- Named Rewind.* automation tests cover the prelude bounds, the latch and
  the commit boundary, and all discoverable Rewind.* tests pass together.

### Minimum Verification Gates

- [x] RewindEditor Win64 Development builds with the editor closed.
- [x] All discoverable Rewind.* automation tests pass together, run headless.
- [x] The journal entry names what was verified and what was not.

## References

- `docs/adr/ADR-0009_event-driven-loop-termination.md`
- `docs/design/loop-and-determinism.md`
- `docs/design/chapter-1-authored.md` (`GroundFuseGate`, `CourtyardGateCommit`)
- `docs/design/anchors.md`
- `docs/backlog/event-driven-rewind-implementation.md`
- `Rewind/Source/Rewind/RewindLoopSubsystem.*`
- `Rewind/Source/Rewind/RewindAnchorBoard.*`

## Checklist

- [x] Create branch `grok/rew-0017-event-driven-rewind` from current `main`.
- [x] Transcribe the frozen charter into `docs/CURRENT_TASK.md`.
- [x] Implement latched rewind, prelude bounds and end-reason surface on
      `URewindLoopSubsystem`; disable default 240-second timeout.
- [x] Implement `ARewindCausalCheckpoint` with the named `GroundFuseGate`
      predicate, evaluated on crossing, not continuously.
- [x] End the loop on first-time Anchor commit from the board; rejected and
      redundant commits do not.
- [x] Implement the loop-break signature so interference rises from elapsed
      loop time during the prelude.
- [x] Add named `Rewind.*` tests for prelude bounds, latch and commit boundary.
- [x] Build RewindEditor Win64 Development editor-closed.
      Evidence: UnrealBuildTool Result: Succeeded, 137.38 s then 17.30 s.
- [x] Run all discoverable `Rewind.*` automation tests headless.
      Evidence: 13/13 Success, `TEST COMPLETE. EXIT CODE: 0`,
      `Rewind/Saved/Logs/Rewind.log` 2026-08-25 03:43.
- [x] Update `docs/CURRENT_STATUS.md`, `docs/SYSTEMDOC.md`, `docs/JOURNAL.md`,
      backlog index, archive the charter and restore the template.

## Decisions and Notes

- A checkpoint after each step or substep is required. Checklist is therefore updated along the work and `CURRENT_STATUS.md` is always updated when changes affect the behavior.
- Record decisions and assumptions within the frozen charter.
- Classify discoveries using `docs/TASK_WORKFLOW.md`.
- Prelude duration default is 2.0 seconds. That is an implementation choice
  inside the accepted [1.0, 3.0] band, not a new design rule. The value is
  clamped to that band at latch time so an out-of-range setting cannot escape
  the accepted bounds.
- `LoopDurationSeconds` remains in developer settings as the authored
  whole-space deadline value. It is applied only when `bUseWholeSpaceDeadline`
  is true. Chapter 1 does not declare a whole-space deadline, so the default
  is false. This is the remaining availability ADR-0009 requires, not a
  retained automatic timeout.
- `GroundFuseGate` is the named checkpoint already owned by
  `docs/design/chapter-1-authored.md`. Its predicate is
  `GROUND_FUSE_POWERED || ANCHOR_courtyard_gate_open_ACTIVE`. Ground-floor
  power is the fuse seated in the existing courtyard/ground-floor socket. The
  socket enum is not renamed: that file is not owned by this task, and the
  backlog rename is outside this charter.
- The GroundFuseGate volume is spawned in C++ at the proof-layout hall-to-
  courtyard seam (X = 4110). Maps tagged `Rewind.SkipProofLayout` do not
  receive it, because they have no courtyard. No `.umap` is edited.
- Death still ends the loop immediately. ADR-0009 says death need not wait
  for the mechanical prelude.
- First-time Anchor commit is detected without editing
  `URewindSessionSubsystem` (not owned): `TryCommitCourtyardGateAnchor`
  returns true and `HasPendingAnchorCommit()` is true only on a new pending
  write. Redundant already-active commits return true with pending false.
- The interference signature is a runtime actor plus post-process blend
  driven by elapsed loop time. It is not a player-facing message system and
  not a materials/lighting/config change.

## Charter Amendment Log

Only non-semantic corrections are allowed after `Ready`.

-none

## Verification

- [x] RewindEditor Win64 Development, editor closed. First full makefile
      build succeeded in 137.38 s. Incremental rebuild after the commit-boundary
      test fix succeeded in 17.30 s.
- [x] Headless `Automation RunTests Rewind`. Found 13 tests; all Success;
      `**** TEST COMPLETE. EXIT CODE: 0 ****` in
      `Rewind/Saved/Logs/Rewind.log` (2026-08-25 03:43). Named tests:
      `Rewind.Loop.Prelude.Bounds`, `Rewind.Loop.Prelude.Latch`,
      `Rewind.Loop.AnchorCommit.Boundary`,
      `Rewind.Loop.BreakSignature.ReadsLoopTime`.
- [x] Skipped: packaged build; live PIE GroundFuseGate crossing; live first-time
      Anchor commit on the proof map; idle observation past 240 s; FL-17/FL-18
      playtest. The default authored map has no courtyard, so the checkpoint is
      not present there. Final static/sound/lighting is REW-0019.

## Documentation Updates

- [x] `docs/CURRENT_STATUS.md`
- [x] `docs/SYSTEMDOC.md`
- [x] `docs/JOURNAL.md`
- [x] `docs/FILESTRUCTURE.md` when structure changes — no tree change; new
      sources live under existing `Rewind/Source/Rewind/`
- [x] ADRs when long-lived decisions change — none; ADR-0009 already accepted
- [x] `docs/backlog/event-driven-rewind-implementation.md` and its index row

## Handoff and Follow-ups

- Current state: implementation complete; charter ready to archive.
- Next recommended step: operator review and merge. A later task may place
  GroundFuseGate into a map that actually contains the courtyard, and run
  FL-17/FL-18 in PIE.
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
