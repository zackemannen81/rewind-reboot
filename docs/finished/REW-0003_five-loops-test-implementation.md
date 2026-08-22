# Current Task

Task ID: REW-0003
Parent Task: None
Status: Complete
Owner: Grok, re-assigned to Claude on 2026-08-23 when Grok could not continue.
Created: 2026-08-19
Last updated: 2026-08-23
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
- [x] Implement authored 4C, radio/`7312`, fuse, generator, gate, patrol,
      turnstile
- [x] Implement explicit `courtyard_gate_open` commit
- [x] Fix the four defects that made FL criteria undemonstrable rather
      than merely ugly: unauthored exposure against a 50000 lux sun, an
      unbounded outdoor run with 20 cm and 30 cm gaps at the two
      doorways, a patrol that teleported behind a barrier covering
      200 cm of a 760 cm corridor, and a gate and turnstile that did
      not span it either
- [x] Give the playtest record a stable path before anything cites it:
      `docs/playtests/`, index discoverability
- [x] Add the `LogRewind` run log, so evidence is read from stamped
      lines rather than transcribed by eye from screenshots
- [x] Run FL-01 to FL-16 from a clean save and record evidence
      (2026-08-23: all sixteen have named evidence across four runs, in
      `docs/playtests/five-loops-2026-08-22.md`. Three pass to a stated
      tick resolution. FL-14 passes on its wording only; every turnstile
      crossing landed within 0.7 s of the gate opening, so the number is
      not evidence of time pressure.)
- [x] Update status, system document, file map
- [x] Add a signed journal entry and archive this task

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
- 2026-08-23. Three criteria were not demonstrable before this wave, and
  nobody knew. The patrol barrier, the courtyard gate and the turnstile
  each spanned less than a third of the corridor, so a closed gate and a
  closed turnstile could both be walked around. Any earlier impression
  that the hub had been reached through the turnstile is void. This is
  the exact failure mode the "Evidence Discipline" rule exists to catch,
  and it was caught by measuring the geometry rather than by playing.
- 2026-08-23. Evidence is now read from `LogRewind` rather than
  transcribed from screenshots. Apply-order lines carry no `t`: they run
  at steps 2 and 3 of the apply order in `world-state-model.md`, before
  the clock is zeroed at step 5, so a stamp there would print the
  previous loop's final time.
- 2026-08-23. Transitions are logged on the tick that first observes
  them, so cross-loop agreement is shown to within about 0.25 s, not
  exactly. The offset is constant inside each loop, which is tick phase
  rather than drift. Exact equality at an arbitrary `t` follows from the
  code, since state is a pure function of elapsed loop time, but the log
  corroborates it rather than proving it.
- 2026-08-23. The editor serves MCP on localhost, which is how the idle
  pair was run. It exposes no console-command and no input tool, so it
  cannot invoke `Rewind.CleanSave` or move the player. Every remaining
  criterion needs a human at the keyboard. The editor also holds the
  build lock, so driving it and rebuilding it cannot overlap.

## Charter Amendment Log

Only non-semantic corrections are allowed after `Ready`.

-none

## Verification

Progress against the frozen gates. The gates themselves are not edited.

- [x] Git history shows no `.uasset` before ADR-0005. No `.uasset` has
      ever been committed. ADR-0005 was accepted in `51acd78` and the
      first `.umap` landed later, in `b669fc0`
- [x] Editor open of the `.uproject` on UE 5.8. Opened 2026-08-23, PIE
      started and ran two full loops
- [x] FL-01 to FL-16 evidence list, in
      `docs/playtests/five-loops-2026-08-22.md`. All sixteen carry named
      evidence from build C or D
- [x] Idle-loop match and patrol clock checked at stated times across
      two loops rather than by eye from one play, to the tick
      resolution stated in the record
- [x] Manual link and fence review, on the documents written this wave
- [x] `git diff --check` clean
- [x] Document skipped checks and reasons, in the record's "Not
      verified" section: frame-rate independence, exact equality at an
      arbitrary `t` for FL-03, FL-08 and FL-12, a packaged build, and
      whether any of it is enjoyable

## Documentation Updates

- [x] `docs/CURRENT_STATUS.md`
- [x] `docs/SYSTEMDOC.md`
- [x] `docs/JOURNAL.md`
- [x] `docs/FILESTRUCTURE.md`
- [x] `docs/PROJECT_BRIEF.md` when ADR-0005 closes asset storage
- [x] `docs/adr/README.md`
- [x] ADR-0005
- [x] ADR-0006

## Handoff and Follow-ups

- Current state: Complete, 2026-08-23. All sixteen FL criteria have
  named evidence in `docs/playtests/five-loops-2026-08-22.md`. Work is
  on branch `rew-0003/run-evidence`.
- Next recommended step: decide what follows the proof. The evidence
  shows the loop has no time pressure, which is a product question and
  not a defect. Three proposals in `docs/backlog/` bear on it: loop
  pressure and interaction cost, traversal as a knowledge axis, and art
  direction. None is authority, and the non-goal list in
  `docs/PROJECT_BRIEF.md` gates the third.
- Blockers: none.
- Child tasks: none.
- Resume condition: next working session on this machine with Unreal
  Engine 5.8. Close any open editor before building; Live Coding blocks
  compile. The editor's MCP endpoint is only reachable while the editor
  is open, so building and driving it cannot overlap.
- Verification gaps, carried into the record's "Not verified" section:
  frame-rate independence was tested at one rate only; FL-03, FL-08 and
  FL-12 hold to tick resolution rather than exactly; no packaged build
  exists; and FL-14 passes on its wording while its number measures the
  turnstile cycle rather than the player.
- Open questions: none inside this charter. Echo and the license remain
  project-level open decisions. Three findings discovered outside the
  charter are routed to `docs/backlog/`: art direction, loop pressure
  and interaction cost, and traversal as a knowledge axis. None is
  authority and none may be built from without an explicit decision.

## Finalize When Complete

- Archive this file under `docs/finished/`.
- Restore this template or populate the next approved task.
- Add a signed `docs/JOURNAL.md` entry.
- If Goal or Definition of Done changed, supersede this task instead of
  rewriting it.
