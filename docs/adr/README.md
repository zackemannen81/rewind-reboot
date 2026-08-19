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

## Decisions that are open

Listed in `docs/PROJECT_BRIEF.md` under "Open decisions". Each will need a
record here before any task may depend on it. The two that block the first
product proof are world determinism and Anchor semantics; the one that is
time-sensitive regardless of scheduling is asset storage.
