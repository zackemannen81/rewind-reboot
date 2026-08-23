# Loop and determinism

Status: Accepted
Owns: loop semantics and world determinism
Does not own: Anchor semantics, save and session, world-state apply order,
authored Chapter 1 facts, Echo
Source decisions: [`ADR-0002`](../adr/ADR-0002_world-determinism.md) and
[`ADR-0009`](../adr/ADR-0009_event-driven-loop-termination.md)

A rule that is not in this document is not a loop or determinism rule.

## Loop

A loop is one play of the authored space from start to end.

- A loop starts with the player's body at the loop-start pose, and with the
  world in the configuration ADR-0002 names: authored baseline, then
  Anchors as [`anchors.md`](anchors.md) defines them, then elapsed time
  zero. Apply order is [`world-state-model.md`](world-state-model.md).
- A loop ends when an authored causal contract fails at its named checkpoint,
  when the player dies, or when the player successfully commits an Anchor.
  Those are the only default end-condition classes.
- A global elapsed duration is not a default end condition. A whole-space
  deadline exists only when that authored situation is explicitly about the
  deadline.
- A causal contract is checked at a no-return checkpoint, not continuously
  while its prerequisite is still solvable. Once it fails, rewind is latched
  and cannot be canceled by stepping back.
- Contract failure and successful Anchor commit run a perceptible rewind
  prelude lasting at least one and at most three seconds before loop-start
  apply. Death need not wait for that mechanical prelude.
- After a loop ends, the next loop starts from the same baseline. Nothing
  the player did in the loop that just ended is world, except through
  Anchors.

The ordered apply of baseline, Anchors and spawn is owned by
[`world-state-model.md`](world-state-model.md). This document states
that the start configuration is determined, not how it is stored.

## Determinism

The world does not learn the player. The player learns the world.

At elapsed time `t` in a loop, world configuration is a function of:

1. the authored baseline;
2. active Anchors as [`anchors.md`](anchors.md);
3. `t`;
4. player actions during this loop with time `<= t`.

It is not a function of how many loops have run, of player actions from
earlier loops except through Anchors, of wall-clock time, of frame count,
or of a per-loop random seed.

World configuration includes object pose, puzzle flags, patrol phase,
gated collision, and any diegetic audio that is part of the space. It
does not include the player's knowledge. Knowledge is not world.

## Clock

Elapsed loop time begins at zero and has no default terminal duration. It is
the source of truth for every world clock: patrol
phase, breathing windows, timers that belong to objects, hover and any
other periodic motion.

Local deadlines may cause a causal-contract failure when missing the deadline
makes the authored situation impossible to complete. They must be legible
parts of that situation, never an invisible fallback that punishes exploration
or idling.

Per-frame delta and engine global time are not world clocks. They may
move a presentation, not a phase.

Two runs that share baseline, Anchors, elapsed loop time and this-loop
input match in world configuration. If they do not, the implementation
is wrong, not the player.

## What may change between loops

- The player's knowledge, as [`save-and-session.md`](save-and-session.md).
- Anchors, as [`anchors.md`](anchors.md), and only those overrides.
- Nothing else.

Loop count is not an input to any world object. A radio, a patrol, a
door, a poster, a statue, a fuse, a generator and a turnstile that differ
on loop two because it is loop two are in violation.

## Testable statements

These are the statements later acceptance may cite. They are not
themselves acceptance criteria.

1. From a loop start with no this-loop player input, world configuration
   at elapsed time `t` is the same on every loop that shares baseline and
   Anchors.
2. Changing frame rate does not change patrol phase, window phase or
   other world clocks at a given `t`.
3. No world object reads loop count.
4. Player actions from loop `N` are invisible to loop `N+1` except
   through active Anchors.
5. The player's body is at the loop-start pose at `t = 0`.
6. Elapsed time alone does not end a loop unless the whole authored situation
   declares a deadline.
7. The same checkpoint predicate produces the same pass or failure and starts
   the same rewind prelude at the same `t` in matching runs.
8. A causal-contract failure begins at least one and at most three seconds
   before loop-start apply.
9. A successful first-time Anchor commit ends the current loop; a rejected or
   redundant commit does not.
