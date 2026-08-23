# Anchors

Status: Accepted
Owns: what an Anchor is, what makes one worth having, when it is
committed, what it costs, which overrides the first product proof allows
Does not own: apply order, save format, authored puzzle steps that
produce the gate-open state, Echo, Insight as a later system
Source decisions: [`ADR-0003`](../adr/ADR-0003_anchor-semantics.md) and
[`ADR-0008`](../adr/ADR-0008_what-an-anchor-is-worth.md), with commit as a
loop boundary decided by
[`ADR-0009`](../adr/ADR-0009_event-driven-loop-termination.md)

A rule that is not in this document is not an Anchor rule.

## Meaning

An Anchor is a named override of baseline world configuration. It is
player-chosen. It is the only legal leak of last-loop play into this-loop
world.

At loop start, after baseline is applied and before the player spawns,
every active Anchor's override is true.

## What makes an override worth anchoring

**Holding it must release something the loop would otherwise have to
commit, and that thing must have another use.**

Name the resource. Name its two uses. If either cannot be named, the
override is a shortcut and not an Anchor.

An override that only makes an existing route faster is not an Anchor. It
may be worth having as some other mechanism, and it does not use this one.

The reason is that a rewind whose exceptions are conveniences is a save
state. The choice is only a choice while almost everything is taken back.

## Scarcity

Few Anchors may be active at once, and the number is small enough that
choosing which to hold costs the player something. This is a design rule,
not a consequence of the first proof's scope. The first proof allows one.

## First product proof

The legal set has one member:

| Identifier | Override |
| --- | --- |
| `courtyard_gate_open` | The courtyard gate is open at `t = 0` |

No other identifier is an Anchor. A flag, a string in a list, or a
puzzle result is not an Anchor unless it is this identifier, committed
by the rule below.

## Commit

The player may commit `courtyard_gate_open` only when all of these hold:

1. The courtyard gate is open in the current loop.
2. It is open because of this-loop play, not because the Anchor was
   already active at loop start.
3. The player performs an explicit commit action.

If the loop ends without that action, the override is not committed.
Opening the gate, starting the generator, or routing the fuse does not
commit it.

At most one Anchor is active. This identifier is that one. Committing it
again while it is active is a no-op.

A successful first-time commit writes the override and latches the authored
one-to-three-second rewind prelude. The next loop applies the newly active
override before the player spawns. A rejected commit, or committing the already
active identifier again, does not end the loop.

## Cost

Insight does not exist in the first product proof. There is no spendable
currency. The cost is producing the open gate and choosing to keep it.

## Testable statements

1. Every active Anchor names a resource it releases and two uses that
   resource has. An Anchor that cannot is a shortcut.
2. A generator success does not, by itself, make `courtyard_gate_open`
   active on the next loop.
3. After an explicit commit while the gate is this-loop open, the next
   loop has the gate open at `t = 0` with no this-loop input.
4. After a loop where the gate opened and the player did not commit, the
   next loop has the gate closed at `t = 0`.
5. No object other than the courtyard gate is held by an Anchor.
6. A successful first-time commit ends the current loop after the rewind
   prelude; a rejected or redundant commit does not.
