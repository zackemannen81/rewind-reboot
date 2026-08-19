# ADR-0006 — C++ and Blueprint

Status: Accepted
Date: 2026-08-19
Deciders: MrWhite
Supersedes: nothing
Superseded by: nothing

## Context

The first Unreal module exists: `Rewind`, a blank C++ game module under
`Rewind/Source/Rewind/`. ADR-0001 left Blueprint versus C++ to the task
that first needed it. REW-0003 is that task.

FL-01 to FL-16 require a loop clock, a single apply order, knowledge,
one Anchor, and patrol/turnstile phases that are functions of elapsed
loop time. Those rules fail if they live in Blueprint graphs that tick
on frame delta, which is how the Unity patrol lost a reproducible
phase.

Blueprint is still the practical way to place actors in a map and to
iterate on presentation.

## Decision

**Gameplay systems that FL criteria depend on are C++.** That includes
the loop, world-state apply order, knowledge, Anchors, elapsed-loop-time
clocks, and the authored puzzle objects those systems own.

**Blueprint may place, decorate, and call into those systems.** It may
not be the source of truth for phase, reset, or persistence.

New gameplay types are added as C++ `UCLASS` types first. A Blueprint
subclass is allowed as a presentation or placement wrapper.

The project stays a C++ project associated with Unreal Engine 5.8.

## Alternatives considered

**Blueprint-only for the first proof, C++ later.** Rejected. The clock
and reset rules are the product. Putting them in graphs first repeats
the Unity failure, then asks C++ to rescue them.

**C++ only, no Blueprint.** Rejected as unnecessary. Placement and
meshes do not need to be code. Forbidding Blueprint would slow the
blockout without protecting determinism.

**Defer until the first actor is written.** Rejected. The module already
exists. Leaving the split open would let the next file choose by habit.

## Consequences

**Accepted costs:**

- Loop, knowledge, Anchors and clocks are written in C++ even when a
  Blueprint node would look shorter.
- Designers who only know Blueprint cannot change those rules without
  a C++ change.

**Accepted benefits:**

- Elapsed loop time can be the clock without fighting Tick graphs.
- Apply order has one place to exist.
- FL-03 and FL-08 can be tested against code, not against a graph that
  drifted.

**Consequences that bind future tasks:**

- No task may implement loop reset, Anchor apply, knowledge write, or a
  world clock only in Blueprint.
- Blueprint Tick is not a world clock.
- Changing this split is a new ADR.
