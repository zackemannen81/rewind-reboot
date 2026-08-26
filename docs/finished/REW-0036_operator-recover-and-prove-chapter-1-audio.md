# Current Task

Task ID: REW-0036
Parent Task: None
Status: Complete
Owner: Codex (process supervisor)
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
- `docs/JOURNAL.md` — REW-0016, REW-0033 and REW-0035
- `docs/FILESTRUCTURE.md`
- `docs/EDITOR_AUTOMATION.md`
- `docs/design/chapter-1-authored.md` — Radio and code
- `docs/design/camera-and-movement.md`
- `docs/finished/REW-0016_operator-finish-chapter-1-through-delegated-agents.md`
- `docs/finished/REW-0035_wire-chapter-1-audio-into-gameplay.md`
- `docs/waves/REW-0036_chapter-1-audio-recovery.md`
- `C:\code\docs-first-multiagent-orchestrator-addon\README.md`
- `C:\code\docs-first-multiagent-orchestrator-addon\templates\MASTER_AGENT_PROMPT.md`
- `C:\code\docs-first-multiagent-orchestrator-addon\templates\OPERATOR_TASK.md`

## Task Summary

REW-0035 merged Chapter 1 audio wiring without an interactive playback pass.
The first operator PIE inspection after merge falsified two assumptions: the
canonical clone was running a DLL older than the merged source, and the new
spatial radio sounds use a 320 cm attenuation boundary while the authored
camera — Unreal's default audio listener — is about 22 m from the radio even
when the player is inside the 320 cm hearing-credit range.

This task is the process-supervisor role for the bounded recovery wave. It
owns the child charter, isolated clone, process launch, monitoring, decisions,
review, merge, canonical rebuild, interactive editor and integrated evidence.
It does not implement the child task's source changes.

## Task Charter

The charter is editable while status is `Draft` and immutable once status is
`Ready`.

### Goal

Land REW-0037 through an isolated delegated agent and prove on the integrated
canonical build that Chapter 1 audio renders from the player's hearing frame
under the authored camera, including the channel-3 station and `7312` digits.

### Primary Deliverable

REW-0037 merged to `main` with a fresh canonical `RewindEditor` build,
complete `Rewind.*` automation results and a clean standard-PIE audio record
showing the radio bed, channel interactions, all four scheduled digits and
player footsteps through the intended runtime events.

### In Scope

- Freeze, brief and delegate REW-0037 through the Docs-First PowerShell Agent
  MCP server at `C:\code\docs-first-multiagent-orchestrator-addon`.
- Assign one clean physical clone, one branch and an exclusive file boundary
  to the child; the operator retains task IDs, canonical integration, merge
  authority, the interactive Unreal editor and shared build/test resources.
- Decide the audio-listener correction: spatial gameplay audio is evaluated
  from the possessed player's position while retaining the authored camera's
  orientation and view target.
- Require the child to preserve the accepted channel, digit timing, hearing
  range, loop reset and deterministic-world rules.
- Review the child against its frozen charter, merge only sufficient work and
  run combined verification after merge.
- Correct durable status claims falsified by the stale DLL and detached-camera
  listener evidence.

### Out of Scope

- Writing REW-0037 implementation code in the supervisor branch.
- New audio sources, replacement voice, music, final sound mix, reverb,
  occlusion or broad sound-design work.
- Changing radio channel count, code, phases, cycle, sequence length,
  `RadioRange`, loop rules, player speed or camera composition.
- Editing `.umap` or existing audio `.uasset` content.
- Changing Windows' default playback device or other machine-wide audio
  settings; the evidence must name the device Unreal actually selected.
- Chapter 2 implementation or unrelated Chapter 1 polish.

### Definition of Done

- REW-0037 is merged to `main`, archived under `docs/finished/`, and carries a
  signed journal entry with named verification evidence.
- A fresh canonical editor-closed build loads the REW-0037 class shape; the
  PIE radio exposes its audio component rather than the stale pre-REW-0035
  `Root`/`Mesh`-only shape.
- Radio hearing credit and spatial playback use the same player-relative
  320 cm boundary despite the detached authored camera.
