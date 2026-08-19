# ADR-0003 — Anchor semantics for the first product proof

Status: Accepted
Date: 2026-08-19
Deciders: MrWhite
Supersedes: nothing
Superseded by: nothing

## Context

The Five Loops Test needs one world change that survives a reset: the
courtyard gate held open. Conflict C3 asks what an Anchor is. Conflict C4
asks whether Insight exists. ADR-0002 already forbids any other form of
last-loop memory.

The imported GDD prices Anchors in Insight, chosen at a board in 4C, one
or two per chapter. The Unity code wrote a string automatically after the
generator ran, with no player choice and no currency.

An automatic flag is not a player decision. An Insight economy is a second
system the first proof does not need. The test only needs: the player
earns a state, chooses to keep it, and sees it present at the next loop
start.

This decision is for the first product proof. It does not invent Anchors
for later chapters.

## Decision

An Anchor is a player-chosen, named override of baseline world
configuration. It is applied at loop start after baseline, before the
player spawns. It is the only way last-loop play may affect this-loop
world.

For the first product proof:

- There is exactly one legal override: courtyard gate held open.
- The player may commit it only if that gate is open in the current loop
  because of this-loop play, and only by an explicit action. A successful
  generator start does not commit it.
- Insight does not exist in this proof. The Anchor has no spendable cost.
  The cost is producing the state and choosing to keep it.
- At most one Anchor is active. Committing the gate override is that one.

The owning game rule is restated in
[`docs/design/anchors.md`](../design/anchors.md). Apply order is owned by
[`docs/design/world-state-model.md`](../design/world-state-model.md).

## Alternatives considered

**Automatic lock after the golden path.** Rejected. The Unity orchestrator
did this. The player never chooses, so "create the first explicit Anchor"
is not a test.

**Insight as currency, spent at a board.** Rejected for this proof. It
adds earn, spend and a board before the loop itself is shown to be fun.
Whether Insight exists later is not decided here.

**Anchors may override any object the designer lists, from the start.**
Rejected for this proof. One named override is enough to test the slot
ADR-0002 left open. A general override table is a later task.

**No Anchors in the first proof.** Rejected. The candidate sequence uses
the held gate as the visible difference between a learned loop and a
first loop. Without that slot, the only persistence is knowledge, and
the world never shows that the player changed it.

## Consequences

**Accepted costs:**

- The first proof has no Insight, no board spend, and no second Anchor.
- Designers cannot hold the generator, a fuse route or a patrol with an
  Anchor until a later decision extends the legal set.
- Implementation must distinguish "gate is open this loop" from "gate
  override is committed".

**Accepted benefits:**

- The Five Loops Test has one visible, player-chosen world change.
- ADR-0002 remains intact: nothing else leaks across the reset.
- A later Insight economy can be added without rewriting this override.

**Consequences that bind future tasks:**

- No task may auto-commit an Anchor from a puzzle result.
- No task may introduce Insight, an Anchor cap other than one, or a
  second override as part of the first product proof.
- Extending the legal override set, or adding Insight, is a new ADR.
- Loop-index and last-loop AI memory remain forbidden. An Anchor is not
  a place to store them.
