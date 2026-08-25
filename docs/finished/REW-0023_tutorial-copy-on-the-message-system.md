# Current Task

Task ID: REW-0023
Parent Task: None
Status: Complete
Owner: grok-copy
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
- `docs/design/player-messages.md`
- `docs/design/chapter-1-authored.md`
- `docs/design/loop-and-determinism.md`
- `docs/adr/ADR-0012_echo-semantics.md`
- Relevant ADRs under `docs/adr/`

## Task Summary

REW-0018 built the message system: actors speak by catalog id, copy lives in
`FRewindMessageCatalog`, and a Slate line renders at the bottom of the
viewport. What it does not have is a first-time player's path through the
authored slice.

A player who has never seen RE:WIND currently starts in Apartment 4C with no
idea that the world loops, that knowledge persists, that the radio carries a
code, or that the fuse is contested. `docs/PROJECT_BRIEF.md` requires the game
to "be legible to a player who has never seen it before" while rewarding one
who has seen it five times.

Tutorial copy that simply tells the player the answer destroys the product.
Copy that teaches the player how to look is what is wanted.

## Task Charter

The charter is editable while status is `Draft` and immutable once status is
`Ready`.

### Goal

A first-time player learns what kind of game this is from the authored
slice itself, without being told the puzzle's answers.

### Primary Deliverable

A first-run copy set in the message catalog, driven by the existing
message subsystem, plus an owning design document that states what the
game may and may not say to the player.

### In Scope

- Copy for the first-loop path through the authored slice: waking in 4C,
  the radio, the code panel, the fuse and its sockets, the lift refusing
  an empty socket, and the stairs as the always-available route.
- A rule, stated in the owning document and enforced by the copy, that
  the game names what a thing is and never what to do with it. The radio
  may be described as carrying a voice; the digits are never printed.
- First-time gating: a line the player has already seen does not repeat on
  later loops. Knowledge is session state and already has an owner in
  URewindSessionSubsystem; use it rather than inventing a second store.
- Copy for the loop boundary itself, so a player understands that the
  world reset and that what they know did not.
- docs/design/tutorial-and-first-run.md, declaring one ownership area,
  with a Status line and explicit Owns and Does not own, listed in
  docs/design/README.md.
- Named automation coverage for first-time gating: a line shown once is
  not shown again for the same session.

### Out of Scope

- Any change to the message subsystem's API or presentation. REW-0018
  owns those and they are accepted; you are authoring content on top.
- Echo. ADR-0012 decided it is narrative only, so no copy may describe a
  ghost the player will see, because there will not be one.
- Voice, audio and localisation.
- The courtyard and anything beyond the entrance hall, which does not
  exist yet and is chartered as REW-0020.
- Renderer, lighting, materials, meshes and any .umap or .uasset binary.
- docs/TASK_IDS.md. The operator owns it and already claimed REW-0023.

### Definition of Done

- A first-time player reaching the entrance hall has been told, by the
  game, that the world loops and that their knowledge persists, without
  being given the code, the fuse's destination or the route.
- No line in the set states a puzzle solution.
- A line shown once in a session is not shown again in that session.
- docs/design/tutorial-and-first-run.md exists, owns one area, and is
  indexed in docs/design/README.md.
- Named Rewind.* automation tests cover first-time gating, and all
  discoverable Rewind.* tests pass together.

### Minimum Verification Gates

- [x] RewindEditor Win64 Development builds with the editor closed.
- [x] All discoverable Rewind.* tests pass together, run headless.
- [x] Every line in the delivered set is quoted in the journal entry, so the
      owner can read the whole script without opening the editor.
- [x] The journal entry names what was verified and what was not.

## References

- `docs/design/player-messages.md`: the channel; this task authors content on it
- `docs/design/chapter-1-authored.md`: the objects the first-run path names
- `docs/design/loop-and-determinism.md`: the world does not learn the player
- `docs/design/save-and-session.md`: knowledge facts already have a store
- `docs/adr/ADR-0012_echo-semantics.md`: Echo is narrative only; no ghost copy
- `Rewind/Source/Rewind/RewindMessageCatalog.*`, `RewindMessageIds.h`,
  `RewindMessageSubsystem.*`: the accepted message system

## Checklist

