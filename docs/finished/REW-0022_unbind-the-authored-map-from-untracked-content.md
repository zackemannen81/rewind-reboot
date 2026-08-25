# Current Task

Task ID: REW-0022
Parent Task: None
Status: Complete
Owner: grok-unbind
Created: 2026-08-25
Last updated: 2026-08-25
Charter frozen at: 2026-08-25

## Read First

- `AGENTS.md`
- `docs/TASK_WORKFLOW.md`
- `docs/PROJECT_BRIEF.md`
- `docs/CONTRIBUTING.md`
- `docs/CURRENT_STATUS.md`
- `docs/SYSTEMDOC.md`
- `docs/JOURNAL.md`
- `docs/FILESTRUCTURE.md`
- `docs/adr/ADR-0011_asset-source-and-provenance.md`
- `docs/adr/ADR-0005_asset-storage.md`
- `docs/backlog/untracked-fab-dependency.md`
- `docs/design/stairwell-visual-direction.md`
- `docs/EDITOR_AUTOMATION.md`

## Task Summary

`Rewind/Content/Maps/FiveLoops_Stairwell_Blockout.umap` is tracked, is the
editor and game default, and has been recorded as referencing untracked
`/Game/Fab/` and `/Game/Art/Texture/` packages. A fresh clone resolves none of
them. ADR-0011 decided that art assets are project-owned or generated and that
third-party packs stay outside the tracked dependency closure. This task makes
the tracked map true to that decision.

## Task Charter

The charter is editable while status is `Draft` and immutable once status is
`Ready`.

### Goal

A fresh clone opens the authored default map with no missing references.

### Primary Deliverable

The tracked default map, unbound from every untracked package, with the
actors that depended on them either removed or rebuilt from project-owned
assets.

### In Scope

- Identify the map's true untracked dependencies using the editor's own
  reference tooling.
- Remove or replace every reference to /Game/Fab/ and /Game/Art/Texture/.
  Where an actor exists only to hold a third-party mesh, remove the actor.
  Where a surface used an untracked material, substitute a project-owned
  material from Rewind/Content/Art/Materials/, which is already tracked.
- Preserve every gameplay actor, camera region, light and piece of
  blockout geometry that does not depend on untracked content. The
  playable route must be unchanged.
- Verify from a clean state that nothing is missing: the check that
  matters is opening the map with the untracked directories renamed or
  absent, not opening it on the owner's machine where they resolve.
- Correct docs/CURRENT_STATUS.md and docs/FILESTRUCTURE.md where they
  describe the dependency situation.

### Out of Scope

- Adding, committing or deleting anything under Rewind/Content/Fab/ or
  Rewind/Content/Art/Texture/. They are the owner's local files. Leave
  them on disk, untracked and unmodified.
- Generating replacement props or textures. ADR-0011 permits generated
  assets; producing them is a separate task and is not this one.
- Any C++ under Rewind/Source/.
- Renderer configuration. ADR-0010 is settled.
- The courtyard, which is REW-0020.
- docs/TASK_IDS.md. The operator owns it and already claimed REW-0022.

### Definition of Done

- The map's dependency set contains no /Game/Fab/ or /Game/Art/Texture/
  package.
- The map opens with no missing-reference warnings when the untracked
  directories are not present.
- The playable route from 4C through the common hall, stairs and lift is
  unchanged: the same actors, the same camera regions, the same
  collision.
- Apartment 4C may be undressed. That is expected and accepted by
  ADR-0011; do not substitute third-party content to avoid it.
- RewindEditor Win64 Development builds with the editor closed, and all
  discoverable Rewind.* automation tests pass together.

### Minimum Verification Gates

- [x] Editor-closed build succeeds.
- [x] All discoverable Rewind.* tests pass together, run headless.
- [x] A stated clean-open check proving no missing references.
- [x] The journal entry names what was verified and what was not.

## References

- `docs/adr/ADR-0011_asset-source-and-provenance.md`
- `docs/adr/ADR-0005_asset-storage.md`
- `docs/backlog/untracked-fab-dependency.md`
- `docs/design/stairwell-visual-direction.md`
- `Rewind/Content/Maps/FiveLoops_Stairwell_Blockout.umap`
- `Rewind/Content/Art/Materials/`
- `docs/EDITOR_AUTOMATION.md`

## Checklist

- [x] Transcribe the frozen charter into `docs/CURRENT_TASK.md`.
- [x] Confirm this clone's `Rewind/Content/Fab/` and
      `Rewind/Content/Art/Texture/` state (present, absent, or renamed).
      Both directories are absent. They were not created, copied, renamed
      or deleted.
- [x] Identify the map's true untracked dependencies with editor reference
      tooling, not a strings scan alone. Evidence: Unreal Asset Registry
      `get_dependencies` on `/Game/Maps/FiveLoops_Stairwell_Blockout` after
      `load_map`, plus per-actor mesh/material inspect. Live package deps
      contained no `/Game/Fab/` or `/Game/Art/Texture/`. Six StaticMeshActors
      still held missing meshes from those packages.
- [x] Remove actors that exist only to hold third-party meshes. Deleted:
      `scene`, `fbx`, `ptish_radio`, `desk_lamp`, `electrical_panel`, and
      unnamed `StaticMeshActor` at `(-250, 1440, 1270)`.
