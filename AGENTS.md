# AGENTS.md

This repository is docs-first. Every task begins in `docs/CURRENT_TASK.md`.

RE:WIND is being rebuilt from a cold start. An earlier version of this game
exists and produced real design work, but none of it is authority here. The rule
that follows from that is the most important one in this file: **no document,
task, asset or line of code from the previous project may be treated as true
until it has been restated in an owning document under `docs/`.**

## Project Identity

- Project name: RE:WIND
- Repository: `rewind-reboot`
- Task identity prefix: `REW`
- Product: a third-person psychological sci-fi puzzle game in which the world
  resets in deterministic loops while the player's knowledge persists. The
  player grows stronger by understanding the world, not by traditional
  progression.
- Engine: Unreal Engine 5.8, decided in
  `docs/adr/ADR-0001_engine-unreal-engine-5.md`.
- Current phase: Chapter 1 authored-space implementation after the Five Loops
  proof. An Unreal Engine 5.8 C++ project and accepted game rules exist; use
  `docs/CURRENT_STATUS.md` for the exact implemented boundary.
- Status: private, all rights reserved. The license is an open decision in
  `docs/PROJECT_BRIEF.md`.

## Start Here

Read these in order before changing the repository:

1. `docs/CURRENT_TASK.md`
2. `docs/TASK_WORKFLOW.md`
3. `docs/PROJECT_BRIEF.md`
4. `docs/CONTRIBUTING.md`
5. `docs/CURRENT_STATUS.md`
6. `docs/SYSTEMDOC.md`
7. `docs/JOURNAL.md`
8. `docs/FILESTRUCTURE.md`

Read decision records under `docs/adr/` when the task touches a decided
boundary, and the design documents under `docs/design/` that the task names.

## Unreal Editor And Agent Playtests

- Before Unreal work, read `docs/EDITOR_AUTOMATION.md`. It owns the editor,
  MCP, plugin, toolset, build and agent-playtest procedure.
- Start Codex or another agent from the repository root so its committed MCP
  configuration is in scope. The Unreal MCP server exists only while this
  project's editor is open; editor autostart is not Windows autostart.
- Before claiming editor or PIE evidence, verify the endpoint and the required
  toolsets in the current session. Restart the agent session from the repo root
  if the configured Unreal tools are absent.
- Drive gameplay through the project PIE-input toolset, always release held
  keys before stopping PIE, and use named Unreal automation tests for
  repeatable acceptance. Desktop focus and Windows input emulation are not
  evidence.

## Documentation Ownership

- `docs/CURRENT_TASK.md`: the active task. One per branch.
- `docs/TASK_WORKFLOW.md`: task states, scope freeze, routing, identity.
- `docs/PROJECT_BRIEF.md`: approved product direction and fixed scope.
- `docs/CURRENT_STATUS.md`: what exists now and what does not.
- `docs/ROADMAP.md`: the order work is done in, and each phase's gate.
  Sequence only. It never states scope, direction or a game rule.
- `docs/SYSTEMDOC.md`: the durable shape of the game's systems and of this
  repository.
- `docs/JOURNAL.md`: dated, signed work waves. Append-only.
- `docs/FILESTRUCTURE.md`: repository map.
- `docs/DELEGATION.md`: how a delegated worker wave is set up and run here.
  Local wiring, launch commands, locks and monitoring.
- `docs/TASK_IDS.md`: identity claims. Allocation only, never status.
- `docs/adr/`: decisions, alternatives and consequences.
- `docs/design/`: game design authority, split by semantics. Each document owns
  a named area, and no two documents define the same rule.
- `docs/acceptance/`: what a build must demonstrate before it may be called
  done.
- `docs/finished/`: archived completed tasks, immutable.
- `docs/paused/`: frozen parent tasks awaiting a resume condition.
- `docs/backlog/`: non-activated proposals.
- `docs/concepts_sandbox/`: excluded concept work and imported legacy material.
  No task may cite it as authority.
- `docs/baseline/`: the frozen source of the working model itself. Provenance,
  never edited, never authority.

## Design Authority

Design documents are created when a task needs them, never in advance to make
the structure look complete. An empty `docs/design/` is an honest statement that
no game rule has been decided yet.

- One area of ownership per document, declared at the top.
- If two documents would define the same rule, one of them is wrong. Fix the
  ownership; do not duplicate the rule.
- A rule that is not in a design document is not a rule. It is a proposal.

## Legacy Material Is Not Authority

`docs/concepts_sandbox/legacy-rewind/` holds the previous project's design
documents, roadmaps, task files and journal, together with a verified inventory
of what its code actually did.

