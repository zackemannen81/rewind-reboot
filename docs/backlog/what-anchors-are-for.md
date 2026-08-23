# What Anchors are for

Status: Proposed
Discovered: 2026-08-23, from the project owner articulating the game's thesis
Owner: unassigned
Activated: not activated. No `REW` identity is claimed for this.

`anchors.md` says what an Anchor **is**: a named, player-chosen override of
baseline, the only legal leak of last-loop play into this-loop world. It does
not say what makes one worth having, or how many there should be. This records
both, and neither is authority yet.

## What is already decided, and agrees

The owner described the game as three layers of state across a rewind: almost
everything returns, the player's knowledge does not, and a very few explicitly
chosen things refuse to come back.

That model is already canon.
[`world-state-model.md`](../design/world-state-model.md) has exactly those
buckets — LoopWorld and PlayerBody discarded, Knowledge persisting, Anchors
persisting as the committed override set. Nothing in this proposal needs to
change it.

What follows is about the two questions that model does not answer.

## An Anchor's value is that it frees a resource

The claim, in the owner's words: *A is required for B, but if B is anchored,
the next loop does not need A, which frees A for C, which opens a new route to
D.*

An Anchor is therefore not an unlock. It is a change to **the consequences of
other actions in the next rewind.**

Chapter 1's chain is the concrete case, recorded in
[`fuse-radio-and-elevator.md`](fuse-radio-and-elevator.md):

```text
loop B   fuse -> generator      gate opens, but the building loses the lift
         (A spent on B)         so the slow stairs are the only route

anchor   the gate refuses to rewind

loop C   the gate is already open, so the fuse is never needed for it
         (A is free)            it stays in the building, the lift works,
                                and a route exists that was not practical before
```

The new route in loop C was not unlocked. It became *possible* because a
resource stopped being committed elsewhere. That is a different and better shape
than a progression list, and it is the thing that distinguishes this from a
loop game where repetition simply grants permissions.

It also explains what an Anchor should cost: producing the world state worth
keeping. `anchors.md` already says exactly that and gives no reason. This is the
reason.

## Anchors must be scarce

If ten things per loop can be carried through, the rewind stops being a rewind
and becomes an oddly-shaped save state. The tension only exists while almost
everything is taken back and the player occasionally gets to say *no, that one
stays.*

Scarcity is therefore a design rule and not a scope limit. ADR-0003 currently
allows exactly one Anchor, and its stated reason is the first proof's scope. If
that number is later raised, this is the argument that should bound it, and the
bound should be small enough that choosing is painful.

## Distance is a mechanic

The route the owner described is `4C → corridor → stairs or lift → lobby →
courtyard → generator and gate`. The stairs-or-lift choice is only meaningful
because the two cost different amounts of time, and the fuse decides which is
available.

So the space is not being enlarged to feel more like a game. **The distance is
what makes the resource choice cost anything.** A larger space with no such
choice would be the expensive version of the same mistake this project already
measured.

This is consistent with the timing rule now in
[`chapter-1-authored.md`](../design/chapter-1-authored.md), which requires a
naive run to take at least one turnstile period longer than a learned one and
deliberately does not say how.

## Chapter 1 is the tutorial for the whole game's language

Radio, fuse, stairs, lift and gate are the first lesson in what time takes back,
what the player carries through it, and what can be made to refuse. Later
chapters stack the same three layers rather than introducing new ones.

If that holds, Chapter 1's chain is not merely the first content. It is the
place the game's grammar has to be legible, which raises the bar for it and is
worth stating before it is built.

## What would have to be decided

- Whether `anchors.md` gains a statement of what an Anchor is worth, and whether
  the scarcity argument supersedes ADR-0003's scope-based reason or sits beside
  it.
- The number, when it is ever raised above one, and the rule that bounds it.
- Whether "frees a resource" is a design guideline or a testable property that a
  candidate Anchor has to satisfy.

## What this proposal does not decide

It does not change `anchors.md`, ADR-0003, or the Anchor count. It does not
adopt the Chapter 1 chain, which is a separate proposal. It records the reason
Anchors exist, which the repository currently states nowhere.
