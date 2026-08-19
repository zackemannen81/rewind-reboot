# Loop and determinism

Status: Accepted
Owns: loop semantics and world determinism
Does not own: Anchor semantics, save and session, world-state apply order,
authored Chapter 1 facts, Echo
Source decision: [`ADR-0002`](../adr/ADR-0002_world-determinism.md)

A rule that is not in this document is not a loop or determinism rule.

## Loop

A loop is one play of the authored space from start to end.

- A loop starts with the player's body at the loop-start pose, and with the
  world in the configuration ADR-0002 names: authored baseline, then
  Anchors as [`anchors.md`](anchors.md) defines them, then elapsed time
  zero. Apply order is [`world-state-model.md`](world-state-model.md).
- A loop ends when the loop timer reaches its duration, or when the player
  dies. Those are the only end conditions for the first product proof.
- The duration is an authored parameter. This document does not set it.
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

Elapsed loop time is the source of truth for every world clock: patrol
phase, breathing windows, timers that belong to objects, hover and any
other periodic motion.

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
