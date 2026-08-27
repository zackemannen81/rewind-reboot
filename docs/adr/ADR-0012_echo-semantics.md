# ADR-0012 — Echo semantics

Status: Accepted

Date: 2026-08-25

Owns: what Echo is, and what it is not

## Context

[`docs/PROJECT_BRIEF.md`](../PROJECT_BRIEF.md) has listed Echo as its first
open decision since the reboot began:

> **Echo semantics.** Whether Echo replays recorded actions, is a visual
> silhouette, or is narrative only.

The previous project left the question half-answered in code rather than in a
document. `code-inventory.md` records that transform sampling at 0.1 s existed,
that `EchoPlayback` was never called, and that no recording was ever persisted.
That is design reference, not a starting position.

The question matters more here than the name suggests.
[`loop-and-determinism.md`](../design/loop-and-determinism.md) states that
world configuration at elapsed time `t` is a function of baseline, Anchors, `t`
and this-loop input, and that player actions from loop `N` are invisible to
loop `N+1` except through Anchors. A replayed Echo is by construction
information from an earlier loop present in a later one. Whether that
information is *world* or *presentation* is the whole decision, and the two
produce different games.

A recording is also the first thing in this project that would grow without
bound per loop, and [`save-and-session.md`](../design/save-and-session.md) owns
nothing of that shape.

## Decision

**Echo is narrative only. It is not a recording, and it is not replayed.**

Echo may be referred to in copy, in the fiction, and in what the world implies
about earlier loops. Nothing samples the player's transform, nothing persists a
path, and nothing renders a ghost following a route the player previously took.

## Alternatives considered

**Presentation-only replay: a recorded path rendered as a ghost that nothing in
the world reads.** Safe under the accepted determinism rule without amending
it, and visually strong. Rejected because it still requires per-loop transform
recording, a storage bound, and a rule about what happens to recordings across
a clean save — real cost for something the product has not yet shown it needs.
It remains the most likely candidate if this record is ever amended.

**Echo as world: a ghost other systems can see, collide with or react to.**
Rejected. It amends [`ADR-0002`](ADR-0002_world-determinism.md) by opening a
second route for information to cross a loop boundary, when Anchors are
currently the only one, and Anchors are deliberately scarce under
[`ADR-0008`](ADR-0008_what-an-anchor-is-worth.md). Adopting it would mean
reopening the determinism model before the loop has been shown to be fun.

**Defer.** Rejected because the question has been open since the reboot
started and was blocking task decomposition. An explicit "narrative only" can
be amended later on evidence; an open question cannot be built against at all.

## Consequences

- No Echo implementation task is chartered, and none should be proposed
  without amending this record.
- `URewindSessionSubsystem` gains no recording bucket. Save size stays bounded
  by knowledge facts and Anchors.
- The determinism rule in `loop-and-determinism.md` is unchanged. Anchors
  remain the only route by which anything crosses a loop boundary.
- The player's sense that earlier loops happened must come from the world and
  from copy, which raises the value of the message system delivered by
  REW-0018 and of the loop-break signature delivered by REW-0017.
- If a later playtest shows the loop needs a visible ghost to feel like a loop,
  that is evidence for amending this record, and the presentation-only option
  above is where to start.

## Amendment, 2026-08-27

**An authored Echo performance is permitted. Derivation from the player is
not.**

The original text says what Echo is not: not a recording, not replayed, nothing
samples the player's transform, nothing persists a path, and nothing renders a
ghost following a route the player previously took. Every one of those clauses
stands unchanged.

What it never said is what Echo may be, and the owner's Chapter 2 script work
needs that line drawn. A hand-placed silent figure performing a hand-animated
action is not a recording and does not follow a route the player took: it was
composed before the player arrived. It is staging, in the same sense that a
decal or a light is staging. This amendment records that it is allowed, so the
question is a rule rather than an argument the next reader has to have.

The boundary, stated once:

- **Permitted:** authored figures, authored actions, keyed to loop time or to a
  knowledge fact, identical every time they play.
- **Not permitted:** anything derived from what the player did — sampling a
  transform, storing a path, replaying a route, reacting to the player, or
  varying with loop count.

Nothing in the Consequences section above changes. `URewindSessionSubsystem`
still gains no recording bucket, save size stays bounded by knowledge facts and
Anchors, and Anchors remain the only route by which anything crosses a loop
boundary. An authored Echo costs animation and staging, and no state at all.

The game rule this permits is owned by
[`../design/echo.md`](../design/echo.md), which did not exist when this record
was written. The frame those performances reveal is owned by
[`../design/the-macro-loop.md`](../design/the-macro-loop.md).

## Status of related records

[`echo-semantics.md`](../backlog/echo-semantics.md) is resolved by this record.
`docs/PROJECT_BRIEF.md`'s open-decision list drops Echo; the licence and the
Unity project's retirement remain open.
