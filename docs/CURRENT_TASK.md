# Current Task

Task ID: REW-0003
Parent Task: None
Status: In Progress
Owner: Grok
Created: 2026-08-19
Last updated: 2026-08-19
Charter frozen at: 2026-08-19

## Read First

- `AGENTS.md`
- `docs/TASK_WORKFLOW.md`
- `docs/PROJECT_BRIEF.md`
- `docs/CONTRIBUTING.md`
- `docs/CURRENT_STATUS.md`
- `docs/SYSTEMDOC.md`
- `docs/JOURNAL.md`
- `docs/FILESTRUCTURE.md`
- `docs/acceptance/five-loops-test.md`
- `docs/adr/ADR-0001_engine-unreal-engine-5.md`
- `docs/adr/ADR-0002_world-determinism.md`
- `docs/adr/ADR-0003_anchor-semantics.md`
- `docs/adr/ADR-0004_save-and-session.md`
- `docs/design/README.md`

## Task Summary

Implement the Five Loops Test in Unreal Engine 5.8 against the written
rules and FL-01 to FL-16. Do not invent game rules. Do not start the
Unreal project, and do not commit a `.uasset`, until asset storage is
decided.

## Task Charter

The charter is editable while status is `Draft` and immutable once status
is `Ready`. The `REW-0003` identity is on `main` as of `f505333`.

### Goal

Produce a playable Unreal 5.8 project in this repository that
demonstrates FL-01 through FL-16 from a clean save, with named evidence
for each criterion.

### Primary Deliverable

An Unreal Engine 5.8 project in this repository, plus a playtest record
that names the evidence for every FL criterion. The playtest record gets
a stable path before anything immutable cites it.

### In Scope

- Write ADR-0005, asset storage, before any `.uasset` is created in this
  repository. Update `.gitignore` and any LFS or external-store config
  in the same change as the ADR.
- Create the Unreal Engine 5.8 project in this repository after ADR-0005
  is Accepted. Pin 5.8.
- Decide Blueprint versus C++ for this project when the first module or
  Blueprint is created. Record it as ADR-0006; it constrains later tasks.
- Implement the loop, world-state apply order, knowledge, the one
  courtyard-gate Anchor, clean save, and the authored space FL-01 to
  FL-16 require.
- Clock world objects from elapsed loop time. Do not use engine global
  time or per-frame delta as the source of truth for patrol phase,
  turnstile or other world clocks.
- Demonstrate FL-01 through FL-16 from a clean save. Name the evidence
  for each. A passing compile is not evidence.
- Update `docs/CURRENT_STATUS.md`, `docs/SYSTEMDOC.md`,
  `docs/FILESTRUCTURE.md` and `docs/JOURNAL.md` in the same change as
  the project they describe.

### Out of Scope

- Inventing or rewriting game rules. If a criterion cannot be built
  because a rule is missing, pause and charter a child. Do not fill the
  hole in passing.
- Echo, Insight, Chapters 2 to 5, later temporal mechanics.
- Environment polish, final art, a narrative script, sound beyond what
  an FL criterion uses as a signal.
- Porting Unity C#, scenes or assets.
- The license, making the repository public, retiring `C:\code\reWiND`.
- Editing `docs/baseline/` or imported files under
  `docs/concepts_sandbox/legacy-rewind/`.

### Definition of Done

- ADR-0005 is Accepted and no `.uasset` was committed before it.
- An Unreal 5.8 `.uproject` exists in this repository and opens in the
  installed editor.
- FL-01 through FL-16 each have named evidence from a run that started
  at a clean save. Skipped criteria are not allowed.
- `docs/CURRENT_STATUS.md` no longer says there is no Unreal project and
  no longer says FL-01 to FL-16 have not been run.
- Echo, asset storage (once decided), and the license: asset storage is
  closed; Echo and the license remain open in `docs/PROJECT_BRIEF.md`.
- `docs/JOURNAL.md` has a signed entry and this task is archived.

### Minimum Verification Gates