- [x] Transcribe this frozen charter into `docs/CURRENT_TASK.md`
- [x] Write `docs/design/tutorial-and-first-run.md` and index it
- [x] First-run catalog ids and copy; rewrite solution-giving existing lines
- [x] First-time gating through `URewindSessionSubsystem` knowledge facts
- [x] Trigger first-run lines on the first-loop path and the loop boundary
- [x] Named `Rewind.*` tests for first-time gating
- [x] RewindEditor Win64 Development, editor closed
- [x] Headless `Automation RunTests Rewind;Quit`
- [x] Update `docs/CURRENT_STATUS.md`, `docs/SYSTEMDOC.md`, `docs/FILESTRUCTURE.md`
- [x] Signed `docs/JOURNAL.md` entry quoting every delivered line
- [x] Archive charter, restore template, commit, push, open PR

## Decisions and Notes

- A checkpoint after each step or substep is required. Checklist is therefore updated along the work and `CURRENT_STATUS.md` is always updated when changes affect the behavior.
- Record decisions and assumptions within the frozen charter.
- Classify discoveries using `docs/TASK_WORKFLOW.md`.
- First-run copy names objects. Existing REW-0018 catalog lines that printed
  `7312` or told the player to seat the fuse were content on this channel,
  not API, and are rewritten so no catalog template states a solution.
- First-time gating is `RewindFirstRun::ShowOnce`. The seen-state is the
  knowledge fact `seen.<MessageId>` on `URewindSessionSubsystem`. The
  message subsystem API is unchanged.
- Discovery, not a new rule: `ARewindCharacter::BeginPlay` runs once per
  pawn, and loop start teleports PlayerBody rather than respawning it.
  Loop-return copy is therefore triggered from `ARewindLoopBreakSignature`
  when the prelude ends, which is the visible loop boundary. Death has no
  prelude, so `Apartment.Returned` is not shown after death. The waking
  line already stated the product rule. Routed as a note, not a new death
  line.
- Discovery, not a new rule: `ARewindStairwell` is spawned only by the
  proof layout. The authored default map uses placed stair geometry.
  `Stairs.Remain` is shown when the player first enters a camera region
  whose name identifies the stairs, which covers both maps without a
  `.umap` edit.
- `Lift.NoPower` is first-run copy in the words it uses, and a refused
  action in when it speaks, so it is not first-time gated.
- Radio spoken digits remain the puzzle speaking. They are not first-run
  copy. First-run copy does not reprint them.
- Echo: no catalog line describes a ghost.
- Discovery, inside the charter: `URewindSessionSubsystem` is a
  `UGameInstanceSubsystem` and cannot be `NewObject`'d on the transient
  package. The gating test therefore uses the same `TSet<FName>` the
  session stores. Actor `ShowOnce` still writes `seen.<MessageId>` through
  `ObtainKnowledge`. Not a second store.

## Charter Amendment Log

Only non-semantic corrections are allowed after `Ready`.

-none

## Verification

- [x] RewindEditor Win64 Development, editor closed. After the gating-test
      fix and unused-helper removal: Result Succeeded, total execution time
      20.11 seconds.
- [x] Headless `UnrealEditor-Cmd` `-ExecCmds='Automation RunTests Rewind;Quit'`
      `-unattended -nopause -nosplash -NullRHI`. Found 19 tests, all
      `Result={Success}`, `TEST COMPLETE. EXIT CODE: 0`.
- [x] Every delivered line quoted in the journal.
- [x] Document skipped checks and reasons: no PIE on this clone; overlay
      timing against the authored frame not judged; death does not show
      `Apartment.Returned`; courtyard out of scope; no packaged build.

## Documentation Updates

- [x] `docs/CURRENT_STATUS.md`
- [x] `docs/SYSTEMDOC.md`
- [x] `docs/JOURNAL.md`
- [x] `docs/FILESTRUCTURE.md` when structure changes
- [ ] ADRs when long-lived decisions change

## Handoff and Follow-ups

- Current state: complete on this branch; awaiting operator review of the PR
- Next recommended step: operator reviews and merges; a later playtest can
  walk the first-run path in PIE
- Blockers: none
- Child tasks: none
- Resume condition: n/a
- Open questions: none

## Finalize When Complete

- Archive this file under `docs/finished/`.
- Restore this template or populate the next approved task.
- Add a signed `docs/JOURNAL.md` entry.
- If Goal or Definition of Done changed, supersede this task instead of
  rewriting it.