That material describes earlier intentions and experiments. It contains
contradictions, it does not describe this repository, and several of its tasks
are marked `DONE` for systems that were never implemented. It may be read
freely. It may not be implemented.

The only route out of the sandbox is:

```text
concept or legacy material
  → activated task or discovery
  → explicit decision
  → restatement in an owning document under docs/
  → implementation
```

Nothing becomes true by sitting in the sandbox, and nothing becomes true by
being linked from an authoritative document.

## Task Workflow

### Start

- Claim the identity in `docs/TASK_IDS.md` and merge that claim to `main`
  before moving the charter to `Ready`. The next free identity is one above the
  highest of the register and `docs/finished/`. A local count cannot see
  somebody else's branch.
- Fill `docs/CURRENT_TASK.md` from `docs/template_CURRENT_TASK.md` with goal,
  deliverable, scope, out-of-scope, definition of done and verification gates.
- Freeze the charter when status changes from `Draft` to `Ready`.

### During work

- Work from the checklist and keep it truthful.
- Do not expand or redefine a frozen goal, deliverable, scope or definition of
  done.
- Route every discovery: inside the charter it becomes a checklist item; if it
  blocks, pause the parent and activate a bounded child; if it is in project
  scope but not needed now, it goes to the backlog; if it is outside project
  scope, it goes to the concepts sandbox.
- Update durable documentation in the same change as what it describes.

### Pause or handoff

- Record blockers, next steps, verification gaps and a resume condition.
- Add a dated, signed entry to `docs/JOURNAL.md`.
- The next actor must be able to resume from the repository alone.

### Finish

- Verify in proportion to risk and state what was not verified.
- Archive the task under `docs/finished/` as `REW-NNNN_task-slug.md`.
- Restore `docs/CURRENT_TASK.md` from the template, or fill it with the next
  approved task.

## One Active Task, Per Branch

- At most one task is active per branch, held in that branch's
  `docs/CURRENT_TASK.md`. Version control already provides exactly one copy per
  branch, so this states the rule where it is already enforced.
- The trunk never states how many tasks are active anywhere.
  `docs/TASK_IDS.md` records that an identity is taken, which says nothing about
  activity.
- A branch merges when its task is complete, so `main` normally carries the
  restored template. A merged in-progress charter is an explicit exception.

## Addressing and Discoverability

- A record's path is its identity. Status is declared in the record's content
  and in its collection index, never in a filename or a location.
- A file cited by append-only or archived documentation keeps its path, whatever
  its type. Renaming it is not repairable: fixing the citations would mean
  editing records that must not be edited.
- Do not cite disposable material from an immutable record. Give it a stable
  path first, or describe it instead of linking it.
- Every collection declares one discoverability mode in its `README.md`:
  `index`, meaning every member is listed there, or a naming convention that
  makes every member addressable without a list.

## Evidence Discipline

The previous project failed its own status surface. Core systems were recorded
as complete while world reset, canonical anchor application and Echo replay did
not exist. That specific failure is what this repository is built to prevent.

- Do not state that something is implemented, tested or working without naming
  the evidence.
- A checklist item is not evidence. A passing gate, a recorded playtest or a
  reviewed diff is.
- Distinguish observed facts, supported inferences and open hypotheses.
- "It compiles" is not "it works". "It works in the editor" is not "it works
  from a clean save".
- When a claim and the build disagree, the build is right and the document is
  corrected in the same change.

## Verification Baseline

Verify in proportion to risk, and state what was not verified.

- Documentation tasks: links resolve, fenced blocks close, ownership is not
  duplicated, `git diff --check` is clean.
- Design tasks: every rule has exactly one owning document, and every rule the
  acceptance criteria depend on is stated in testable terms.
- Implementation tasks: the gates named in the charter. Unreal build, editor,
  PIE-input and automation-test procedures are in
  `docs/EDITOR_AUTOMATION.md`.

Every task states which checks it ran and which it could not. A successful
compile is never substituted for the PIE or automation evidence its charter
requires.

## Safety and External Effects

- Never commit credentials, personal data, or third-party licensed assets whose
  terms have not been checked.
- Large binary assets are a deliberate decision, not the side effect of a task.
  Unreal projects grow quickly, and `docs/adr/` is where the storage strategy is
  decided before the first `.uasset` lands.
- The repository stays private until the license question is decided.
- Publication, releases, store pages and announcements require explicit
  approval.
- Nothing under `docs/baseline/` or `docs/concepts_sandbox/legacy-rewind/` is
  ever edited. Corrections belong in this project's own documents.