- [ ] ADR-0005 Accepted before the first `.uasset` commit, checked by
      git history
- [ ] The `.uproject` opens in Unreal Engine 5.8 on this machine
- [ ] Each of FL-01 to FL-16 is demonstrated from a clean save, with
      named evidence
- [ ] Idle-loop match (FL-03) and patrol clock (FL-08) are checked at a
      stated `t`, not by eye from one play
- [ ] Manual link and fence review
- [ ] `git diff --check` clean

## References

- `docs/acceptance/five-loops-test.md`
- `docs/design/loop-and-determinism.md`
- `docs/design/anchors.md`
- `docs/design/world-state-model.md`
- `docs/design/save-and-session.md`
- `docs/design/chapter-1-authored.md`
- `docs/adr/ADR-0001_engine-unreal-engine-5.md` through ADR-0004
- `C:\Program Files\Epic Games\UE_5.8`

## Checklist

- [x] Merge the `REW-0003` identity claim to `main` before moving this
      charter to `Ready`
- [x] Write ADR-0005, asset storage, and the matching ignore/LFS config
- [x] Create the Unreal 5.8 project in this repository
- [x] Write ADR-0006, Blueprint versus C++, when the first gameplay type
      is added
- [x] Implement loop start/end, apply order, clean save
- [ ] Implement authored 4C, radio/`7312`, fuse, generator, gate, patrol,
      turnstile
- [ ] Implement explicit `courtyard_gate_open` commit
- [ ] Run FL-01 to FL-16 from a clean save and record evidence
- [ ] Update status, system document, file map
- [ ] Add a signed journal entry and archive this task

## Decisions and Notes

- This is the first implementation task. It is large because the proof
  is the milestone. If a discovery blocks it, pause and charter a child.
  Do not silently drop an FL criterion.
- Do not open Unreal to create the project until ADR-0005 is Accepted.
  ADR-0005 is Accepted. The next step is to create the Unreal 5.8 project
  in this repository. Opening the editor only to confirm 5.8 is installed
  is not that step.
- Unity code is not a starting point. Read it for authored intent already
  restated in `docs/design/`. Do not migrate it.
- "It compiles" is not "it works". "It works in the editor" is not "it
  works from a clean save".
- Enjoyment is recorded in the playtest write-up and is not a pass/fail
  gate, per `docs/acceptance/five-loops-test.md`.

## Charter Amendment Log

Only non-semantic corrections are allowed after `Ready`.

-none

## Verification

- [ ] Git history shows no `.uasset` before ADR-0005
- [ ] Editor open of the `.uproject` on UE 5.8
- [ ] FL-01 to FL-16 evidence list
- [ ] Manual link and fence review
- [ ] `git diff --check`
- [ ] Document skipped checks and reasons

## Documentation Updates

- [x] `docs/CURRENT_STATUS.md`
- [x] `docs/SYSTEMDOC.md`
- [ ] `docs/JOURNAL.md`
- [x] `docs/FILESTRUCTURE.md`
- [x] `docs/PROJECT_BRIEF.md` when ADR-0005 closes asset storage
- [x] `docs/adr/README.md`
- [x] ADR-0005
- [x] ADR-0006

## Handoff and Follow-ups

- Current state: In Progress. Loop clock, apply order, session save and
  `Rewind.CleanSave` exist in C++. No authored space. Compile and FL
  playtests have not been recorded.
- Next recommended step: compile RewindEditor, then implement authored
  4C, radio/`7312`, fuse, generator, gate, patrol and turnstile as C++
  participants of the apply order.
- Blockers: none.
- Child tasks: none.
- Resume condition: not applicable.
- Open questions: none inside this charter. Echo and the license remain
  project-level open decisions.

## Finalize When Complete

- Archive this file under `docs/finished/`.
- Restore this template or populate the next approved task.
- Add a signed `docs/JOURNAL.md` entry.
- If Goal or Definition of Done changed, supersede this task instead of
  rewriting it.
