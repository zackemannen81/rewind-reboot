# ADR-0011 — Asset source and provenance

Status: Accepted

Date: 2026-08-25

Owns: where RE:WIND's art assets come from, and what may enter the tracked
dependency closure

## Context

The tracked default map `/Game/Maps/FiveLoops_Stairwell_Blockout` references
packages that git does not track:

- `/Game/Fab/` — an armchair and ottoman, a coffee table, a desk lamp, a radio,
  a tileable floor and four Megascans surfaces, measuring 342 MB
- `/Game/Art/Texture/` — seven owner-authored `4c_*` materials

A fresh clone resolves none of them. The repository therefore claims a default
map it cannot reproduce, which was recorded in
[`untracked-fab-dependency.md`](../backlog/untracked-fab-dependency.md) and
proved concrete when three agent clones opened that map during REW-0016.

[`ADR-0005`](ADR-0005_asset-storage.md) already states that a large art import
is a new decision. This is that decision.

The product also wants specific things from its art: "1:1 style true textures",
prop meshes generated rather than shopped for, and a player who reads as the
silhouette of a man in a coat. Those are authored intentions, not a shopping
list.

## Decision

**RE:WIND's art assets are project-owned or generated. Third-party asset packs
do not enter the tracked dependency closure.**

Concretely:

1. The tracked repository contains only assets this project authored,
   generated, or imported under a licence recorded at import time with the file
   itself.
2. The existing `/Game/Fab/` and `/Game/Art/Texture/` content stays untracked
   and local. The authored map is unbound from it, so a fresh clone opens
   cleanly. Removing those references is chartered as REW-0022.
3. Generated meshes and textures are permitted and are the expected source for
   props and surfaces. The tool used and the date are recorded with the asset.
4. A third-party asset may still be adopted later, but only by amending this
   record, not by a task deciding it is convenient.

## Alternatives considered

**Commit the existing Fab and Texture imports under Git LFS.** Fastest: the
dressing the owner already built would become reproducible immediately.
Rejected because it would fix 342 MB of unverified provenance into git history
permanently, tie the look to packs chosen before the direction was decided, and
spend LFS quota on material the look pass may discard anyway.

**Hybrid: track the Megascans surfaces, generate the props.** Attractive,
because the surfaces are close to the "1:1 true texture" goal. Rejected for now
because it still requires a licence review to be done properly, and because
splitting the rule makes it harder to state than to follow. It is the most
likely candidate for a future amendment.

**Strip the references and leave the question open.** This is what REW-0022
does mechanically, but as a decision it defers rather than decides, and the
project has been carrying the ambiguity since REW-0014.

## Consequences

- Apartment 4C returns to undressed blockout until generated or authored props
  replace what is being unbound. That is a visible regression in the owner's
  editor and is accepted deliberately.
- The 358 MB of LFS objects pushed accidentally during REW-0016, and since
  orphaned by deleting that branch, remain unreferenced on the remote until
  purged. They are not part of any branch.
- Every future asset carries a provenance line. An asset without one is a
  defect, not a detail.
- Generation becomes a dependency of the look. If a generator is unavailable,
  the space stays blockout rather than borrowing a pack.
- `docs/PROJECT_BRIEF.md`'s non-goal on asset pipelines still stands. This
  record decides where assets come from, not that a pipeline is now in scope.

## Status of related records

[`untracked-fab-dependency.md`](../backlog/untracked-fab-dependency.md) is
resolved by this record. Its option 2 was chosen.
