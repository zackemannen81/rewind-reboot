# ADR-0008 — What an Anchor is worth

Status: Accepted
Date: 2026-08-23
Deciders: MrWhite
Supersedes: nothing
Superseded by: nothing

## Context

[`ADR-0003`](ADR-0003_anchor-semantics.md) decided what an Anchor **is**: a
player-chosen override of baseline, one legal identifier for the first proof, no
Insight. [`anchors.md`](../design/anchors.md) restates that as rules.

Neither says why an Anchor should exist, or how many there should be once scope
stops being the reason. That gap showed up as a result rather than as an
argument. The Five Loops Test passed FL-10 and FL-11, which prove the commit
mechanism works, and the playtest record shows the loop it produced had no
pressure at all. The mechanism was right and what it was applied to was thin.

The project owner then stated the thesis the mechanism was always meant to
serve: a rewind takes almost everything back, the player carries knowledge
forward, and a very few explicitly chosen things refuse to return. The value of
refusing is not that a step is skipped. It is that a resource stops being
committed, so the next loop can spend it elsewhere.

Stated as a chain: A is required for B. If B is anchored, the next loop does not
need A, which frees A for C, which opens a route to D.

## Decision

**An override is worth anchoring when holding it releases something the loop
must otherwise commit, and that thing has another use.**

The test is concrete and a candidate Anchor either passes it or does not. Name
the resource. Name its two uses. If either cannot be named, the override is a
shortcut and not an Anchor.

An override that only makes an existing route faster is not an Anchor. It may
be worth having as some other mechanism, but it does not use this one, because
a rewind whose exceptions are conveniences is a save state wearing a costume.

**Anchors stay scarce, and scarcity is a design rule rather than a scope
limit.** The tension exists only while almost everything is taken back and the
player occasionally gets to say that one thing stays. If many overrides survive
each loop, the rewind stops being a rewind. ADR-0003's count of one stands for
the first proof; this record is the argument that bounds the number whenever it
is raised, and the bound must stay small enough that choosing hurts.

## The consequence for the Anchor that exists

`courtyard_gate_open` **does not pass the test as the chain is currently
implemented.** Holding the gate open removes the fuse and generator steps and
leaves every route available, only faster. There is no contested resource, so
there is nothing to name.

It does pass under the chain in
[`fuse-radio-and-elevator.md`](../backlog/fuse-radio-and-elevator.md), where the
fuse powers either the lift or the courtyard: the resource is the fuse, its two
uses are the lift and the generator, and holding the gate is what lets the fuse
stay in the building.

This is not a defect in ADR-0003 or in the implementation. FL-10 and FL-11 test
that a commit survives a loop boundary, and it does. What this record says is
that the thing being committed was not yet worth committing, which is a design
gap and is exactly what the playtest measured.

## Note, 2026-08-27

The section above was written against the chain as it stood on 2026-08-23. The
contested-fuse chain it names as the version that passes has since been
implemented: one carried fuse, two exclusive sockets, the building socket
powering the lift and the ground socket enabling the generator. `ADR-0008`'s
test is therefore met by the implementation, and `docs/CURRENT_STATUS.md`
records it as met.

The section is left as written, because the reasoning is the point and because
other records cite it. This note exists so that the next reader does not take a
statement about August 23rd as a statement about now.

What remains open is evidence rather than design: FL-18 has no recorded PIE
run, so a first-time commit of `courtyard_gate_open` has been implemented and
tested in automation but never played.

## Alternatives considered

**Leave the reason unstated.** Rejected. The gap produced a Five Loops Test that
passed while its central mechanic bought the player almost nothing, and nobody
could point at a rule that had been broken, because none had been.

**Make it a guideline rather than a test.** Rejected. A guideline cannot be
failed, and the one Anchor this project has would have quietly kept failing it.
A test that the existing content fails on its first application is worth more
than a principle everything satisfies.

**Bound the count numerically now.** Rejected as premature. The first proof has
one Anchor and no pressure to raise it. Deciding that a later chapter may hold
three would be inventing a number to sound decided.

**Price Anchors with Insight instead of scarcity.** Rejected. ADR-0003 removed
Insight from this proof, and a currency turns the choice into arithmetic. The
cost of an Anchor is producing the world state worth keeping and giving up the
alternatives, which is a stronger cost than a number.

## Consequences

**Accepted costs:**

- The Chapter 1 chain has to be rebuilt around a contested resource before the
  one Anchor this project has is worth having. That work is REW-0005 and the
  spatial rebuild that follows it.
- Designing an Anchor becomes harder. "This would be convenient to keep" stops
  being a sufficient reason, and every candidate has to name its resource.
- Some overrides that would be pleasant will not qualify, and will either be
  cut or need a different mechanism.

**Accepted benefits:**

- A candidate Anchor can be checked rather than argued about.
- The reason the mechanism exists is written down, so a later reader does not
  have to reconstruct it from a chain of playtest numbers.
- It explains the Five Loops Test result rather than leaving it as an anomaly:
  FL-14's number was meaningless because the Anchor released nothing.
- Scarcity gains an argument that survives the first proof's scope, so the
  count is bounded by design rather than by what happened to be convenient.

## What this does not decide

The Anchor count beyond the first proof's one. The Chapter 1 chain itself, which
is REW-0005's remaining work. Whether a released resource must be a physical
object, or may be time, access or knowledge. Consequence and death, which
nothing defines.
