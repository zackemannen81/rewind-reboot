# Five Loops Test

Status: Proposed
Discovered: 2026-08-19 during REW-0001
Owner: unassigned

## Discovery context

`docs/PROJECT_BRIEF.md` names the Five Loops Test as the whole of the current
milestone. `docs/concepts_sandbox/legacy-rewind/reboot-plan-source.md` names it
as the task that should follow canonical project state. REW-0001 establishes
that state and must not absorb implementation, design authority or acceptance
criteria.

## Proposed outcome

A bounded task, suggested identity REW-0002 once claimed, whose goal is a
testable answer to: is it enjoyable to experience the same deterministic world
several times and become objectively better at it because you know more?

The reboot plan's candidate sequence, from a clean save, is:

1. Wake in 4C.
2. Play a real loop.
3. See the world reset deterministically.
4. Discover `7312`.
5. Keep that knowledge after reset.
6. See fuse, generator and gate reset correctly.
7. Observe a reproducible patrol.
8. Learn and use that patrol.
9. Create the first explicit Anchor override.
10. Start the next loop with the courtyard gate already held.
11. Reach Transit Hub.
12. Run the same sequence measurably faster once the solution is known.

That list is a proposal. It is not acceptance until a task writes
`docs/acceptance/` and the design documents those steps depend on.

## Why it is outside the active charter

REW-0001 may not implement gameplay, create an Unreal project, write design
authority, or write acceptance criteria.

## Dependencies

- REW-0001 complete, so the repository is honest about what exists.
- ADR-0001 (engine) is already accepted.
- World determinism and Anchor semantics are open in
  `docs/PROJECT_BRIEF.md` and block any task that would treat them as rules.
- Asset storage is open and blocks the first `.uasset`.
- Echo semantics are open. The candidate sequence above does not require Echo;
  if a later charter adds it, that is a scope event.

## Suggested verification

Named playtest from a clean save against written acceptance criteria. Until an
Unreal project exists, no build gate can be claimed.

## What this proposal does not decide

It does not decide determinism, Anchors, Echo, Insight, save model, loop
length, or Chapter 1's authored details. Those remain open or sandbox.
