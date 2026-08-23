# The fuse dilemma, the radio's cost, and the elevator

Status: Proposed
Discovered: 2026-08-23, from the project owner describing the intended Chapter 1
structure, and confirmed against the imported GDD
Owner: unassigned
Activated: not activated. No `REW` identity is claimed for this.

This is the puzzle structure the first proof was always meant to have. It is
recorded because the implemented chain does not have it, and because it answers
the loop-pressure question better than making the space larger does.

Nothing here is authority. The route out is a decision and a restatement in
`docs/design/`.

## The structure

As described by the owner, in three loops:

**Loop one is the radio.** The player tunes to a frequency, hears static, then
cryptic fragments that spell out a code. Listening takes long enough that the
loop ends having achieved that and nothing else.

**Loop two is the dilemma.** The code is known, so the door opens immediately.
The player takes the fuse and carries it to the courtyard generator, which opens
the gate. But the fuse is what powers the elevator, so taking it forces the
stairs, and the stairs are slow enough that the loop ends before the player
reaches the end.

**Loop three is the resolution.** The gate is held open by an Anchor, so the
fuse is not needed for the generator at all. It stays where it is, the elevator
works, and there is time to reach the end.

## Why it is better than the current chain

The implemented chain has no dilemma. Every object is a press of `E`, the fuse
is routed rather than carried, and there is no cost to any choice. That is why
the golden path measures 34 metres and why FL-14's number turned out to mean
nothing.

This structure produces pressure from mechanics instead of distance:

- **One resource, two consumers.** The fuse powers either the elevator or the
  courtyard. It cannot do both, so the player must choose and live with it.
- **The Anchor's value is that it frees the resource.** It is not "a step you
  skip"; it is what converts a dilemma into having both. That is a much stronger
  reason for Anchors to exist than the current gate-stays-open framing.
- **Knowledge removes a whole loop.** The radio costs a loop the first time and
  nothing afterwards, which is knowledge-as-progression stated as time.

Each loop teaches exactly one thing, and each solution creates the next problem.

## What the imported GDD already contains

Confirmed by reading `original-gdd/GDD_About.md`, which is non-authority and is
cited here as evidence that the structure is recoverable rather than invented:

- The fuse is a loose single-use item found in a kitchen drawer.
- **"Sätt i säkring → hiss online eller gården får svagt ström (du väljer)."**
  The choice between elevator and courtyard power is explicit in the source.
- The radio has channels; channel 1 is static and channel 3 carries the code.
- The keypad is hidden behind a painting.
- The generator has a mini-puzzle: two levers and a three-second hold.
- The stairwell has a guard drone passage every 90 seconds.
- The guard route leaves a 20-second window, and the turnstile breathes every 30
  seconds for 2 to 3. Both are already canon in `chapter-1-authored.md`.

So most of the detail exists and would be restated rather than designed.

## Where it conflicts with accepted rules

Three conflicts, and they are not equal.

**1. The GDD makes the radio clearer between loops.** "Radio kanal 3 nu
tydligare", and "ljudstegrar mellan looparna: brus → ord → siffror". That is a
world object reading loop count, which
[`ADR-0002`](../adr/ADR-0002_world-determinism.md) forbids and
[`chapter-1-authored.md`](../design/chapter-1-authored.md) restates as "it does
not depend on loop count".

**The owner's version resolves this without losing anything.** The radio does
not change; listening takes time, and the player learns. The escalation is in
the player rather than in the world, which is exactly what this project says
progression is. This is the version to restate, and the GDD's is the version to
leave in the sandbox.

**2. Insight.** The GDD spends 1 Insight on the Anchor.
[`ADR-0003`](../adr/ADR-0003_anchor-semantics.md) decided Insight does not exist
in the first proof. The structure above does not need it: the cost of the Anchor
is producing the open gate and choosing to keep it, which is what ADR-0003
already says.

**3. The fuse is routed, not carried.** `chapter-1-authored.md` currently says
"the fuse can be routed to courtyard power once this loop", with one consumer
and no choice. A carried, single-use fuse with two consumers supersedes that.

## What would have to be decided

- How long listening to the radio takes, and whether tuning is a mechanic or a
  single interaction. This is interaction cost, which
  [`loop-pressure-and-interaction.md`](loop-pressure-and-interaction.md) lists
  as lever 4 and which nothing has decided.
- Whether the fuse is carried state on `PlayerBody`, which
  `world-state-model.md` discards at every loop start, or something else. If it
  is PlayerBody, a carried fuse returns to its drawer each loop for free, which
  is the behaviour this structure wants.
- What the elevator is: a timed lift, a door, or a fast route. And what the
  stairs cost in time relative to it.
- Whether the stairwell's drone matters, which reaches consequence and death,
  and nothing defines those.

## Effect on work already decided

**This makes the 60-metre knowledge-saving rule the wrong shape.** That rule was
authored on 2026-08-23 under REW-0004 and expresses the saving as path length.
The saving in this structure is not distance: it is a loop spent listening and a
slow route taken because the fuse is elsewhere. The rule's substance survives and
its units do not, so it should be stated in time rather than in metres.

**It also means the spatial rebuild should not start first.** A space rebuilt
for the current chain has no stairwell, no elevator and no drawer, and would be
rebuilt again. REW-0004's camera work is independent of this and stands; its
spatial rebuild is not.

## Suggested verification

- A run where the player takes the fuse and demonstrably cannot finish, followed
  by a run where the Anchor makes the fuse unnecessary and they can. That single
  pair is the whole structure, and it is what FL-14 was trying and failing to
  measure.
- The radio's cost measured as elapsed loop time, not asserted.

## What this proposal does not decide

It does not change the fuse, the radio, the space, or any accepted rule. It does
not activate the elevator or the stairwell. It records a structure, the evidence
that it is recoverable, and the three places it disagrees with decisions this
repository has already made.
