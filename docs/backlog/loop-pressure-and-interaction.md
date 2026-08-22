# Loop pressure and interaction cost

Status: Proposed
Discovered: 2026-08-22, during REW-0003, from an editor play by MrWhite
Owner: unassigned
Activated: not activated. No `REW` identity is claimed for this.

This proposal records a measured observation and does not decide anything.
Loop duration and the authored space are owned by
[`chapter-1-authored.md`](../design/chapter-1-authored.md), and that document is
Accepted. Nothing here changes it.

## Discovery context

An editor play on 2026-08-22 produced the observation that the authored chain
"feels like walk to everything, press E, and it will be fine". The measurements
below were taken from the source afterwards and support it.

Actor positions are from `ARewindProofLayout::EnsureContents` and
`ARewindFourCBlockout::GetLoopStartLocation`. Straight-line distances along the
golden path, ignoring the small detours that walls and the door impose:

| Leg | Distance |
| --- | --- |
| loop-start pose to radio | 222 cm |
| radio to fuse box | 440 cm |
| fuse box to code lock | 620 cm |
| code lock to generator | 457 cm |
| generator to courtyard gate | 658 cm |
| gate to turnstile | 1040 cm |
| **Total** | **3437 cm, about 34 m** |

`ARewindCharacter` sets `MaxWalkSpeed` to 500 cm/s. The entire golden path is
therefore **about 7 seconds of movement inside a 420-second loop**, or 1.6% of
the loop budget.

Consequences that follow from that number:

- The loop timer applies no pressure. A player can stand still for six minutes
  and still complete FL-13.
- The only friction anywhere in the chain is the patrol window (FL-09, a
  20-second uncovered window) and the turnstile (FL-12, open 2.5 seconds in
  every 30). The turnstile's worst case is a 27.5-second wait, which the budget
  absorbs 15 times over.
- Every object in Apartment 4C is within a few metres of the loop-start pose,
  so obtaining `7312` costs a short walk and one key press rather than a search.
- Being wrong costs nothing, and being slow costs nothing. There is no reason
  for the player to feel that knowing the answer was worth anything, even though
  FL-14 will correctly measure that they were faster.

## Why this is not a defect in the implementation

The build matches the design. `chapter-1-authored.md` says 420 seconds, and it
authors the space, the patrol window and the turnstile cycle exactly as they are
implemented. The chain does what the accepted document says it does.

The observation is that the *rules as written* leave the loop without pressure.
That is a design finding, and it belongs to the design documents, not to
REW-0003.

## Why it is outside the active charter

- REW-0003's charter forbids inventing or rewriting game rules, and forbids
  filling a hole in passing.
- [`five-loops-test.md`](../acceptance/five-loops-test.md) states that enjoyment
  is recorded in the playtest write-up and is not a pass/fail criterion. This
  finding is exactly that kind of material.
- FL-01 to FL-16 are all still demonstrable as written. Nothing here blocks the
  charter, so the routed destination is the backlog, not a child task.

## Proposed outcome

A later decision on whether the first proof's loop should exert pressure, and
if so which lever moves. The candidate levers, listed so that a later task does
not have to rediscover them, none of them recommended here:

1. **Loop duration.** 420 seconds against a 34 m space is the widest gap. A
   shorter loop makes knowledge worth something immediately.
2. **Space scale.** A larger authored space at the same duration reaches the
   same result and costs level work.
3. **Movement speed.** 500 cm/s is an engine-template run speed. It interacts
   with FL-13 and FL-14 timings, so it is not a free knob.
4. **Interaction cost.** Interactions that take time, or that can be failed,
   turn a press into a decision. FL-06 already has a failure case in `0000`.
5. **Consequence.** The patrol currently cannot cost the player anything. A
   patrol that can end a loop would give the FL-09 window a reason to exist.

Lever 5 touches loop-end conditions, which
[`loop-and-determinism.md`](../design/loop-and-determinism.md) owns and states
as timer expiry or player death only. Nothing currently defines what kills the
player, and `Rewind.EndLoopDeath` is a console command with no in-world cause.
Any work on lever 5 has to decide that first, in an owning document.

## Dependencies

- REW-0003 complete, with FL-01 to FL-16 evidence from a clean save. The proof
  should run against the rules as accepted, so that the finding is measured
  rather than assumed.
- A decision changing `chapter-1-authored.md`, or a superseding document, before
  any lever moves.
- Levers 3 and 5 additionally depend on
  [`loop-and-determinism.md`](../design/loop-and-determinism.md).

## Suggested verification

- Re-measure the golden path after any lever moves, in the same form as the
  table above, so that the two numbers are comparable.
- A playtest that records where the player spent the loop, not only whether
  they finished it.
- FL-13 and FL-14 re-run, since both are stated in elapsed loop time and both
  are sensitive to every lever listed.

## What this proposal does not decide

It does not change loop duration, space scale, movement speed, interaction cost
or loop-end conditions. It does not claim the Five Loops Test is invalid: FL-01
to FL-16 remain demonstrable exactly as written, and running them is still the
next step. It records one measurement and one observation so that the playtest
write-up has something to compare against.
