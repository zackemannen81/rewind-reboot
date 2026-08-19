# System Document

The durable shape of this repository and of the game's systems once they
exist. Behaviour that is implemented is described here; intentions belong in
`docs/PROJECT_BRIEF.md`. Game rules belong in `docs/design/` when a task
writes them.

An Unreal Engine 5.8 blank C++ project exists at `Rewind/Rewind.uproject`.
No loop, world-state apply, knowledge, Anchor or authored space is
implemented. The sections below describe the working model. They do not
describe a running game.

## The working model in one page

```text
private actor context
  → explicit repository state
  → one bounded active task
  → only the authority that task names
  → work and verification
  → durable status, decision and handoff
  → next actor resumes without the previous actor's memory
```

The benefit is not documentation volume. It is work continuity with bounded
context loading: the reader loads the active task and the authority it names,
not the project's whole history.

## Semantic roles

| Role | This repository | Owns |
| --- | --- | --- |
| Entry and guardrails | `AGENTS.md` | Reading order, safety, operating rules |
| Active work | `docs/CURRENT_TASK.md` | One task's frozen charter, progress, verification, handoff |
| Task state machine | `docs/TASK_WORKFLOW.md` | States, freeze, routing, identity |
| Approved direction | `docs/PROJECT_BRIEF.md` | Product, first proof, non-goals |
| Current reality | `docs/CURRENT_STATUS.md` | What exists now, and the gaps |
| Durable system model | `docs/SYSTEMDOC.md` | This document |
| Work history | `docs/JOURNAL.md` | Dated, signed evidence of work waves |
| Repository map | `docs/FILESTRUCTURE.md` | Where things live and why |
| Identity allocation | `docs/TASK_IDS.md` | Which identities are taken |
| Durable decisions | `docs/adr/` | Decisions, alternatives, consequences |
| Game rules | `docs/design/` | One ownership area per document |
| Proof of done | `docs/acceptance/` | What a build must demonstrate; empty until a task writes it |
| Completed work | `docs/finished/` | Immutable archived tasks |
| Blocked work | `docs/paused/` | Frozen parents awaiting a condition |
| Future work | `docs/backlog/` | Non-activated proposals |
| Excluded ideas | `docs/concepts_sandbox/` | Undecided concepts and imported legacy, never authority |
| Model provenance | `docs/baseline/` | Frozen docs-first source, never edited, never authority |

## Why the sandbox exists

An idea has three possible destinations: an authoritative document, where it
contaminates the truth surface; a chat log, where it leaves the repository; or
a marked non-authority area. Only the third keeps both properties the project
depends on — nothing valuable leaves, nothing undecided gains authority.

The previous project's GDD, tasks and code inventory live there for that
reason. They may be read. They may not be implemented.

The only route out:

```text
concept or legacy material
  → activated task or discovery
  → explicit decision
  → restatement in an owning document under docs/
  → implementation
```

## Addressing

A record's path is its identity. Status lives in the record's content and in
its collection index.

Journal entries are append-only and archived tasks are immutable, and they
cite other records by path. Renaming a cited record leaves two exits, and
both are forbidden: repairing append-only history, or leaving citations
broken. Restore the original path.

Validation of cited paths must distinguish tense. A document describing the
present must name paths that exist. The archive names files after they are
gone and the active charter names its deliverables before they exist.

## Identity allocation

Identities are claimed on the trunk before a charter freezes, in a strictly
ascending append-only register. The prefix in force is `REW`. `DFC-0001`
remains a taken address from the extraction this repository was bootstrapped
from.

The register records identity, never activity.

## Game systems

`Rewind/` has C++ types for the loop clock, loop-start apply order, session
knowledge, the one legal Anchor identifier, a USaveGame slot, and a
reachable `Rewind.CleanSave` command. World clocks are required to read
`URewindLoopSubsystem::GetElapsedLoopTime`, not engine time.

Apartment 4C is a C++ blockout (`ARewindFourCBlockout`) on
`/Game/Maps/FiveLoops`. Loop start is a `PlayerStart` tagged `LoopStart`
inside 4C. There is no radio, gate, patrol or turnstile. Apply order has no
LoopWorld puzzle participants yet besides teleporting the pawn to that
start.
FL-01 to FL-16 have not been run.

The previous Unity project, inventoried in
`docs/concepts_sandbox/legacy-rewind/code-inventory.md`, contained manager
classes, a Chapter 1 bootstrap and a player controller. It did not contain a
canonical world reset, a running loop in the committed scene, Echo replay, or
reproducible patrols. That inventory is evidence, not a system description of
this project.

When a task implements a system, this document is updated in the same change
to describe what the implementation does, not what is intended.

## This repository's own structure

```text
rewind-reboot/
├── AGENTS.md
├── README.md
├── Rewind/                Unreal Engine 5.8 C++ project; loop/session types exist, no authored space
├── docs/                  live working state
│   ├── design/            game rules, one ownership area per document
│   ├── acceptance/        proof criteria
│   ├── baseline/          frozen working-model provenance
│   └── concepts_sandbox/  legacy import and excluded ideas
```

Three levels coexist and must not be confused:

| Level | Meaning |
| --- | --- |
| `docs/` excluding sandbox and baseline | This project's live working state |
| `docs/baseline/` | Provenance of the docs-first working model. Frozen, unedited, not authority |
| `docs/concepts_sandbox/` | Non-authority. Legacy RE:WIND and other excluded ideas |
