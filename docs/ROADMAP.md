# Roadmap

Status: Accepted
Date: 2026-08-27
Owns: the order work is done in, and the gate each phase must pass before the
next one starts
Does not own: product direction and scope, which are
[`PROJECT_BRIEF.md`](PROJECT_BRIEF.md)'s; game rules, which are
[`design/`](design/)'s; what exists today, which is
[`CURRENT_STATUS.md`](CURRENT_STATUS.md)'s; task identity, which is
[`TASK_IDS.md`](TASK_IDS.md)'s

This document says **when**, never **what**. A phase's contents may change
without amending this record as long as its gate still stands. Changing a gate,
or the order of the phases, is an amendment.

There are no dates here. This repository runs on evidence, and a phase is over
when its gate is met.

## The problem this ordering solves

Three production lines are coming online at once: the narrative contract
pipeline, Fusion 360 for modelling, and delegated agent work in isolated
clones. Each of them produces faster than the game absorbs, and none of them
forces convergence on something playable.

So the ordering is by **playability**, not by what happens to be ready to
produce. Every phase ends in something a person can play or look at, never in a
pile of assets waiting for a place to go.

## What usually bites, in this domain specifically

Recorded here because it is the part a strong software process does not warn
you about, and it shapes every phase below.

- **Content is the cost, not code.** The instinct from software is that
  architecture is the hard part. In a game the code is a minority of the
  effort, and the phases below are sequenced around content throughput rather
  than around features.
- **The first packaged build always breaks.** Editor-only references, assets
  that exist in memory and not on disk, cook-only failures. This is why
  packaging is the very first gate and not a step near the end.
- **The vertical slice is the estimate.** Nothing about a later chapter can be
  estimated until one chapter has been finished all the way through, including
  the unglamorous fifth of it.
- **The boring last fifth is most of the remaining work.** Main menu, pause,
  settings, save slots, audio mix, quitting cleanly. None of it appears in any
  design document in this repository, and all of it has to exist. It is
  registered as a gap in Phase 1 rather than discovered in Phase 4.
- **Performance is measured, not reasoned about.**
  [`ADR-0010`](adr/ADR-0010_renderer-configuration.md) fixes 16.67 ms at
  1920×1080 on the named machine. That budget is only true on the day someone
  measures it.

## Phase 0 — prove what already exists

The chain is implemented and 25 automation tests pass. Almost none of it has
been played, and none of it has ever been packaged.

**Gate:**

- A packaged build exists and launches from a clean machine state.
- FL-18 has a recorded PIE run: a first-time commit of `courtyard_gate_open`
  ends a loop, and the next loop starts with the Anchor active. This is the
  game's central mechanic and it has never been played.
- FL-01, FL-02, FL-03 and FL-07 have recorded evidence against the current
  build.

**Why first:** no pipeline can be calibrated against an unverified base, and a
packaging failure found now costs a fraction of the same failure found after
three chapters of content have been built on top of it.

## Phase 1 — the vertical slice

Chapter 1, finished, end to end.

**Gate:**

- The route from the building exit through the courtyard, the gate, the ground
  beyond it and into the Transit Hub entrance is built and traversable.
- Echo rung 1 is in the game: the three authored figures in Apartment 4C, per
  [`design/echo.md`](design/echo.md). This is the narrative pipeline's first
  real output, deliberately at the smallest scale that proves it.
- The planted details in [`design/planted-details.md`](design/planted-details.md)
  are authored and registered as `authored`.
- Player animation is no longer a hard switch between two clips.
- The shell gap above is closed or explicitly deferred with a named owner:
  menu, pause, settings, save slots, quit.
- One recorded playthrough by somebody who is not the owner.

**Why here:** this is the first time all three production lines run on the same
content. The chapter is not the real deliverable — knowing what a chapter costs
is.

## Phase 2 — what Chapter 2 needs before Chapter 2

Everything here is cheap now and expensive later. That is the only reason it is
a phase.

**Gate:**

- [`ADR-0014`](adr/ADR-0014_source-organisation.md) is applied: the eight
  directories exist, and `ARewindProp` exists so Chapter 2's flavour objects
  never become classes.
- The Fusion 360 to Unreal path is proven on **one** Chapter 1 space, with the
  process written down: units, tessellation budget against
  [`ADR-0013`](adr/ADR-0013_third-party-asset-adoption.md)'s ceiling, UVs or
  triplanar handoff, shells rather than solids, collision, LODs. Proven where a
  mistake is cheap.
- [`design/echo.md`](design/echo.md),
  [`design/the-macro-loop.md`](design/the-macro-loop.md) and
  [`design/planted-details.md`](design/planted-details.md) are Accepted or
  amended, so tasks may cite them.
- Frame time is measured again against ADR-0010's budget.

## Phase 3 — Chapter 2 as production

With Phase 1's calibration and Phase 2's proven pipelines, Chapter 2 stops
being research. [`design/chapter-2-authored.md`](design/chapter-2-authored.md)
is accepted design authority and implementation is still blocked by
`PROJECT_BRIEF.md`'s phase non-goals; unblocking it is a decision this
document does not make.

**Gate:** Chapter 2 playable end to end, packaged, with its acceptance criteria
in [`acceptance/chapter-2-conduit-test.md`](acceptance/chapter-2-conduit-test.md)
recorded against a build.

## The gate after Chapter 2

**Decide whether Chapters 3 to 5 exist.** A decision, not an assumption.

[`design/the-macro-loop.md`](design/the-macro-loop.md) already requires that
the game be able to end earlier than planned and still have said what it means,
and that the strongest material not sit behind chapters that may never be
built. This is where that requirement is cashed.

## Amendments

Only changes to the order or to a gate are amendments. Phase contents move
freely.

- none
