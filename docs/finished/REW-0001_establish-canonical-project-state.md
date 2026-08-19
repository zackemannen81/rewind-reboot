# Current Task

Task ID: REW-0001
Parent Task: None
Status: Complete
Owner: Grok
Created: 2026-08-19
Last updated: 2026-08-19
Charter frozen at: not frozen; identity not yet merged to `main`

## Read First

- `AGENTS.md`
- `docs/TASK_WORKFLOW.md`
- `docs/PROJECT_BRIEF.md`
- `docs/CONTRIBUTING.md`
- `docs/CURRENT_STATUS.md`
- `docs/SYSTEMDOC.md`
- `docs/JOURNAL.md`
- `docs/FILESTRUCTURE.md`
- `docs/adr/ADR-0001_engine-unreal-engine-5.md`
- `docs/concepts_sandbox/legacy-rewind/README.md`
- `docs/concepts_sandbox/legacy-rewind/reboot-plan-source.md`

## Task Summary

Establish the canonical project state of this repository as the RE:WIND reboot.
A newcomer must be able to tell what the product is, what actually exists, what
does not, and which hypothesis is to be proved first, without private chat
history and without treating the previous project as authority.

DFC-0001, the leftover protocol-specification charter from the source
repository, is cancelled in the same change. Its identity remains taken.

## Task Charter

The charter is editable while status is `Draft` and immutable once status is
`Ready`. This charter is being executed before the identity claim is merged,
because the work is already in the working tree and the leftover DFC-0001
charter is the wrong task for this repository.

### Goal

Make it possible for a new human or agent to understand what RE:WIND is, what
actually exists today, and which smallest playable hypothesis must be proved
first.

### Primary Deliverable

A consistent docs-first instance for RE:WIND: product direction, current
reality, engine decision, verified legacy inventory, design-conflict register,
repository map, and a proposal for the next bounded task.

### In Scope

- Cancel DFC-0001 and claim REW-0001.
- Finish aligning live documents with `AGENTS.md` and `docs/PROJECT_BRIEF.md`.
- Inventory the Unity implementation at `C:\code\reWiND` against the source and
  write `docs/concepts_sandbox/legacy-rewind/code-inventory.md`.
- Register contradictions in the imported design material as
  `docs/concepts_sandbox/legacy-rewind/design-conflicts.md`.
- Write a `.gitignore` suitable for an Unreal Engine 5 project that does not
  yet exist, or stop claiming that one exists.
- Propose the next bounded task without activating it.
- Update `docs/CURRENT_STATUS.md`, `docs/SYSTEMDOC.md`, `docs/FILESTRUCTURE.md`
  and `docs/JOURNAL.md` in the same change.

### Out of Scope

- Any gameplay implementation.
- Creating an Unreal project.
- Writing design authority in `docs/design/`.
- Writing acceptance criteria in `docs/acceptance/`.
- Deciding world determinism, Anchor semantics, Echo semantics, save model,
  asset storage, or the license.
- Editing anything under `docs/baseline/` or imported files under
  `docs/concepts_sandbox/legacy-rewind/` other than this task's own inventory
  and conflict register.
- Implementing or restating any legacy mechanic as a rule.

### Definition of Done

- A stranger reading `AGENTS.md` and the start-here list can name the product,
  the engine, what exists, what does not, and the first product proof.
- Every live document that still described the protocol project has been
  restated for RE:WIND.
- `code-inventory.md` names the Unity files that were read and distinguishes
  observed behaviour from earlier DONE claims.
- `design-conflicts.md` states each contradiction as a question a decision
  must answer.
- `docs/CURRENT_STATUS.md` no longer claims files that are not in the tree.
- DFC-0001 is archived as Cancelled.
- `docs/JOURNAL.md` has a signed entry.

### Minimum Verification Gates

- [x] Every path cited from live documents under `docs/` (excluding
      `docs/baseline/` and imported legacy files) resolves