- [x] Substitute project-owned materials on surfaces that used untracked
      materials. Landings, envelope, 4C floor, closed doors, code lock and
      remaining cube dressing now use tracked instances under
      `Rewind/Content/Art/Materials/`.
- [x] Preserve gameplay actors, camera regions, lights and blockout
      geometry that do not depend on untracked content. Post-edit inspect:
      same Rewind* actors and seven regions at the same transforms; actor
      count 65 → 59.
- [x] Editor-closed `RewindEditor Win64 Development` build.
- [x] All discoverable `Rewind.*` tests pass together, headless.
- [x] Clean-open check with untracked directories renamed or absent.
- [x] Correct `docs/CURRENT_STATUS.md` and `docs/FILESTRUCTURE.md`.
- [x] Signed `docs/JOURNAL.md` entry.
- [x] Archive this charter and restore `docs/CURRENT_TASK.md` from the
      template.
- [ ] Commit explicit paths only, confirm Fab/ and Art/Texture/ remain
      untracked, push, open PR. Do not merge.

## Decisions and Notes

- A checkpoint after each step or substep is required. Checklist is therefore updated along the work and `CURRENT_STATUS.md` is always updated when changes affect the behavior.
- Record decisions and assumptions within the frozen charter.
- Classify discoveries using `docs/TASK_WORKFLOW.md`.
- This clone at `C:\code\rewind-multiagents\rewind-reboot` has no
  `Rewind/Content/Fab/` and no `Rewind/Content/Art/Texture/` on disk. That
  is already the clean-open condition the charter requires; those
  directories will not be created, copied or deleted.
- A strings scan of the tracked umap found no full `/Game/Fab/` or
  `/Game/Art/Texture/` paths, but did find name-table entries `desk_lamp`
  and `ptish_radio`. The strings scan is a starting point only; the
  editor's dependency tools are authority.
- Discovery: the REW-0015 LFS object still contains the full `/Game/Fab/`
  and `/Game/Art/Texture/` import list. The REW-0019 save, made in a clone
  without those directories, had already dropped the package paths and left
  empty-mesh actors. This task deleted those actors and restored
  project-owned materials on the surfaces whose untracked overrides had
  become `None`.
- `electrical_panel` and the unnamed `StaticMeshActor` were not in the
  starting strings-scan list. Editor inspect showed both as
  `StaticMeshActor` with `static_mesh = None`. They existed only to hold a
  mesh this clone cannot resolve, so they were removed with the named Fab
  props. Gameplay `4C_BuildingFuseSocket` remains.
- `docs/SYSTEMDOC.md` still describes the saved map as referencing those
  untracked packages. It was outside this task's file ownership.

## Charter Amendment Log

Only non-semantic corrections are allowed after `Ready`.

-none

## Verification

- [x] Editor-closed `RewindEditor Win64 Development` build. First run:
      Succeeded in 57.06 s (four actions). Post-save run: Succeeded,
      target up to date, 1.94 s, 0 actions.
- [x] All discoverable `Rewind.*` automation tests pass together, headless.
      Seventeen tests listed, seventeen `Result={Success}`,
      `TEST COMPLETE. EXIT CODE: 0` in `Rewind/Saved/Logs/Rewind.log` at
      `2026.08.25-08.54.36`.
- [x] Clean-open check: `Rewind/Content/Fab/` and
      `Rewind/Content/Art/Texture/` are absent on this clone, so they were
      not renamed. A fresh `UnrealEditor-Cmd` load of
      `/Game/Maps/FiveLoops_Stairwell_Blockout` reported Asset Registry
      untracked deps `[]`, `missing_mesh=[]`, actor count 59, and no
      `LogLinker` / `Can't find file` lines for those packages. Saved umap
      strings contain zero `Fab`, `Art/Texture`, `desk_lamp` or
      `ptish_radio` tokens.
- [x] Not run: packaged build, live PIE walk of the 4C-hall-stairs-lift
      route, owner visual review of undressed 4C, Five Loops criteria.
      Collision geometry was not re-measured; the collision actors were not
      edited.

## Documentation Updates

- [x] `docs/CURRENT_STATUS.md`
- [ ] `docs/SYSTEMDOC.md` — outside file ownership; still describes the
      saved map as referencing untracked packages
- [x] `docs/JOURNAL.md`
- [x] `docs/FILESTRUCTURE.md` when structure changes
- [ ] ADRs when long-lived decisions change

## Handoff and Follow-ups

- Current state: complete. The tracked default map is unbound. Charter
  archived here; `docs/CURRENT_TASK.md` restored from the template.
- Next recommended step: operator review and merge. Generated or authored
  4C props are a later task under ADR-0011.
- Blockers: none.
- Child tasks: none.
- Resume condition: not paused.
- Open questions: none. `docs/SYSTEMDOC.md` remains stale until a later
  task that owns it.

## Finalize When Complete

- Archive this file under `docs/finished/`.
- Restore this template or populate the next approved task.
- Add a signed `docs/JOURNAL.md` entry.
- If Goal or Definition of Done changed, supersede this task instead of
  rewriting it.
