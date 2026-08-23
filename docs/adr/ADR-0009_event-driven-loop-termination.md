# ADR-0009 — Event-driven loop termination

Status: Accepted
Date: 2026-08-24
Deciders: MrWhite
Supersedes: nothing
Superseded by: nothing

## Context

The accepted first proof ends a loop when a 240-second global timer expires or
when the player dies. REW-0007 made that rule mechanically meaningful by
building a route whose unanchored fuse branch exceeds the duration. The proof
therefore established that the timer works as authored. It did not establish
that a global countdown is the right reason for RE:WIND to happen.

That reason matters. A hidden or ever-present deadline turns environmental
reading into route optimization: after a rewind the player's first question is
how to move faster. The product brief instead says that the player grows
stronger by understanding the world. A reset caused by a legible causal
failure asks what prerequisite was missed, leaves room to inspect the authored
camera composition, and lets each boundary teach the next piece of the chain.

The owner supplied the Chapter 1 case that exposes the difference. If the
player crosses the building-to-courtyard gate threshold without first powering
the required ground-floor fuse box, the route ahead is invalid. Static should
creep in, the audiovisual world should collapse for one to three seconds, and
the player should wake in 4C. Reaching that threshold is the event. The
missing power is the cause. A clock reaching zero explains neither.

Removing the global end time creates a second obligation. The existing
three-part chain uses timeout to separate the loop that opens and Anchors the
courtyard gate from the loop that spends the released fuse on the lift. A new
rule must preserve that consequence without inventing an unrelated downstream
obstacle or weakening the Anchor.

ADR-0002 remains the determinism boundary. An event-driven loop is not allowed
to read loop count, previous-loop actions outside Anchors, frame time or
randomness merely because it no longer reads a duration.

## Decision

**RE:WIND is event-driven by default. A global elapsed duration is not a
default loop-end condition.**

A running loop may end for exactly these classes of reason:

1. **An authored causal contract fails at its named checkpoint.**
2. **The player dies.**
3. **The player successfully commits an Anchor.**

### Causal contracts

A causal contract consists of:

- a named checkpoint in the authored space;
- a deterministic predicate over authored baseline, active Anchors, elapsed
  loop time and this-loop player actions;
- the fact that crossing the checkpoint makes the unmet prerequisite a
  no-return choice for this loop.

The contract is evaluated at the checkpoint, not continuously and not whenever
any prerequisite is temporarily false. A fuse lying on a table is not failure.
Crossing the boundary that required the fuse is.

When a contract fails, rewind is latched. The player receives an authored
prelude lasting at least one and at most three seconds before loop-start apply
begins. The prelude must make the causal break perceptible; it may use static,
light instability, reversed or collapsing sound and visual artifacts. The
latched rewind cannot be canceled by stepping back across the checkpoint.
Exact final effects remain presentation work, but an immediate unexplained
teleport is non-compliant.

### Anchor commit

A successful explicit Anchor commit is itself a loop boundary. The commit is
written, the same one-to-three-second rewind prelude runs, and the next loop
starts with the newly active override applied. A rejected commit or a no-op
commit does not end the loop.

This keeps the Chapter 1 resource argument intact. The loop that spends the
fuse to produce and commit courtyard_gate_open ends at that meaningful choice.
The next loop receives the held gate and can spend the fuse on the lift.
Timeout is no longer what separates those uses.

### The role of time

Elapsed loop time still starts at zero and remains the source of truth for
deterministic world clocks. Radio phases, patrol windows, turnstile cycles,
train departures, overheating and other situations whose content is time may
use it.

A local deadline may fail a causal contract when missing that deadline makes
the situation impossible to complete and the deadline is authored and
legible. A whole-space duration is allowed only when the whole authored
situation is explicitly about that duration. It is never an invisible fallback
used to force an idle or exploratory player to rewind.

### Determinism

Contract evaluation and rewind scheduling obey ADR-0002. Two runs that share
baseline, Anchors, elapsed loop time and this-loop input reach the same
checkpoint result and begin the same prelude at the same t. Presentation may
smooth per frame; the decision to rewind may not.

## Alternatives considered

**Keep the global timer as the primary rule.** Rejected. It makes failure mean
that the player was too slow even when the actual puzzle error was causal, and
it taxes investigation under the authored camera.

**Use event gates but retain a global timeout as a safety net.** Rejected as the
default. A safety timer becomes the real rule for any player who explores or
idles, recreating the same pressure under a different label. A deliberately
timed whole-space scenario remains available when authored as content.

**Rewind immediately when a predicate is false.** Rejected. Prerequisites are
normally false while the player is still solving them, and an immediate
teleport reads as a load or punishment rather than a consequence.

**Teleport immediately at the checkpoint.** Rejected. The one-to-three-second
prelude is what lets the player connect the signal to the action that caused
it.

**Add an unrelated blocker after the courtyard to preserve three loops.**
Rejected. A new obstacle would hide the loss of the timer rather than express
the existing fuse-and-Anchor chain. A successful Anchor commit is already the
meaningful boundary between spending and releasing the fuse.

**Finish the new level first and decide later.** Rejected. Level distances,
one-way thresholds, camera coverage and puzzle placement depend on what ends a
loop. Building first would encode the current timer rule by accident.

## Consequences

**Accepted benefits:**

- Rewinds explain a missed causal prerequisite instead of an expired abstract
  budget.
- Exploration, fixed composition, radio listening and environmental reading do
  not consume a global failure resource.
- Every reset boundary can teach a named rule, so the player's knowledge front
  moves outward through the authored space.
- Local timing remains available where reading time is the actual mechanic.

**Accepted costs:**

- Every event gate needs a named checkpoint, predicate, no-return argument,
  prelude and repeatable test. A vague trigger volume is not enough.
- Spaces can no longer rely on timeout to recover a player who chooses not to
  progress. That is acceptable unless the situation is deliberately timed.
- The present C++ subsystem, route metrics, automation tests and REW-0007
  playtest implement and evidence the superseded timer-driven design surface.
  They remain true historical evidence and are not evidence for this decision.
- Chapter 1 must implement successful Anchor commit as an end reason and must
  distinguish it from failed or redundant commits.

**Consequences that bind future tasks:**

- loop-and-determinism.md owns the restated end-condition rule.
- chapter-1-authored.md owns the concrete ground-fuse checkpoint and the
  commit boundary.
- No implementation task may retain 240 seconds as an automatic Chapter 1 loop
  end after claiming compliance with this ADR.
- A timer may end a loop only when its owning authored situation states why
  that deadline is part of the causal problem.
- Immediate reset without the one-to-three-second perceptible prelude is a
  defect.

## What this does not decide

This record does not choose the final static texture, sound, post-process,
camera shake or animation used by the prelude. It does not set level dimensions
or camera transforms, and it does not make any untracked owner blockout a
repository artifact. It does not change Echo, Insight, save format or the
number of Anchors.
