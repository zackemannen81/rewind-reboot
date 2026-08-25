# Echo semantics

Status: Proposed
Discovered: 2026-08-25, from an owner request during REW-0016
Owner: unassigned
Activated: not activated. No `REW` identity is claimed for this.

## Discovery context

`docs/PROJECT_BRIEF.md` lists Echo as the first of its open decisions:

> **Echo semantics.** Whether Echo replays recorded actions, is a visual
> silhouette, or is narrative only.

On 2026-08-25 the owner named it as remaining Chapter 1 work, described as
"the ghostlike echo recording and replaying". That is close to choosing the
first of the three, but it is a sentence in a work list rather than a decision,
and the brief requires an explicit one.

The legacy project is not authority here and is also not a starting point:
`docs/concepts_sandbox/legacy-rewind/code-inventory.md` records that transform
sampling at 0.1 s existed, `EchoPlayback` was never called, and no recording was
ever persisted.

## Why it needs a decision before implementation

Echo touches the determinism model directly. `docs/design/loop-and-determinism.md`
states that world configuration at elapsed time `t` is a function of baseline,
Anchors, `t` and this-loop input, and that player actions from loop `N` are
invisible to loop `N+1` except through Anchors.

A replayed Echo is, by construction, information from an earlier loop present in
a later one. Whether that is:

- **not world**, a presentation the player sees but nothing reads, or
- **world**, an object other systems may collide with or react to

is the whole decision, and the two produce different games. The first is safe
under the accepted rule. The second amends it.

Storage is the second question. A per-loop transform recording is the first
thing in this project that grows without bound, and
[`save-and-session.md`](../design/save-and-session.md) currently owns nothing of
that shape.

## Proposed outcome

An ADR deciding what Echo is, followed by an owning document under
`docs/design/` if the answer is anything other than "narrative only". Then, and
only then, an implementation task.

## Why it is outside the active charter

REW-0017 implements ADR-0009 and its charter excludes Echo for exactly this
reason: an agent must not settle an open product decision in passing.

## Dependencies

- [`ADR-0002`](../adr/ADR-0002_world-determinism.md)
- [`loop-and-determinism.md`](../design/loop-and-determinism.md)
- [`save-and-session.md`](../design/save-and-session.md)

## Suggested verification

- Two idle loops match at the same `t` with Echo present, in the same form as
  FL-03, if Echo is decided to be presentation.
- A stated bound on recording size per loop, and what is discarded.
