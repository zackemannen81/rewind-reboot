# Five Loops Test

Status: Accepted
Owns: what a later implementation must demonstrate for the first product
proof
Does not own: game rules. Every criterion below cites one owning design
document.

Run from a clean save, as
[`save-and-session.md`](../design/save-and-session.md). Until an Unreal
project exists, no criterion may be claimed passed.

Identifiers are permanent. Do not renumber.

The backlog twelve-step list was input. Enjoyment is recorded in the
playtest write-up and is not a pass/fail criterion: it is not a game
rule.

## Criteria

### FL-01 — Clean start in 4C

From a clean save, at `t = 0`, the player is in Apartment 4C, the
courtyard gate is closed, courtyard power is off and the generator is
offline.

Owner: [`chapter-1-authored.md`](../design/chapter-1-authored.md)

### FL-02 — Loop end conditions

A started loop ends when the timer expires or when the player dies, and
not otherwise.

Owner: [`loop-and-determinism.md`](../design/loop-and-determinism.md)

### FL-03 — Idle loops match

From a clean save, two loops with no player input have the same world
configuration at the same `t`.

Owner: [`loop-and-determinism.md`](../design/loop-and-determinism.md)

### FL-04 — Code on loop one

On the first loop, a player who listens to the radio in 4C can obtain
`7312`.

Owner: [`chapter-1-authored.md`](../design/chapter-1-authored.md)

### FL-05 — Code persists

After the player obtains `7312` and the loop ends, `radio_code_7312` is
true at the next loop start.

Owner: [`save-and-session.md`](../design/save-and-session.md)

### FL-06 — Keypad

The lock between 4C and the courtyard opens for `7312` and not for
`0000`.

Owner: [`chapter-1-authored.md`](../design/chapter-1-authored.md)

### FL-07 — Chain resets

After a loop in which the fuse was placed in the courtyard socket, the
generator started and the gate opened, and in which the player did not
commit an Anchor, the next loop at `t = 0` has the fuse in neither socket,
the generator offline and the gate closed.

Owner: [`world-state-model.md`](../design/world-state-model.md)

### FL-08 — Patrol clock

At a chosen `t` after loop start, with no this-loop input, patrol phase
matches across two loops.

Owner: [`loop-and-determinism.md`](../design/loop-and-determinism.md)

### FL-09 — Patrol window

The street patrol leaves a 20-second uncovered window on the path toward
the Transit Hub entrance, repeating as a function of `t`.

Owner: [`chapter-1-authored.md`](../design/chapter-1-authored.md)

### FL-10 — Explicit Anchor

Starting the generator, without an explicit commit, does not make
`courtyard_gate_open` active on the next loop. An explicit commit while
the gate is this-loop open does.

Owner: [`anchors.md`](../design/anchors.md)

### FL-11 — Held gate

After that commit, the next loop has the courtyard gate open at `t = 0`
with no this-loop input.

Owner: [`anchors.md`](../design/anchors.md)

### FL-12 — Turnstile

The Transit Hub turnstile is open when `(t modulo 30) <= 2.5`.

Owner: [`chapter-1-authored.md`](../design/chapter-1-authored.md)

### FL-13 — Reach the hub

From a clean save, the player can reach Transit Hub. It takes more than one
loop, and the loop in which the hub is reached has `courtyard_gate_open`
active.

Amended 2026-08-23, identifier intact. It previously required the whole
chain inside one loop, which the chain in `chapter-1-authored.md` makes
impossible on purpose: carrying the fuse to the courtyard empties the
building socket, so the lift is dead and the stairs cost more time than the
loop has. Reaching the hub is what the held gate is *for*, and a criterion
that required it in a single loop was describing a game where the Anchor
bought nothing.

The duration is not restated here. `chapter-1-authored.md` owns it.

Owner: [`chapter-1-authored.md`](../design/chapter-1-authored.md)

### FL-14 — Faster learned run

In the same session, after `radio_code_7312` is true and
`courtyard_gate_open` is active, the player passes the turnstile at a
smaller elapsed `t` than on the first successful FL-13 run.

Unchanged in wording, changed in what it compares. FL-13's first success now
already carries both facts, so this compares two anchored runs and measures
what remains: route knowledge, patrol timing, and knowing which way is fast.
That is a narrower claim than before and a truer one.

Owner: [`chapter-1-authored.md`](../design/chapter-1-authored.md)

### FL-15 — Clean save reachable

A clean save can be reached, after which FL-01 holds.

Owner: [`save-and-session.md`](../design/save-and-session.md)

### FL-16 — Session load

After obtaining `7312` and committing the gate Anchor, quitting and
loading restores those two facts and starts at the loop-start pose, not
at the quit pose.

Owner: [`save-and-session.md`](../design/save-and-session.md)

## Amendments

Identifiers are permanent and none has been renumbered. Criteria have been
amended where the design they cite changed, and the change is recorded here
rather than left to be noticed.

| Date | Criterion | Change |
| --- | --- | --- |
| 2026-08-23 | FL-07 | "routed" became "placed in the courtyard socket", following the carried fuse in `chapter-1-authored.md` |
| 2026-08-23 | FL-13 | No longer requires the whole chain inside one loop, which the chain now makes impossible on purpose. Requires the hub to be reached in a loop with `courtyard_gate_open` active |
| 2026-08-23 | FL-14 | Wording unchanged. Its baseline is now an anchored run, so it measures route knowledge rather than stored facts |

The evidence in `docs/playtests/five-loops-2026-08-22.md` was taken against
the criteria as they read before these amendments. It stays true of that
build and of those criteria, and it is not evidence for these.

## Dropped from the backlog list

| Backlog step | Fate |
| --- | --- |
| Enjoyable | Not a criterion. Recorded in the playtest write-up. No design document owns fun |
| Echo | Out of charter. Not required |
| Insight / board spend | Not in this proof. ADR-0003 |

## What this does not claim

Passing these criteria is not "Chapter 1 is done". It is evidence for
the product hypothesis, once an implementation task runs them.
