# Current Task

Task ID: REW-0037
Parent Task: REW-0036
Status: In Progress
Owner: codex-audio-runtime
Created: 2026-08-26
Last updated: 2026-08-26
Charter frozen at: 2026-08-26 (Ready)

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
- `docs/design/chapter-1-authored.md` — Radio and code
- `docs/design/camera-and-movement.md`
- `docs/adr/ADR-0002_world-determinism.md`
- `docs/finished/REW-0035_wire-chapter-1-audio-into-gameplay.md`
- Relevant ADRs under `docs/adr/`

## Task Summary

REW-0035 wired Chapter 1 sounds to the radio and footsteps, with 320 cm
attenuation matching hearing credit. The authored camera remains the visual
view target, so Unreal's default listener sits on that detached camera and
spatial radio can fall silent while the player is still in range. This task
corrects the listener so hearing follows the possessed player without moving
the camera or changing radio rules.

## Task Charter

The charter is editable while status is `Draft` and immutable once status is
`Ready`.

### Goal

Make Chapter 1 spatial audio audible from the possessed player's hearing
position while the existing authored camera remains the visual view target.

### Primary Deliverable

A runtime audio-listener correction plus regression coverage that keeps radio
bed/digit playback and hearing credit on the same 320 cm player-relative
boundary, delivered as a reviewed pull request.

### In Scope

- Attach or update Unreal's audio listener from the possessed player's
  position while retaining authored-camera orientation.
- Preserve the camera actor as the view target and preserve every accepted
  radio channel, timing, code, range, reset and deterministic-world rule.
- Ensure the REW-0035 radio component, bed, digit, interaction and footstep
  paths exist in a fresh build.
- Add focused automation/runtime observability that would fail when the audio
  listener silently falls back to the detached camera.
- Build, run the complete `Rewind.*` suite, update owning documentation,
  archive REW-0037, push and open a pull request.

### Out of Scope

- Map or existing audio-asset edits.
- New audio, final mix, reverb, occlusion, music or replacement voice.
- Moving the authored camera, widening `RadioRange`, making every gameplay
  sound 2D, or changing radio/puzzle/loop rules.
- Windows playback-device changes.
- `docs/TASK_IDS.md`, canonical integration, editor ownership and merge
  authority; those belong to REW-0036.

### Definition of Done

- A fresh REW-0037 build creates the radio audio component and applies the
  existing radio sounds and 320 cm attenuation.
- The active listener position follows the possessed player while its
  orientation follows the authored camera, including after loop/player reset.
- Channel 3 selects the station bed and schedules `7312` at phases 4, 9, 14 and
  19 without changing readable messages or knowledge semantics.
- Focused coverage demonstrates the player/listener contract with a detached
  camera, the full `Rewind.*` suite passes and the pull request contains an
  archived task plus signed journal evidence.

### Minimum Verification Gates

- [ ] Build `RewindEditor Win64 Development` with Unreal Editor closed.
- [ ] Run focused listener/radio tests and the complete `Rewind.*` suite.
- [ ] Inspect the built/runtime class or named test state for the audio
      component and player-relative listener contract.
- [ ] Run `git diff --check`, inspect the full diff and state skipped PIE or
      heard-output checks explicitly; the operator owns interactive PIE.

## References

- `docs/design/chapter-1-authored.md`
- `docs/design/camera-and-movement.md`
- `docs/adr/ADR-0002_world-determinism.md`
- `docs/adr/ADR-0007_camera-and-perspective.md`
- `docs/finished/REW-0035_wire-chapter-1-audio-into-gameplay.md`
- `docs/EDITOR_AUTOMATION.md`
- `Rewind/Source/Rewind/RewindCameraRig.cpp`
- `Rewind/Source/Rewind/RewindRadio.cpp`
- `Rewind/Source/Rewind/RewindCharacter.cpp`

## Checklist

- [x] Verify branch `codex/rew-0037-audio-listener-runtime` at base
      `c4c18af74ac0b60aa562bacfc68d5d0a9c26b283`.
- [x] Populate this charter from the template and freeze it In Progress.
- [x] Commit the charter as the first meaningful checkpoint.
- [x] Apply Unreal's audio-listener override from the possessed player's
      root/capsule with authored-camera orientation, including after
      SnapToPlayer / possession change.
- [x] Keep `ARewindCameraRig` as the view target; do not move the camera or
      change radio range, channels, timing or 2D interaction sounds.
- [x] Expose the smallest testable listener contract (attachment, requested
      world position, camera orientation, refresh after reset/possession).
