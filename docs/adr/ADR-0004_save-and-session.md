# ADR-0004 — Save and session for a clean save

Status: Accepted
Date: 2026-08-19
Deciders: MrWhite
Supersedes: nothing
Superseded by: nothing

## Context

The Five Loops Test starts from a clean save. Conflict C7 asks what
survives a loop, what survives quitting, and how a clean save is
obtained. The Unity stores wrote knowledge and anchors to PlayerPrefs
with no clear. A playtest that cannot return to nothing cannot be
repeated.

ADR-0002 already says the player's body is world and resets, and that
knowledge is not world. ADR-0003 says Anchors are the only world
exception. This decision names what is stored, not how it is serialised.

## Decision

Three states matter.

**Loop-volatile world** is not saved across a loop boundary. It is
rebuilt at loop start from baseline then Anchors. The player's body is
loop-volatile.

**Across loops, inside a session:** knowledge facts and active Anchors
persist. Nothing else does.

**Across sessions:** loading a session restores the same knowledge facts
and active Anchors. It does not restore loop-volatile world. The next
loop after load starts like any other loop.

**A clean save** is knowledge empty, no active Anchors, and the next
loop starting from baseline. It must be reachable as a first-class
action. A playtest that needs a hex editor, a deleted file the game
cannot name, or a new OS user is not a clean save.

The first product proof's required knowledge fact is that the player has
obtained the radio code, once they have. Other facts may be added only
by the authored-content document.

The owning game rule is restated in
[`docs/design/save-and-session.md`](../design/save-and-session.md).

## Alternatives considered

**Knowledge lives only in the player's head.** Rejected as the whole
model. The human tester will remember `7312` anyway. The game still
needs a defined store so a session can resume, a clean save can wipe,
and later UI can show what is known. Head-only knowledge cannot be
cleared.

**Persist everything that is in memory, including pose and patrol
phase.** Rejected. That is not a loop. It is a pause.

**Clean save is a developer-only wipe of engine save data.** Rejected
as the definition, though a developer path may exist during
implementation. The rule is that the game can return to nothing.

**Defer serialisation format (files, slots, cloud).** Accepted as
unspecified. This ADR does not choose a format. It chooses the state.

## Consequences

**Accepted costs:**

- Implementation must provide a reachable clean save before the Five
  Loops Test can be run.
- A knowledge fact needs a definition of when it is obtained, not only
  that the human heard something.

**Accepted benefits:**

- Two playtests can start from the same nothing.
- Quitting is not a loop, and it is not a wipe.
- ADR-0002 and ADR-0003 have a place to put the things they said
  persist.

**Consequences that bind future tasks:**

- No task may persist loop-volatile world across a loop or a session
  load.
- No task may leave knowledge or Anchors in a store that cannot be
  cleared by the clean-save action.
- Storage format, slot count and UI chrome are not decided here. They
  belong to the implementation task, provided this state is what is
  stored.
