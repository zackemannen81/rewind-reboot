# ADR-0013 — Third-party asset adoption

Status: Accepted

Date: 2026-08-26

Owns: when a third-party asset may enter RE:WIND's tracked dependency closure,
and the cost, licence and performance tests it must pass first

## Context

[`ADR-0011`](ADR-0011_asset-source-and-provenance.md) decided that RE:WIND's art
is project-owned or generated, and that third-party packs stay outside the
tracked closure. Its point 4 says an asset may still be adopted later, but only
by amending that record rather than by a task deciding it is convenient. This is
that amendment, written as its own record because it adds a test a future task
has to run, not just a sentence.

Two things changed since 2026-08-25.

The first is the owner's position. The `/Game/Fab/` content is acquired from the
Unreal marketplace and the owner states it is approved for use in this project.
ADR-0011's reasoning against it was never that the art was wrong; it was that
342 MB of unverified provenance would be fixed into git history permanently. The
provenance is no longer unverified once the owner records it.

The second is that generation alone did not carry the room. Apartment 4C reached
a look the owner accepts only after marketplace props, a city backdrop, a neon
sign and apartment dressing were placed beside the generated ones. The map that
exists on disk today depends on them. Refusing the decision does not remove that
dependency; it only leaves it unrecorded, which is the specific failure mode this
repository exists to prevent.

There is a real cost that ADR-0011 was right about, and it does not go away.
Marketplace packs are authored for a demo frame, not for a 16.67 ms budget, and
they ship at whatever size the seller chose. `/Game/Deko_MatrixDemo/` is 2.5 GB
on disk while the saved map references eight meshes from it. Adopting the pack
and adopting what the map uses are very different commitments, and only one of
them is affordable.

## Decision

**A third-party asset may enter the tracked closure when it is free to acquire,
licensed for use in a commercial product, recorded with its provenance, and
inside the performance budget below. The pack it came from does not enter with
it.**

### 1. Cost and licence

- The asset is acquired at zero cost: permanently free, free for the month, or
  otherwise granted at no charge.
- Its licence permits use in a commercial product without per-title royalty or
  attribution obligations the project cannot meet. Epic's Fab Standard Licence
  and the Unreal Engine EULA both satisfy this.
- Nothing acquired under a personal, educational, evaluation or non-commercial
  licence is adopted, whatever it costs.

### 2. Provenance

Each adopted asset family gets one dated record under
`Rewind/ArtSource/ThirdParty/` naming the pack, the seller, the store listing,
the licence, the acquisition date and the exact `/Game/` packages adopted.
An adopted asset without such a record is a defect, not a detail. This is the
same rule ADR-0011 already applies to generated assets.

### 3. Performance budget

The ceiling is the owner's: **an adopted mesh may not exceed 300 000 triangles
at LOD0.** Above that it is decimated, Nanite-enabled or dropped, and a mesh in
the millions of triangles is not adopted in any form. The same applies to
texture size: an oversized set is resampled at import, and an asset that cannot
be brought down without visibly breaking is not adopted.

Under that ceiling, prefer as little as the frame allows. These are targets,
not gates, and they exist so the ceiling is rarely the thing being tested:

| Property | Dressing, seen at distance | Hero, near the camera |
| --- | --- | --- |
| Triangles at LOD0 | ≤ 15 000 | ≤ 50 000 |
| LODs or Nanite | expected above 15 000 triangles | expected above 15 000 triangles |
| Largest texture | 1024 | 2048 |
| Material slots | ≤ 3 | ≤ 6 |

Judged against ADR-0010's 16.67 ms at 1920×1080 on the named development
machine. Apartment 4C is the one space where "near the camera" is not settled:
the owner is writing later chapters that may return to it in first person, so
4C's hero props keep the hero budget even where the authored camera never gets
close.

An adopted asset carries no runtime behaviour of its own: no ticking Blueprint,
no bundled dynamic light, no particle or simulation component. Meshes,
materials and textures only. Behaviour in this project is C++ under
[`ADR-0006`](ADR-0006_cpp-and-blueprint.md), organised under
[`ADR-0014`](ADR-0014_source-organisation.md).

Style is the owner's call and is not a rule here. An asset inside every number
above may still be removed for looking wrong, and that decision needs no record
beyond the commit that removes it.

### 4. Only the referenced closure is tracked

Git tracks the dependency-closed set of packages the saved map actually
references, never the pack directory. The rest of the pack stays untracked and
local, exactly as ADR-0011 leaves it. An adopted subset is measured before it is
staged, and the measurement goes in the task record.

A wave that would add more than 250 MB of LFS objects stops and asks the owner
first. ADR-0005 put binaries in LFS and the quota is finite; this is the number
that makes "a large art import is a new decision" checkable instead of a
judgement call.

### 5. What this does not change

- Generated and project-owned assets remain the expected source. This record
  permits adoption; it does not make shopping the default.
- ADR-0011's rule that an asset without provenance may not be tracked is
  unchanged and now applies to two kinds of asset instead of one.
- `docs/PROJECT_BRIEF.md`'s non-goal on asset pipelines still stands.

## Alternatives considered

**Leave ADR-0011 as written and strip the references again.** This is REW-0022
repeated. It would return Apartment 4C to a state the owner has already judged
worse, and would discard a look pass the owner accepts. Rejected because the
decision being avoided is the same one either way.

**Adopt whole packs.** Simplest to state and to execute: track
`/Game/Fab/` and `/Game/Deko_MatrixDemo/` entire. Rejected on arithmetic. Those
two directories are 2.67 GB on disk for a map that references a few dozen
packages out of them, and LFS quota is the one resource here that cannot be
recovered by editing a file.

**Track packs but exclude them from the cook.** Keeps the editor library
complete for future dressing. Rejected because it spends the same quota for a
convenience the local untracked copy already provides, and because an unused
tracked asset is indistinguishable from a used one six months later.

**Set no performance budget and rely on profiling.** Honest, and how most
projects do it. Rejected because ADR-0010 already fixed a frame-time budget, and
a budget with no admission test is discovered as a regression instead of
prevented as a rule.

## Consequences

- The saved Apartment 4C map becomes reproducible from a fresh clone for the
  first time since REW-0014. That is the main thing this buys.
- Every future import gains an admission step: measure, check the licence, write
  the provenance record, stage the closure. That is slower than dragging a
  folder into `Content/`, deliberately.
- Assets already in the tree that were adopted before this record must be
  brought up to it retroactively or removed. The Fab electrical panel staged
  during REW-0038 is the first case.
- `/Game/Deko_MatrixDemo/` and `/Game/Fab/` keep existing as large untracked
  local libraries. Their referenced subsets become tracked; the libraries do
  not. A clone therefore reproduces the map without reproducing the shop.
- On acceptance, ADR-0011 point 2 and point 4 gain a dated amendment pointing
  here. ADR-0011 is not superseded: its rule that art is project-owned or
  generated by default still holds, and this record is the exception it
  anticipated.

## Acceptance

Accepted by the owner on 2026-08-27, with the 300 000 triangle ceiling as the
owner's own number. ADR-0011 points 2 and 4 carry the dated amendment this
record's consequences required.
