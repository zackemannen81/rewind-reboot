# REW-0036 Chapter 1 audio recovery wave

Status: Frozen
Operator task: REW-0036
Prepared by: Codex (process supervisor)
Prepared at: 2026-08-26
Base revision: `c4c18af74ac0b60aa562bacfc68d5d0a9c26b283`

## Goal decomposition

The wave has one implementation child. The canonical stale build, the detached
camera's audio-listener position, radio attenuation and end-to-end playback are
one coupled runtime defect. Splitting them between writing agents would overlap
the same camera/radio/test files and would not create independent deliverables.

The operator remains a separate task because it owns process state, decisions,
review, integration, the Unreal editor and the owner-heard acceptance gate; it
does not absorb implementation if the child fails.

## Worker allocation

| Child task | Goal | Physical clone | Branch | Agent ID | CLI executable |
| --- | --- | --- | --- | --- | --- |
| REW-0037 | Make Chapter 1 audio render from the player's hearing frame under the authored camera | `C:\code\rewind-multiagents\rewind-reboot3` | `codex/rew-0037-audio-listener-runtime` | `rew-0037-audio-runtime` | `C:\Users\zakri\.grok\bin\grok.exe` |

## Frozen REW-0037 charter

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

## Ownership matrix

| Path or resource | Owner | Access | Merge behavior |
| --- | --- | --- | --- |
| `docs/TASK_IDS.md` | REW-0036 operator | exclusive write | claims already on `main` |
| canonical clone `C:\code\rewind-reboot` | REW-0036 operator | exclusive write | integration only |
| interactive Unreal editor and port 8000 | REW-0036 operator | global lock | closed for builds; operator PIE only |
| `Rewind/Source/Rewind/RewindCameraRig.*` | REW-0037 | exclusive write | normal text merge |
| `Rewind/Source/Rewind/RewindRadio.*` | REW-0037 | exclusive write | normal text merge |
| `Rewind/Source/Rewind/RewindCharacter.*` | REW-0037 | exclusive write if required | normal text merge |
| `Rewind/Source/Rewind/RewindRadioAudioTest.cpp` and new focused audio tests | REW-0037 | exclusive write | normal text merge |
| `docs/CURRENT_TASK.md`, `docs/SYSTEMDOC.md`, `docs/CURRENT_STATUS.md`, `docs/JOURNAL.md`, `docs/finished/REW-0037_*` in worker clone | REW-0037 | task-scoped write | operator reviews semantic merge |
| `.umap`, existing `.uasset`, audio WAV sources | nobody in this wave | forbidden | no merge |
| merge, pull-request acceptance and combined verification | REW-0036 operator | exclusive | operator decision |

The single worker's ownership set is trivially pairwise disjoint. It does not
intersect the operator's task-ID, integration, editor or merge locks.

## Global locks

- Canonical integration clone: `C:\code\rewind-reboot`, operator only.
- Worker clone: `C:\code\rewind-multiagents\rewind-reboot3`, REW-0037 only.
- Interactive editor/MCP port 8000: operator only.
- Binary assets: no writer in this wave.
- Task-ID register and merge authority: operator only.
- Windows playback configuration: observed and named, never changed.

## Launch order and permission preflight

1. Confirm REW-0036 and REW-0037 claims on `origin/main`.
2. Start a fresh Codex session from the canonical repository root so the
   `docs_first_agents` stdio server is registered.
3. Confirm all five process tools and run a harmless MCP smoke task.
4. Resolve and version-check `C:\Users\zakri\.grok\bin\grok.exe`.
5. Fetch, clean-check and fast-forward the worker clone to the frozen base;
   preserve no generated or unrelated changes by assumption.
6. Write the REW-0037 worker brief verbatim from this charter.
7. Launch with a non-interactive permission mode no broader than the task's
   authorized edit/build/commit/push/PR effects.

## Merge order

REW-0037 is the only child. The operator reviews its complete diff and
verification, requests corrections as needed, then merges. Any status or
system-document wording is reconciled against the canonical post-build and
PIE observations, not accepted from the worker branch merely because it is
newer.

## Combined gates

- Fresh canonical editor-closed `RewindEditor Win64 Development` build.
- Complete integrated `Rewind.*` automation suite.
- Standard PIE driven by the project input toolset: loaded radio component,
  player/listener relation, channel 3, station bed, all four digit events and
  footsteps; release all keys before stop.
- Unreal-selected output device named and owner-heard confirmation recorded.
- Documentation review and `git diff --check`.

## Recovery surface

- Frozen wave plan: `docs/waves/REW-0036_chapter-1-audio-recovery.md`.
- Worker brief directory: `C:\code\rewind-multiagents\briefs`.
- First pushed checkpoint: frozen REW-0037 charter before implementation.
- Worker log/state: MCP memory only; Git branch and pushed checkpoints are the
  recovery authority.
- Recovery procedure:
  `C:\code\docs-first-multiagent-orchestrator-addon\templates\CRASH_RECOVERY.md`.
