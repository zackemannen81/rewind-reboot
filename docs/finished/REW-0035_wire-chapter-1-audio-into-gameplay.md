# Current Task

Task ID: REW-0035
Parent Task: None
Status: Complete
Owner: codex-audiowire
Created: 2026-08-26
Last updated: 2026-08-26
Charter frozen at: 2026-08-26 (Ready)

## Read First

- `AGENTS.md`
- `docs/TASK_WORKFLOW.md`
- `docs/design/chapter-1-authored.md` — Radio and code
- `docs/adr/ADR-0002_world-determinism.md`
- `docs/JOURNAL.md` — REW-0033
- `docs/EDITOR_AUTOMATION.md`

## Task Summary

REW-0033 supplied the Chapter 1 SoundWaves, but no gameplay code referenced
them. This task wired the existing radio schedule and player movement to those
assets without changing authored radio timing or introducing loop-dependent
audio.

## Task Charter

### Goal

Make all fifteen REW-0033 Chapter 1 sounds reachable through the intended
gameplay events.

### Primary Deliverable

Radio digit, bed, and interaction playback plus distance-driven, varied player
footsteps, with attenuation consistent with radio hearing credit.

### In Scope

- Attach the four digit one-shots to the existing `ReportFragment` events while
  retaining their readable messages.
- Add radio station/static beds, off behavior, dry interaction one-shots, and a
  `USoundAttenuation` asset for radio digit audibility.
- Add distance-driven interior footsteps that do not repeat the prior variation.
- Add radio digit/audio-state rewind automation coverage.
- Build and run the complete `Rewind` automation suite.
- Archive this task and record the evidence in the journal and system document.

### Out of Scope

- Changing radio timing, channels that carry the code, loop rules, player speed,
  audio source files, or imported SoundWaves.
- Map edits except if essential to create the attenuation asset.
- New gameplay progression, sound mix, animation-notify, or UI work.

### Definition of Done

- All fifteen supplied sounds are referenced from runtime gameplay code.
- Radio digits remain both audible and readable; beds and interactions follow
  radio state; radio digit attenuation does not exceed `RadioRange = 320.0`.
- Footsteps are distance-driven and do not immediately repeat an index.
- The existing suite and the two new radio tests pass in an actual headless run.
- The archive and journal contain the completed charter and named evidence.

### Minimum Verification Gates

- [x] Build `RewindEditor` with Unreal Editor closed.
- [x] Run the requested headless `Rewind` automation suite and record its count.
- [x] Confirm the radio selection and rewind-clean-state tests pass.
- [x] Run `git diff --check`.

## References

- `docs/design/chapter-1-authored.md`
- `docs/adr/ADR-0002_world-determinism.md`
- `docs/JOURNAL.md` — REW-0033
- `Rewind/Source/Rewind/RewindRadio.cpp`
- `Rewind/Source/Rewind/RewindCharacter.cpp`

## Checklist

- [x] Read task authority, implementation, audio provenance, and editor procedure.
- [x] Wire radio sounds and reset their loop-local state.
- [x] Create and configure radio attenuation without editing a map.
- [x] Wire distance-driven, non-repeating footsteps.
- [x] Add radio audio automation tests.
- [x] Build and run verification gates.
- [x] Update system documentation, archive the charter, and add the journal entry.

## Decisions and Notes

- The inspected radio had no separate on/off member. `Channel` remains the sole
  radio state: channel `0` is off and channels `1`–`4` retain tuning. Baseline
  returns it to channel `1` and replaces any station bed with static.
- Digit scheduling remains derived only from elapsed loop time. Footstep choice
  is random only because it is cosmetic and has no gameplay effect.
- Radio digit and bed attenuation use the created spherical linear
  `/Game/Audio/Chapter1/A_REW_Radio_Attenuation` asset, with zero inner extent
  and a 320 cm falloff. This agrees exactly with `RadioRange = 320.0`: a digit
  cannot remain audible outside the range that grants credit.
- On/off/tune interactions use dry 2D playback. No map was opened or edited.

## Charter Amendment Log

Only non-semantic corrections are allowed after `Ready`.

- none

## Verification

- Build: `RewindEditor Win64 Development` succeeded after confirming no
  `UnrealEditor` process was open (2026-08-26).
- Automation: the requested headless command completed with exit code `0`.
  `Rewind.Radio.Audio.DigitSelection` and
  `Rewind.Radio.Audio.ResetAtRewind` both passed; the log contains 24
  `Result={Success}` completions and no fail/error result.
- Attenuation: the asset-creation commandlet exited `0` and read back its 320.0
  cm falloff.
- `git diff --check` completed cleanly.
- Not run: interactive PIE/audio playback. The schedule is compiled and
  unit-tested, but this actor did not hear it in play.

## Documentation Updates

- [x] `docs/SYSTEMDOC.md`
- [x] `docs/JOURNAL.md`

## Handoff and Follow-ups

- Current state: complete; ready for operator review and merge.
- Next recommended step: review the diff and, if desired, perform an
  interactive audio mix/playback pass.
- Blockers: none.
- Child tasks: none.
- Resume condition: n/a.
- Open questions: none.
