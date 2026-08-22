# Traversal as a knowledge axis

Status: Proposed
Discovered: 2026-08-22, during REW-0003, from a design hypothesis by MrWhite
Owner: unassigned
Activated: not activated. No `REW` identity is claimed for this.

Design input, not canon. The originating diagram is explicitly not a proposed
layout and its dashed branches — stairs, service corridor, locked shortcut,
alternate courtyard exit, one-way passage — are illustrative types, not routes.
Nothing here changes an Accepted document.

## The hypothesis

That the first product proof has compressed traversal far enough that it can
pass FL-14 while not representing the part of RE:WIND's knowledge progression
that is spatial: which route is fastest, when a periodic route opens, where a
patrol crosses, which passage does not allow a return, and which route puts the
player somewhere at the right world-clock time.

Stated as a chain the eventual game should exercise:

```text
exploration → spatial understanding → timing understanding
  → route optimization → deliberate learned traversal
```

The connective spaces named in the input — corridor, elevator landing, elevator,
lobby, enclosed courtyard, gate, street, exterior passage, station entrance,
station interior, turnstile — are proposed as playable locations rather than
transition filler.

## What canon already supports

More than expected. Four of the five load-bearing pieces already exist.

**Spatial knowledge is already product direction.**
[`PROJECT_BRIEF.md`](../PROJECT_BRIEF.md) states that a player on their fifth
loop is faster "because they know where the fuse is, what the radio says, and
when the patrol looks away". The first of those three is spatial knowledge. The
hypothesis is not new direction; it is direction the first proof does not
exercise.

**The determinism rules already cover route objects.**
[`loop-and-determinism.md`](../design/loop-and-determinism.md) includes "gated
collision" in world configuration at time `t`, and names "breathing windows" and
"timers that belong to objects" among the clocks that must read elapsed loop
time. An elevator that arrives on a schedule, a passage that opens periodically
and a shortcut behind gated collision are already legal and already constrained.
**No new determinism decision is required.**

**Traversal is already a named bucket.**
[`world-state-model.md`](../design/world-state-model.md) gives `PlayerBody` the
contents "pose, stance, traversal state", discarded and replaced at every loop
start. Canon already anticipated traversal state and already decided it does not
persist. Player traversal *knowledge* persisting while traversal *state* resets
is consistent with that, not in tension with it.

**Unstored, player-held knowledge is already load-bearing.**
[`save-and-session.md`](../design/save-and-session.md) records exactly one stored
fact, `radio_code_7312`, and says of the player: "The human tester will also
remember. That does not replace this fact." Patrol timing (FL-09) and turnstile
phase (FL-12) are not stored facts at all. They live only in the player's head,
and FL-14 already depends on them. Route knowledge falls in that same category
and would need no change to the save model.

**One instance already exists.** The Transit Hub turnstile — open 2.5 seconds in
every 30 — is a periodic gate on traversal whose phase the player must learn to
use. It is the hypothesis, implemented once, at a single point.

## What the hypothesis adds

**Branching routes.** [`chapter-1-authored.md`](../design/chapter-1-authored.md)
authors "four connected areas, in access order". That is a single line. There is
exactly one path, so route *choice* does not exist and there is nothing to
optimize. This is the substantive addition.

**Connective space as playable.** Canon says the four areas are "connected" and
never says what connects them. That is silence, not prohibition, and the
hypothesis fills a gap rather than contradicting a rule.

**Route knowledge as a named progression axis**, alongside stored facts and
Anchors.

**Risk-differentiated routes.** "Faster but exposed to a patrol" requires that
exposure cost something. Nothing in canon says what a patrol can do to the
player, and nothing defines what kills the player, although
`loop-and-determinism.md` names death as a loop end condition. This is the same
blocker recorded as lever 5 in
[`loop-pressure-and-interaction.md`](loop-pressure-and-interaction.md).

**Route-holding Anchors.** A locked shortcut or an alternate exit held open
across loops would be an Anchor. [`anchors.md`](../design/anchors.md) states the
legal set has exactly one member and that "No other identifier is an Anchor".

**Irreversibility.** A one-way passage makes route choice a commitment rather
than a preference. Canon has no such object.

## How much of the measured problem this explains

[`loop-pressure-and-interaction.md`](loop-pressure-and-interaction.md) measured
the golden path at about 34 m, roughly 7 seconds of movement in a 420-second
loop. That measurement is the product of two independent compressions, and this
hypothesis addresses one of them.

| Compression | What it is | Addressed here |
| --- | --- | --- |
| Spatial | Distance is negligible against the loop budget | No |
| Decision | The path is a single line with no alternatives | Yes |

The distinction matters. Scaling the space ten times without adding branches
would give a longer walk and still nothing to optimize: the learned run would be
the same line walked without hesitation. Conversely, branches in a 34 m space
would be choices too small to be worth making. Neither compression is a
substitute for fixing the other.

One coupling that the input under-states: traversal scale is not independent of
loop pressure. A longer traversal makes the 420-second budget mean something and
raises the cost of a wasted loop, but the cost of a wasted loop is paid in the
player's real time. A wasted 420-second loop costs seven real minutes, which is
pressure of the tedious kind rather than the tense kind. **Traversal budget and
loop duration have to be decided together.** Deciding either alone will produce
a number that the other one invalidates.