- A standard-PIE run reaches channel 3 and records the station bed, interaction
  sounds, `7312` at phases 4/9/14/19 and distance-driven footsteps; the owner
  confirms audible output on the named playback device.
- The complete integrated `Rewind.*` suite passes, documentation states what
  was and was not heard, and no worker branch remains unmerged without a
  stated reason.

### Minimum Verification Gates

- [x] The ownership matrix is checked before launch and no worker shares the
      canonical clone, editor, task-ID register or merge authority.
- [x] The MCP process plane passes a harmless smoke task before REW-0037 is
      launched.
- [x] REW-0037 passes an editor-closed `RewindEditor Win64 Development` build
      and its focused audio tests in its isolated clone.
- [x] The supervisor reviews the complete child diff and evidence against the
      frozen child charter before merge.
- [x] After merge, the canonical clone receives a fresh editor-closed build
      and the complete `Rewind.*` automation suite passes.
- [x] Standard PIE is driven through `RewindEditor.RewindPIEInputToolset`, all
      held keys are released before PIE stops, and runtime component/listener,
      channel-3 and digit evidence is recorded.
- [x] The selected Windows playback device is named and an owner-heard check
      distinguishes actual output from logs or active-source state.
- [x] `git diff --check` is clean and unrelated local files remain untouched.

## References

- `docs/waves/REW-0036_chapter-1-audio-recovery.md`
- `Rewind/Source/Rewind/RewindCameraRig.cpp`
- `Rewind/Source/Rewind/RewindRadio.cpp`
- `Rewind/Source/Rewind/RewindRadioAudioTest.cpp`
- `Rewind/Source/Rewind/RewindCharacter.cpp`
- `/Game/Audio/Chapter1/A_REW_Radio_Attenuation`
- `C:\code\docs-first-multiagent-orchestrator-addon`
- `C:\code\rewind-multiagents\rewind-reboot3`

## Checklist

- [x] Re-read repository authority, REW-0016 and the add-on operator model.
- [x] Reproduce and classify the missing-audio report in standard PIE.
- [x] Claim REW-0036 and REW-0037 on `main` and push the claims.
- [x] Freeze the supervisor charter and wave ownership plan.
- [x] Register and smoke-test the Docs-First PowerShell Agent MCP process plane.
- [x] Synchronize the isolated worker clone and write the verbatim REW-0037 brief.
- [x] Launch and monitor REW-0037; make all scope and implementation decisions.
- [x] Review the worker branch and send back named defects until sufficient.
- [x] Merge REW-0037 and run the canonical combined gates.
- [x] Run the standard-PIE channel-3/audio scenario and obtain the owner-heard check.
- [x] Correct durable documentation, archive the task and restore the template.

## Decisions and Notes

- A checkpoint after each step or substep is required. The checklist remains
  truthful and `CURRENT_STATUS.md` changes with observed behavior.
- The add-on's two-plane model is binding for this wave: repository state is
  authority; MCP state and worker output are process signals only.
- Only one implementation child exists. Stale build state, listener placement,
  radio playback and its regression evidence are one coupled runtime boundary;
  parallel source tasks would overlap the same files without independent goals.
- The operator decision is player-position audio with authored-camera
  orientation. Moving the camera, widening `RadioRange`, making spatial sounds
  globally 2D or changing the puzzle's hearing rule are rejected.
- The original Codex session did not expose the five process tools. The project
  config therefore registers the stdio server for future repository-root
  sessions. No unrecorded substitute is treated as the process plane.
- The reconnected desktop session still did not surface the stdio tools. The
  add-on's own installer restored its missing npm dependencies and passed all
  six package tests. A recorded long-lived raw MCP stdio transport then listed
  the same five tools and is the process plane for this wave; Git checkpoints
  remain recovery authority if that ephemeral process is lost.
- The first REW-0037 worker froze and committed the charter, then its Grok
  provider returned HTTP 402 because the Build usage balance was exhausted.
  The operator pushed that durable checkpoint, preserved the three-file
  partial diff as untrusted handoff state, and replaced only the process with
  Codex CLI under a new MCP agent ID. Scope, clone, branch, ownership and
  charter did not change.