- [x] Add focused automation that fails if the listener silently follows the
      detached camera; keep existing radio audio tests.
- [x] Build `RewindEditor Win64 Development` with the editor closed.
- [x] Run focused listener/radio tests, then the complete `Rewind.*` suite.
- [x] Update `docs/CURRENT_STATUS.md` and `docs/SYSTEMDOC.md`.
- [ ] Archive this task, restore the template, prepend a signed journal entry.
- [ ] `git diff --check`, inspect the full diff against the ownership list,
      commit explicit paths, push and open a pull request against `main`.

## Decisions and Notes

- Binding operator decision: the visual view target remains
  `ARewindCameraRig`. Do not move it and do not change camera composition.
- Binding operator decision: use Unreal's supported audio-listener override so
  the listener's world position tracks the possessed player's root/capsule
  while listener orientation matches the authored camera's current world
  orientation. The contract must remain true through camera-region changes and
  player placement/loop reset.
- Binding operator decision: do not solve this by increasing attenuation,
  changing `RadioRange`, or converting the radio bed/digits to 2D.
- The camera already ticks the possessed pawn and already snaps at loop start
  via `URewindWorldStateSubsystem`. Listener refresh belongs there so reset
  and region changes reuse the same path.
- Testable contract to expose:
  1. the player component is the listener's positional attachment/reference;
  2. the requested listener world position equals the possessed player position
     within tick tolerance;
  3. listener orientation follows the camera rather than player facing;
  4. the override is refreshed after player reset or possession changes;
  5. the radio still applies its `RadioBed` and existing attenuation/sounds.
- Interactive PIE and heard-output checks are owned by the operator. This
  actor will not launch the editor or claim heard evidence.

## Charter Amendment Log

Only non-semantic corrections are allowed after `Ready`.

- none

## Verification

- [x] Build `RewindEditor Win64 Development` with Unreal Editor closed.
- [x] Focused listener/radio tests and the complete `Rewind.*` suite.
- [x] Runtime class / named test state for the audio component and
      player-relative listener contract.
- [x] `git diff --check` and full-diff ownership review.
- Skipped: interactive PIE and heard-output. The operator owns those.
- Build: with no `UnrealEditor.exe` process, UE 5.8
  `Build.bat RewindEditor Win64 Development` succeeded in 49.50 seconds; the
  final test-fixture rebuild succeeded in 9.10 seconds.
- Focused listener: `Rewind.Audio.Listener` found one test;
  `PlayerPositionCameraOrientation` passed and the process reported
  `TEST COMPLETE. EXIT CODE: 0`.
- Focused radio: `Rewind.Radio.Audio` found two tests; `DigitSelection` and
  `ResetAtRewind` passed and the process reported exit code `0`.
- Complete suite: `Automation RunTests Rewind` found 25 tests; 25 passed,
  0 failed, and the process reported `TEST COMPLETE. EXIT CODE: 0`.
- Runtime evidence: the listener test calls UE 5.8's public
  `APlayerController::GetAudioListenerPosition` and compares its resulting
  world position/front/right vectors against the possessed root and detached
  camera through initial possession, possession replacement and reset snap.
  The radio reset test reads back the `RadioBed` component, shared attenuation
  asset, channel-3 station selection and Baseline static restoration.
- An initial focused listener run failed because the isolated `CreateWorld`
  fixture had not registered its controller in the world's player-zero list.
  The fixture now establishes the runtime relationship with `AddController`;
  the production path was unchanged, rebuilt and passed both focused and full
  reruns.

## Documentation Updates

- [x] `docs/CURRENT_STATUS.md`
- [x] `docs/SYSTEMDOC.md`
- [ ] `docs/JOURNAL.md`
- [ ] `docs/FILESTRUCTURE.md` when structure changes
- [ ] ADRs when long-lived decisions change

## Handoff and Follow-ups

- Current state: charter frozen In Progress on
  `codex/rew-0037-audio-listener-runtime` at
  `c4c18af74ac0b60aa562bacfc68d5d0a9c26b283`. Implementation not started.
- Next recommended step: implement the camera-owned listener override and
  focused tests.
- Blockers: none.
- Child tasks: none.
- Resume condition: n/a.
- Open questions: none.

## Finalize When Complete

- Archive this file under `docs/finished/`.
- Restore this template or populate the next approved task.
- Add a signed `docs/JOURNAL.md` entry.
- If Goal or Definition of Done changed, supersede this task instead of
  rewriting it.