## Where the compression came from

Worth separating, because the two answers are different.

The subsetting was deliberate and declared. `chapter-1-authored.md` opens: "This
is not Chapter 1. It is the subset of Apartment 4C, courtyard, street and
Transit Hub that the first product proof requires."

The ratio was never authored. That document sets loop duration explicitly at 420
seconds and sets **no traversal budget, no distance and no scale**. Nobody
decided 34 metres; it fell out of implementation defaults in
`ARewindProofLayout`. The relationship between the space and the clock — the
thing that determines whether the loop has any pressure — is currently unowned.

That is the smallest true statement of the finding: canon chose to subset, and
canon did not choose the ratio.

## What FL-14 actually proves

FL-14 requires the player to pass the turnstile at a smaller elapsed `t` after
`radio_code_7312` is true and `courtyard_gate_open` is active. Both of those are
*stored* state. They let the player skip the radio, the fuse, the generator and
the gate.

So FL-14 will pass, and it will pass on stored knowledge alone. It demonstrates
that stored knowledge shortens the run. It does not demonstrate that
understanding the world makes the player better at moving through it, because
there is no route to understand.

This is not a defect in FL-14. `five-loops-test.md` already bounds its own
claim: "Passing these criteria is not 'Chapter 1 is done'. It is evidence for
the product hypothesis." The finding is that the evidence is narrower than the
brief's statement of what knowledge progression is, and that the gap should be
stated rather than discovered later.

## Proposed outcome

Not an expansion of the Five Loops Test. `PROJECT_BRIEF.md` makes the first
proof "deliberately smaller than Chapter 1", FL identifiers are permanent, and
the criteria are Accepted. Expanding the proof to cover traversal would mean
doing level design before the loop is proven, which is the ordering the reboot
explicitly rejects.

The proposed shape is a **successor proof**, chartered only after FL-01 to FL-16
pass, that tests route and timing knowledge specifically: a space with at least
two viable routes between the same two points, differing in time and in some
cost, where a player who has learned the world chooses differently from a player
who has not.

Recorded as a candidate, not a decision.

## What would need a decision before any of this is authoritative

Cheapest first. Only one of these is an ADR-scale question today.

1. **A traversal budget**, owned by `chapter-1-authored.md` or a successor. It
   already owns loop duration and the space; it authors one and not the other.
   This is a design-document decision, not an ADR, and it is the smallest change
   that turns the ratio into a decision.
2. **Whether route knowledge is stored or player-held.** Player-held requires no
   change to `save-and-session.md` and matches how patrol timing already works.
   Recording routes on the Anchor board would be a save-model change and would
   reach ADR-0004.
3. **Whether branching routes exist**, and whether connective space is playable.
   A design-document decision about the authored space.
4. **Whether any route object may be Anchored.** `anchors.md` and ADR-0003 are
   explicit that one identifier is the whole legal set, so this needs ADR-0003
   superseded, not amended.
5. **Consequence and death.** Required before any risk-bearing route means
   anything. New ADR. Blocks the stairs, the service corridor and patrol
   exposure entirely.

Determinism is not on this list. `loop-and-determinism.md` already governs every
route object the hypothesis names.

## Prior art, and a caution

The previous project planned this. `ENV-HALL-001` in
[`task_transition_hallway.md`](../concepts_sandbox/legacy-rewind/old-task-material/task_transition_hallway.md)
specifies a hallway and elevator transition between 4C and the courtyard, with
interactive controls and door sequencing. Its status there is `OPEN`; the code
inventory shows it was never built.

That is convergence, and it is also the caution. The previous project authored
connective space in documents while its loop could not reset the world. This
proposal is only safe in the other order.

## Dependencies

- REW-0003 complete, with FL-01 to FL-16 evidence from a clean save.
- A joint decision on traversal budget and loop duration, per the coupling above
  and per [`loop-pressure-and-interaction.md`](loop-pressure-and-interaction.md).
- Item 5 above for any risk-bearing route.
- ADR-0003 superseded for any route Anchor.

## Suggested verification

- A route-choice measurement rather than a completion measurement: on a learned
  run, which route the player took and whether it differed from their first.
- Two players, one briefed and one not, on the same space, compared on route
  rather than on time alone.
- Re-measured golden path in the same form as
  [`loop-pressure-and-interaction.md`](loop-pressure-and-interaction.md), so the
  numbers stay comparable.
- Every periodic route object checked at a stated `t` across two idle loops, in
  the same form as FL-03 and FL-08.

## What this proposal does not decide

It does not adopt the diagram's layout, which its author explicitly did not
propose as canon. It does not add a route, a corridor, an elevator, a shortcut
or a one-way passage. It does not change loop duration, space scale, the Anchor
set, the save model or the FL criteria. It does not modify REW-0003. It records
a hypothesis, the canon that already supports it, the four things that would
have to be decided, and the reason the Five Loops Test should not absorb it.

The originating diagram is not in this repository. If it should be preserved, it
needs a stable path first, and ADR-0005 governs it as a binary.