- The Unreal editor is operator-owned. It must be closed for canonical builds
  and reopened from the repository root for PIE evidence.

## Charter Amendment Log

Only non-semantic corrections are allowed after `Ready`.

- none

## Verification

- [x] Observed PIE player `(50, 1300, 1298)`, radio `(-225, 1340, 1260)` and
      authored camera near `(1970, 1300, 1390)`; the player was inside 320 cm
      while the default listener/view target was about 22 m from the radio.
- [x] Observed the loaded PIE radio exposing only `Root` and `Mesh`; the merged
      source declares `RadioBed`.
- [x] Compared timestamps: canonical `UnrealEditor-Rewind.dll` was built at
      01:05 local, before REW-0035 source at 05:07 local.
- [x] Unreal log named `LG TV (NVIDIA High Definition Audio)` as the active
      WASAPI render device for the diagnostic run.
- [x] Add-on install/test: 6 passed, 0 failed, including the real MCP stdio
      handshake. Harmless MCP smoke task exited 0 and emitted
      `agent-mcp-smoke-ok` through `run-agent-task`/`about-agent`.
- [x] Worker clone `rewind-reboot3` was clean, fast-forwarded to claimed base
      `c4c18af`, given branch `codex/rew-0037-audio-listener-runtime`, and the
      frozen brief was written under the operator-owned brief directory.
- [x] REW-0037 charter commit `b24a5fe` was inspected and pushed. The original
      `rew-0037-audio-runtime` process ended on provider HTTP 402; replacement
      process `rew-0037-codex-takeover-2` was launched through the same MCP
      server against the preserved isolated worktree.
- [x] Codex completed REW-0037 in the isolated clone. Full diff review was
      clean; PR #39 merged as `7b71cc5`, and the child is archived.
- [x] Canonical editor-closed build succeeded in 74.03 seconds. The integrated
      headless `Rewind.*` suite passed 25/25 with
      `TEST COMPLETE. EXIT CODE: 0`.
- [x] Standard PIE loaded `RadioBed` alongside `Root` and `Mesh`, retained the
      authored `RewindCameraRig_0` view target, and used the player-relative
      listener contract covered by the integrated listener test. The project
      input toolset moved the player within interaction range, selected channel
      3 and recorded digits seven, three, one and two at phases 4/9/14/19.
- [x] The next complete cycle logged `complete 20s sequence heard`; the owner
      confirmed, "yes, radion funkar perfekt nu", on Unreal's named
      `LG TV (NVIDIA High Definition Audio)` output. All injected keys were
      released before standard PIE stopped.

## Documentation Updates

- [x] `docs/CURRENT_STATUS.md`
- [x] `docs/SYSTEMDOC.md`
- [x] `docs/JOURNAL.md`
- [x] `docs/FILESTRUCTURE.md`
- [x] REW-0037 archive and final behavior corrections after merge
- [ ] ADRs only if review finds a long-lived decision not already owned

## Handoff and Follow-ups

- Current state: complete. REW-0037 is reviewed, merged and archived;
  canonical build, integrated 25/25 automation and standard PIE are green.
- Next recommended step: activate the separately requested Apartment 4C visual
  target task using the supplied concept and existing generated assets.
- Blockers: none. Grok usage exhaustion was routed by replacing the worker
  process without changing the task.
- Child tasks: REW-0037, frozen in the wave plan and active.
- Resume condition: not paused. After a process loss, restart the stdio server,
  inspect the REW-0037 branch and resume from its last pushed checkpoint.
- Open questions: none about product or implementation direction. The final
  heard-output gate requires the owner to confirm what reaches the named device.

## Finalize When Complete

- Archive this file as
  `docs/finished/REW-0036_operator-recover-and-prove-chapter-1-audio.md`.
- Restore `docs/CURRENT_TASK.md` from the template.
- Add a signed `docs/JOURNAL.md` completion entry.
- If Goal or Definition of Done changed, supersede this task instead of
  rewriting it.


