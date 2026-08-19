# Project Brief

Status: Approved direction for the reboot phase. Revised only by an explicit
decision, never by a task in passing.

This document owns the product direction at a high level. It does not own the
detailed behaviour of Echo, Anchors, Insight or any chapter beyond the first
product proof. Those become authority in `docs/design/` when a task needs them,
and not before.

## The core fantasy

RE:WIND is a third-person psychological sci-fi puzzle game. The world resets in
deterministic loops. The player's knowledge does not.

> The world is deterministic. The player accumulates knowledge.

This is the product. Everything else is negotiable. What "deterministic"
means is decided in
[`ADR-0002`](adr/ADR-0002_world-determinism.md) and owned as a rule by
[`docs/design/loop-and-determinism.md`](design/loop-and-determinism.md).

The player becomes more powerful by understanding the world, not by collecting
upgrades, levelling up or unlocking abilities. A player on their fifth loop is
faster than a player on their first because they know where the fuse is, what
the radio says, and when the patrol looks away. The game does not hand them
anything for the repetition; it just stops being in their way.

## Target player

Players who enjoy being the one who figures it out. The reference points are
puzzle games that respect deduction over reflex, and loop-structured games where
mastery is knowledge rather than execution.

The game must be legible to a player who has never seen it before and must
reward a player who has seen it five times. If loop two is not measurably
different from loop one for a player who paid attention, the design has failed.

## Product goals

1. Prove that the loop is fun before building content for it.
2. Keep the world's behaviour reproducible, because knowledge is worthless if
   the world is not the same next time.
3. Make the player's growing competence visible to the player.
4. Keep the truth about what is built in the repository, not in someone's head.

## The first product proof

The first product proof is not a chapter, a level or a build. It is a testable
answer to one question:

> Is it enjoyable to experience the same deterministic world several times and
> become objectively better at it because you know more?

That proof is chartered as the Five Loops Test. It is the whole of the current
milestone, and it is deliberately smaller than Chapter 1. When it passes, the
project has evidence for its central hypothesis. Until it passes, the project
has a hypothesis.

`docs/acceptance/` owns what the proof must demonstrate, once a task writes it.

## Engine

Unreal Engine 5.8. Decided in
[`ADR-0001`](adr/ADR-0001_engine-unreal-engine-5.md).

The previous project was built in Unity. Its C# implementation is design
reference, not a starting point, and is inventoried in
[`code-inventory.md`](concepts_sandbox/legacy-rewind/code-inventory.md).

## Non-goals for this phase

None of the following may be started, planned into a schedule, or used to
justify scope in the current phase:

- Chapters 2 to 5, and any temporal mechanic beyond a plain deterministic loop:
  backwards time, parallel loops, timeline editing, paradoxes, local time
  bubbles.
- A general-purpose time engine. The loop needs to reset one small authored
  space correctly, not model time.
- Environment polish, final art, a brutalist environment kit, procedural asset
  generation, or an asset pipeline.
- Sound design beyond what an acceptance criterion depends on.
- A narrative script. Chapter 1's authored content becomes design authority only
  where the product proof needs it.
- Reimplementing a legacy idea because it is documented. Documentation is not a
  mandate.
- Multiplayer, console platforms, storefront work, marketing.

Non-goals are not a backlog. Something moves off this list by an explicit
decision that changes this document, not by a task deciding it is ready.

## What the reboot rejects

The previous project accumulated a 123 KB design document, five chapter task
files, a five-phase seventeen-month plan, and 140 MB of assets, while its core
loop could not reset the world. Volume was mistaken for progress, and status
documents recorded intentions as completions.

This phase is ordered the other way round: prove the loop, then earn the right
to build content for it.

## Evidence position

| Class | Permitted statement |
| --- | --- |
| Observed | The previous project produced authored Chapter 1 content, a player controller, patrol behaviour and interaction scripts in Unity |
| Observed | Its loop timer, knowledge persistence and anchor storage existed; world reset, canonical anchor application and Echo replay did not |
| Supported inference | The authored Chapter 1 golden path is detailed enough to support a five-loop test without new design invention |
| Not yet proven | That the loop is fun; that knowledge-based progression sustains a full game; that the deterministic model survives contact with an authored space |

`docs/CURRENT_STATUS.md` is the authority on what exists. If this document and
the status document disagree about reality, the status document is right.

## Open decisions

These are not decided, and no task may assume them:

1. **Echo semantics.** Whether Echo replays recorded actions, is a visual
   silhouette, or is narrative only.
2. **Asset storage strategy.** Whether binary assets live in git, in Git LFS, or
   outside the repository. This must be decided before the first `.uasset`.
3. **The license, and whether this repository ever becomes public.** The
   repository currently carries an all-rights-reserved notice, chosen as the
   safe default rather than as a decision.
4. **Whether the Unity project is retired.** `C:\code\reWiND` still exists and is
   untouched. Nothing here depends on it.
