# Decision Records

Discoverability: index. Every member of this directory is listed below.
Member state: required. Every member declares a `Status:` line under its title.

A decision record is written when a choice constrains future work: the engine,
the world-state model, what an Anchor means, what Echo does, how assets are
stored, or anything that would be expensive to reverse.

Conflicts in the legacy design are not resolved by picking the version that
sounds most reasonable. They become decisions, recorded here, with the
alternatives and the consequences.

A record states the context, the decision, the alternatives considered and the
consequences. Superseded records stay, marked, with a link to what replaced
them. They are never deleted, because the reasoning is the point.

Naming: `ADR-NNNN_short-slug.md`. Numbers are permanent and are cited by tasks,
journal entries and design documents, so a record is never renumbered, renamed
or moved.

## Records

| Record | Status | Owns |
| --- | --- | --- |
| [`ADR-0001_engine-unreal-engine-5.md`](ADR-0001_engine-unreal-engine-5.md) | Accepted | The engine, and the status of the legacy Unity implementation |
| [`ADR-0002_world-determinism.md`](ADR-0002_world-determinism.md) | Accepted | What is identical across loops, and what may change |
| [`ADR-0003_anchor-semantics.md`](ADR-0003_anchor-semantics.md) | Accepted | Anchor meaning, commit, cost, and the first-proof override |
| [`ADR-0004_save-and-session.md`](ADR-0004_save-and-session.md) | Accepted | What persists, and what a clean save is |
| [`ADR-0005_asset-storage.md`](ADR-0005_asset-storage.md) | Accepted | Unreal binaries in git via Git LFS |
| [`ADR-0006_cpp-and-blueprint.md`](ADR-0006_cpp-and-blueprint.md) | Accepted | C++ owns FL systems; Blueprint may place and present |
| [`ADR-0007_camera-and-perspective.md`](ADR-0007_camera-and-perspective.md) | Accepted | The camera is authored, not player-controlled |
| [`ADR-0008_what-an-anchor-is-worth.md`](ADR-0008_what-an-anchor-is-worth.md) | Accepted | Why an Anchor exists, the test a candidate must pass, and why they stay scarce |
| [`ADR-0009_event-driven-loop-termination.md`](ADR-0009_event-driven-loop-termination.md) | Accepted | Why loops end, how causal checkpoints work, and when time may end a loop |
| [`ADR-0010_renderer-configuration.md`](ADR-0010_renderer-configuration.md) | Accepted | Which renderer features are on, the frame-time budget and the hardware floor |
| [`ADR-0011_asset-source-and-provenance.md`](ADR-0011_asset-source-and-provenance.md) | Accepted | Where art assets come from, and what may enter the tracked dependency closure |
| [`ADR-0012_echo-semantics.md`](ADR-0012_echo-semantics.md) | Accepted | What Echo is, and what it is not |
| [`ADR-0013_third-party-asset-adoption.md`](ADR-0013_third-party-asset-adoption.md) | Accepted | When a third-party asset may enter the tracked closure, and the cost, licence and performance tests it must pass |
| [`ADR-0014_source-organisation.md`](ADR-0014_source-organisation.md) | Accepted | How C++ under `Rewind/Source/` is laid out and named, and the test a new class must pass |

## Decisions that are open

Listed in `docs/PROJECT_BRIEF.md` under "Open decisions". Asset storage no
longer blocks the first `.uasset`. Echo is decided by ADR-0012 and asset
provenance by ADR-0011. The license and the Unity project's retirement remain
open.
