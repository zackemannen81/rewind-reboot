# ADR-0002 — World determinism

Status: Accepted
Date: 2026-08-19
Deciders: MrWhite
Supersedes: nothing
Superseded by: nothing

## Context

The first product proof asks whether it is enjoyable to live in the same
world several times and become faster because you know more. That question
has no meaning unless "the same world" is specified.

The imported GDD states both sides. It promises pattern-driven patrols a
player can learn, and it promises that small details change between loops:
a door that was closed is open, a statue moves, a poster changes. Those are
different games. Conflict C1 in
[`design-conflicts.md`](../concepts_sandbox/legacy-rewind/design-conflicts.md)
names the question. `docs/PROJECT_BRIEF.md` already chose the product
slogan; it did not choose the exact rule.

Two further holes would let the slogan survive on paper while the world
still changed.

**Loop index as a world input.** The GDD radio becomes clearer on later
loops. That is the world reading how many times the player has died. If
that is allowed, "details shift" returns as content scheduling.

**Frame time as a world clock.** The legacy drone moved with
`Time.deltaTime` and hovered on global `Time.time`, with no loop-start
reset. A player could not learn its pattern, because it did not have one.
Unreal will not fix that. ADR-0001 already recorded the cost.

Anchors, if they exist, are an explicit exception: a chosen override that
survives a reset. Their semantics are ADR-0003. This decision leaves that
slot and does not fill it.

## Decision

The world does not learn the player. The player learns the world.

At any elapsed time in a loop, world configuration is determined by:

1. the authored baseline for that space;
2. the Anchors that ADR-0003 says are active;
3. elapsed time in the current loop;
4. player actions taken during the current loop.

It is not determined by loop count, by player actions from earlier loops
except through those Anchors, by wall-clock time, by frame count, or by a
per-loop random seed.

The player's body is world. It resets. The player's knowledge is not world.
What knowledge is, and what a clean save is, belongs to ADR-0004.

The rule that implementation must satisfy: two runs that share baseline,
Anchors, elapsed loop time and this-loop player input produce the same
world configuration.

The owning game rule is restated in
[`docs/design/loop-and-determinism.md`](../design/loop-and-determinism.md).

## Alternatives considered

**Authored details shift between loops.** Rejected. If a statue, a door or
a poster can move without an Anchor, knowledge of their last position is
worthless. That is not the product in `docs/PROJECT_BRIEF.md`.

**Loop index may drive authored beats, but not AI or physics.** Rejected.
The GDD radio drip is the example. It makes the world a function of how
many times the player has failed, which is the world learning the player
under another name. It also gives later content a path around this ADR:
any change can be labelled a beat. The product line "the game does not
hand them anything for the repetition" forbids that path.

**Defer until an Unreal prototype exists.** Rejected. The Five Loops Test
cannot be specified, and no implementation task can be chartered, while
this is open. The previous patrol already showed what an unspecified clock
produces.

## Consequences

**Accepted costs:**

- Radio code, patrols and puzzle objects are fully present from the first
  loop. Pacing by drip across loop count is not a tool this project has.
- A first-time player who listens can hear `7312` on loop one, if the
  authored content later places it there. The reward for loop five is
  speed, not a newly completed broadcast.
- Implementation must clock the world from elapsed loop time. Tick-delta
  motion and global engine time are defects, not engine defaults to keep.

**Accepted benefits:**

- A player can learn a patrol, a window and a chain of causes.
- The Five Loops Test has a defined meaning of "the same world".
- Frame-rate variance cannot be mistaken for design.

**Consequences that bind future tasks:**

- No task may make world objects read loop count, or store last-loop
  player behaviour, except by the Anchor rule in ADR-0003.
- No task may use engine global time or per-frame delta as the source of
  truth for patrol phase, puzzle windows or other world clocks.
- A change that would make the world different on loop N because it is
  loop N is a new ADR, not a content tweak.
- ADR-0003 may add overrides. It may not add loop-index or last-loop
  memory outside those overrides.
- Exact loop length, radio wording, patrol paths and window timings are
  not decided here. They are authored content, and they must obey this
  rule once written.
