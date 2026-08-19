# Anchors

Status: Accepted
Owns: what an Anchor is, when it is committed, what it costs, which
overrides the first product proof allows
Does not own: apply order, save format, authored puzzle steps that
produce the gate-open state, Echo, Insight as a later system
Source decision: [`ADR-0003`](../adr/ADR-0003_anchor-semantics.md)

A rule that is not in this document is not an Anchor rule.

## Meaning

An Anchor is a named override of baseline world configuration. It is
player-chosen. It is the only legal leak of last-loop play into this-loop
world.

At loop start, after baseline is applied and before the player spawns,
every active Anchor's override is true.

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

## Cost

Insight does not exist in the first product proof. There is no spendable
currency. The cost is producing the open gate and choosing to keep it.

## Testable statements

1. A generator success does not, by itself, make `courtyard_gate_open`
   active on the next loop.
2. After an explicit commit while the gate is this-loop open, the next
   loop has the gate open at `t = 0` with no this-loop input.
3. After a loop where the gate opened and the player did not commit, the
   next loop has the gate closed at `t = 0`.
4. No object other than the courtyard gate is held by an Anchor.
