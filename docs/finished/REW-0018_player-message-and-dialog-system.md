# Current Task

Task ID: REW-0018
Parent Task: None
Status: Complete
Owner: grok-message
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
- `docs/design/camera-and-movement.md`
- `docs/design/chapter-1-authored.md`
- `docs/backlog/art-direction.md` (P7 and Stage 7)
- Relevant ADRs under `docs/adr/`

## Task Summary

Every line of player-facing text in this game is currently
`GEngine->AddOnScreenDebugMessage`. That is an engine debug instrument, not a
user interface. `docs/backlog/art-direction.md` states in writing that the
debug overlay is not UI and that no task may quietly promote it. This task
replaces it with a project-owned message system whose copy is data, whose
presentation is legible against the authored frame, and whose queue is covered
by named automation tests.

## Task Charter

The charter is editable while status is `Draft` and immutable once status is
`Ready`.

### Goal

Player-facing text comes from a project-owned message system, not from the
engine's debug overlay.

### Primary Deliverable

A message and dialog system that owns every line the player reads in the
authored slice, with the copy held as data rather than inline literals.

### In Scope

- A runtime message subsystem with a small, explicit API: show a line,
  show a timed line, show a sequence of lines, and clear.
- Copy held as data, separated from the actors that trigger it, so a line
  can be rewritten without touching gameplay code.
- A presentation layer that is legible against a dark, low-contrast frame
  and does not fight the authored camera composition.
- Migration of the player-facing AddOnScreenDebugMessage call sites in
  RewindCodeLock, RewindRadio, RewindFuse, RewindFuseSocket,
  RewindGenerator, RewindLift, RewindAnchorBoard and RewindCharacter.
- A first design document, docs/design/player-messages.md, declaring what
  the system owns: when the game speaks, how long a line stays, what may
  not become a message, and that the debug overlay is not UI.
- Named automation tests for queueing, ordering and expiry.

### Out of Scope

- RewindLoopSubsystem.cpp. REW-0017 owns it, including its own overlay
  calls. Leave them.
- The `t=` elapsed-time overlay, which is instrumentation for stated-time
  checks and must keep working.
- Voice, audio and localisation.
- Renderer configuration, lighting and materials. REW-0019 owns those.
- Any .umap or .uasset binary. REW-0019 owns those.
- Interaction close-up camera framing, which docs/design/camera-and-
  movement.md already owns.
- docs/TASK_IDS.md. The operator owns it and already claimed REW-0018.

### Definition of Done

- No player-facing AddOnScreenDebugMessage call remains in the files listed
  in scope.
- Every migrated line is reachable and readable in a build.
- Copy is data: changing a line does not require editing an actor's logic.
- docs/design/player-messages.md exists, declares one ownership area, and
  is listed in docs/design/README.md.
- Named Rewind.* automation tests cover queueing, ordering and expiry, and
  all discoverable Rewind.* tests pass together.

### Minimum Verification Gates

- [x] RewindEditor Win64 Development builds with the editor closed.
- [x] All discoverable Rewind.* automation tests pass together, run headless.
- [x] The journal entry names what was verified and what was not.

## References

- `docs/backlog/art-direction.md` P7 and Stage 7: the debug overlay is not UI
- `docs/design/camera-and-movement.md`: authored framing; this task must not
  fight it and must not take close-up ownership
- `docs/design/chapter-1-authored.md`: the facts the migrated lines refer to
- `docs/design/loop-and-determinism.md`: messages are presentation, not world
- `Rewind/Source/Rewind/RewindLoopSubsystem.cpp`: leave its overlay calls

## Checklist

- [x] Transcribe this frozen charter into `docs/CURRENT_TASK.md`
- [x] Add `docs/design/player-messages.md` and list it in `docs/design/README.md`
- [x] Runtime catalog: copy as data, keyed by id, durations included
- [x] Runtime queue with Show, ShowTimed, ShowSequence and Clear
- [x] World subsystem plus Slate presentation that sits off the composed subject
- [x] Enable Slate / SlateCore in `Rewind.Build.cs`
- [x] Migrate RewindCodeLock, RewindRadio, RewindFuse, RewindFuseSocket,
      RewindGenerator, RewindLift, RewindAnchorBoard, RewindCharacter
- [x] Named Rewind.* tests for queueing, ordering and expiry
- [x] Confirm no player-facing AddOnScreenDebugMessage remains in scoped files
- [x] RewindEditor Win64 Development, editor closed
- [x] Headless `Automation RunTests Rewind;Quit`
- [x] Update `docs/CURRENT_STATUS.md`, `docs/SYSTEMDOC.md`, `docs/FILESTRUCTURE.md`
- [x] Signed `docs/JOURNAL.md` entry
- [x] Archive charter, restore template, commit, push, open PR

## Decisions and Notes

- Copy lives in C++ catalog data, not a `.uasset` or DataTable. REW-0019 owns
  Unreal binaries; a rewrite of a line is a catalog edit, not an actor edit.
- Presentation is a project Slate overlay added to the game viewport. GameMode
  is not in this task's file ownership, so the HUD class is not the vehicle.
- Catalog durations restate the previous overlay times so the slice does not
  silently change how long a line is readable. Changing a duration is a
  catalog edit.
- Messages are presentation. They tick on frame delta, which
  `camera-and-movement.md` already permits for presentation. They are not
  world state and they are not saved.
- This task does not edit `RewindLoopSubsystem`, so it does not clear the
  queue at loop start. A line may outlive a rewind by its remaining duration.
  Whether rewind should silence the overlay is not a rule; it is noted here.
- Colour is not a second information channel. The previous green/red/cyan
  debug colours are not restated as meaning.
- `LogRewind` lines stay as logs. They are not player-facing messages.

## Charter Amendment Log

Only non-semantic corrections are allowed after `Ready`.

-none

## Verification

- [x] RewindEditor Win64 Development, editor closed. First pass failed on
      `RewindMessageTest.cpp` `TestEqual` int32 versus size_t. After the
      cast, the same command succeeded in 16.61 seconds.
- [x] Headless `UnrealEditor-Cmd` `-ExecCmds='Automation RunTests Rewind;Quit'`
      `-unattended -nopause -nosplash -NullRHI`. Found 13 tests, all
      `Result={Success}`, `TEST COMPLETE. EXIT CODE: 0`.
- [x] Scoped files contain no player-facing AddOnScreenDebugMessage. The only
      remaining calls are in `RewindLoopSubsystem.cpp`.
- [x] Document skipped checks and reasons: no PIE on this clone, so overlay
      legibility against the authored frame was not judged in a running
      viewport. No packaged build. Each migrated line was not triggered in
      play.

## Documentation Updates

- [x] `docs/CURRENT_STATUS.md`
- [x] `docs/SYSTEMDOC.md`
- [x] `docs/JOURNAL.md`
- [x] `docs/FILESTRUCTURE.md` when structure changes
- [x] `docs/design/player-messages.md` and `docs/design/README.md`
- [ ] ADRs when long-lived decisions change

## Handoff and Follow-ups

- Current state: complete on this branch; awaiting operator review of the PR
- Next recommended step: operator reviews and merges; a later playtest can
  judge the overlay against the authored frame
- Blockers: none
- Child tasks: none
- Resume condition: n/a
- Open questions: whether rewind should clear the message queue (not owned)

## Finalize When Complete

- Archive this file under `docs/finished/`.
- Restore this template or populate the next approved task.
- Add a signed `docs/JOURNAL.md` entry.
- If Goal or Definition of Done changed, supersede this task instead of
  rewriting it.