- [x] `code-inventory.md` findings can be pointed to in named Unity source
      files
- [x] Manual link and fence review
- [x] `git diff --check` clean
- [x] Status document agrees with the working tree

## References

- `docs/concepts_sandbox/legacy-rewind/reboot-plan-source.md`
- `C:\code\reWiND` at commit `9f959140e25163bddef390979f23f352e8f0c2f6`
- `docs/adr/ADR-0001_engine-unreal-engine-5.md`

## Checklist

- [x] Read `AGENTS.md` and the start-here list
- [x] Observe that DFC-0001 is the wrong charter for this repository
- [x] Cancel DFC-0001 and claim REW-0001 in `docs/TASK_IDS.md`
- [x] Inventory Unity core systems against source
- [x] Write `code-inventory.md`
- [x] Write `design-conflicts.md`
- [x] Align remaining live documents and collection indexes
- [x] Write or stop claiming `.gitignore`
- [x] Propose the next bounded task in `docs/backlog/`
- [x] Correct `docs/CURRENT_STATUS.md` against the tree
- [x] Add a signed journal entry and archive this task when the gates pass

## Decisions and Notes

- Transcription of the working model is already in `docs/baseline/`. This task
  does not extract a protocol specification; that was DFC-0001 and is cancelled.
- The engine decision is already accepted in ADR-0001. This task does not
  reopen it.
- `code-inventory.md` and `design-conflicts.md` are sandbox material. They are
  evidence, not authority.
- The identity claim for REW-0001 has not been merged to `main`. The charter
  is therefore not frozen. Completing the work in this working tree is an
  explicit exception, recorded here so it is not mistaken for a merged Ready
  charter.
- Stronger inventory finding than the recovery notes: the committed Unity
  scene does not instantiate TimeManager, KnowledgeManager or AnchorManager.

## Charter Amendment Log

Only non-semantic corrections are allowed after `Ready`.

-none

## Verification

- [x] Ledger-style path check of live docs
- [x] Inventory findings matched to named source files
- [x] Manual link and fence review
- [x] `git diff --check`
- [x] Document skipped checks and reasons

Ran: relative-link resolution and even-fence check on twenty-four live
documents, including the new inventory and conflict register;
`git diff --check` (clean); directory listing of `C:\code\reWiND\Assets\Scripts`;
full reads of the manager, event, echo, patrol, orchestrator, gate, fuse,
generator, radio, turnstile, bootstrap and scene-entry files; GUID search of
`Chapter1_Blockout.unity`; presence check of
`C:\Program Files\Epic Games\UE_5.8`.

Did not run: Unity Editor, an Unreal editor session, a packaged build, a
link checker over `docs/baseline/` (excluded on purpose), or any automated
test. This repository has no tooling.

## Documentation Updates

- [x] `docs/CURRENT_STATUS.md`
- [x] `docs/SYSTEMDOC.md`
- [x] `docs/JOURNAL.md`
- [x] `docs/FILESTRUCTURE.md`
- [x] `docs/TASK_IDS.md`
- [x] Collection indexes that still name DFC or missing files

## Handoff and Follow-ups

- Current state: Complete. Canonical project state is in the working tree,
  not yet committed.
- Next recommended step: commit this change, then activate
  `docs/backlog/five-loops-test.md` only after the open decisions it names
  are either decided or explicitly scoped out of that charter.
- Blockers: none for this task. Asset storage still blocks the first
  `.uasset`. Determinism and Anchor semantics still block treating those as
  rules.
- Child tasks: none.
- Resume condition: not applicable.
- Open questions: none inside this charter. Product open decisions remain in
  `docs/PROJECT_BRIEF.md`.

## Finalize When Complete

- Archive this file under `docs/finished/`.
- Restore this template or populate the next approved task.
- Add a signed `docs/JOURNAL.md` entry.
- If Goal or Definition of Done changed, supersede this task instead of
  rewriting it.
