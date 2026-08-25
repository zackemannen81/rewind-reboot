# Current Task

Task ID: REW-0019
Parent Task: None
Status: Complete
Owner: grok-look
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
- Relevant ADRs under `docs/adr/`
- `docs/design/stairwell-visual-direction.md`
- `docs/design/camera-and-movement.md`
- `docs/backlog/art-direction.md`
- `docs/concept/README.md`
- `docs/EDITOR_AUTOMATION.md`

## Task Summary

The authored slice cannot be judged against the accepted visual grammar while
the renderer is still in its blockout configuration. Unlit surfaces render at
zero because nothing bounces, nothing reflects, and bloom is off, so the 4C
sign reads as a sticker rather than a light source. This task records the
renderer decision, applies it, and shapes light and exposure in the authored
default map so the first-pass grammar can actually be seen.

## Task Charter

The charter is editable while status is `Draft` and immutable once status is
`Ready`.

### Goal

The authored slice renders with indirect light, reflections and bloom, so
the accepted visual grammar can actually be judged.

### Primary Deliverable

A decision record for the renderer configuration, that configuration
applied, and light and exposure shaping in the authored default map that
demonstrates the accepted first-pass grammar.

### In Scope

- A new ADR under docs/adr/ deciding which of Lumen global illumination,
  reflections, virtual shadow maps and mesh distance fields are enabled,
  what the frame-time budget is, and what the target hardware floor is.
  Take the next free ADR number, add its row to docs/adr/README.md, and
  state the alternatives and consequences, not just the choice.
- Applying that configuration in Rewind/Config/DefaultEngine.ini.
  r.DefaultFeature.AutoExposure stays False: exposure is authored.
- Light shaping in the authored default map: practicals with real falloff
  and source size rather than hot circular blowouts, restrained cool fill,
  and a value range in which navigation surfaces never fall to pure black.
- Emissive sources that also emit light, so the 4C sign bleeds onto its
  wall instead of floating.
- Measured frame time on this machine, before and after, at a stated
  position in the map.
- Captured frames from stated positions, committed under docs/playtests/,
  as the evidence that the grammar reads.

### Out of Scope

- Any C++ under Rewind/Source/. REW-0017 and REW-0018 own that.
- New third-party asset imports. The untracked /Game/Fab/ and
  /Game/Art/Texture/ content in the owner's tree is an undecided
  provenance and quota question under ADR-0005, and is not yours to
  resolve. Do not add, remove or commit it.
- Final environment art, an environment kit, decals, rain or an asset
  pipeline. docs/PROJECT_BRIEF.md still lists those as non-goals.
- Camera field of view and camera placement, which the owner is judging
  separately.
- docs/TASK_IDS.md. The operator owns it and already claimed REW-0019.

### Definition of Done

- The ADR exists, is Accepted, names alternatives and consequences, and is
  indexed in docs/adr/README.md.
- DefaultEngine.ini matches the ADR.
- A frame captured from a stated position shows indirect light, a readable
  value range with no navigation surface at pure black, and the 4C sign
  lighting the surface behind it.
- Frame time before and after is measured on a named machine and recorded.
- The build opens the authored default map and the existing Rewind.*
  automation tests still pass together.

### Minimum Verification Gates

- [x] RewindEditor Win64 Development builds with the editor closed.
- [x] All discoverable Rewind.* automation tests pass together, run headless.
- [x] Captured frames and measured frame time are committed as evidence.
- [x] The journal entry names what was verified and what was not.

## References

- `docs/design/stairwell-visual-direction.md`
- `docs/backlog/art-direction.md` Stage 3 (renderer decision; that proposal
  numbered the ADR as 0008, which is already taken by
  `ADR-0008_what-an-anchor-is-worth.md`)
- `docs/adr/ADR-0001_engine-unreal-engine-5.md`
- `docs/adr/ADR-0005_asset-storage.md`
- `Rewind/Config/DefaultEngine.ini`
- `Rewind/Content/Maps/FiveLoops_Stairwell_Blockout.umap`
- `docs/concept/stairwell-4c-landing.png`
- `docs/concept/apartment-4c-interior.png`
- `docs/EDITOR_AUTOMATION.md`

## Checklist

- [x] Claim already merged to `main` by the operator (`ef308c2`).
- [x] Branch `grok/rew-0019-renderer-and-look` created from current `main`.
- [x] Charter transcribed verbatim; status `Ready`; freeze date 2026-08-25.
- [x] Record pre-change renderer settings, machine identity, and frame time
      at a stated position under the current blockout configuration.
      Evidence: `docs/playtests/renderer-look-2026-08-25.md`.
- [x] Write ADR-0010 with alternatives, consequences, frame-time budget and
      hardware floor; index it in `docs/adr/README.md`.
- [x] Apply the decided configuration in `Rewind/Config/DefaultEngine.ini`.
      Keep `r.DefaultFeature.AutoExposure=False`.
- [x] Build `RewindEditor Win64 Development` with the editor closed.
      Evidence: 3.87 s, target up to date.
- [x] Shape practicals, cool fill, authored exposure and the 4C emissive-plus-
      light in the authored default map. Do not touch untracked Fab/Texture
      content.
- [x] Capture frames from stated positions; measure after frame time at the
      same position as the before measurement.
- [x] Run all discoverable `Rewind.*` automation tests headless together.
      Evidence: nine tests, `TEST COMPLETE. EXIT CODE: 0`.
- [x] Update `docs/CURRENT_STATUS.md`, `docs/SYSTEMDOC.md`,
      `docs/playtests/`, `docs/JOURNAL.md` and `docs/FILESTRUCTURE.md` if
      structure changes.
- [x] Archive this charter and restore `docs/CURRENT_TASK.md` from the
      template.
- [ ] Commit, push, open the pull request. Do not merge.

## Decisions and Notes

- A checkpoint after each step or substep is required. Checklist is therefore updated along the work and `CURRENT_STATUS.md` is always updated when changes affect the behavior.
- Record decisions and assumptions within the frozen charter.
- Classify discoveries using `docs/TASK_WORKFLOW.md`.
- The next free ADR number on this trunk is 0010. The art-direction backlog
  called the renderer decision ADR-0008; that number is already owned.
- Auto-exposure stays off. That is already correct for
  `docs/design/stairwell-visual-direction.md` ("Exposure is authored and
  stable") and is not reopened.
- Substrate stays on. This task does not decide character shading or a
  Substrate material rewrite.
- Untracked `/Game/Fab/` and `/Game/Art/Texture/` packages, including
  `4c_neon_sign_Mat`, remain referenced by the saved map and remain
  unresolved in this clone. They are not restored, deleted or committed.
  A project-owned 4C sign (emissive surface plus a real light) is added so
  the grammar can be judged without those packages.
- Camera FOV and placement are out of scope; captures use the existing
  authored 4C start frame and the existing stairwell composition.

## Charter Amendment Log

Only non-semantic corrections are allowed after `Ready`.

-none

## Verification

- [x] `RewindEditor Win64 Development` build, editor closed, duration recorded.
      3.87 s, target up to date, no C++ change.
- [x] All discoverable `Rewind.*` automation tests pass together, headless.
      Nine tests, `TEST COMPLETE. EXIT CODE: 0`.
- [x] Before and after frame time at a stated map position, named machine.
      8.54 ms before, 13.03 ms after, 361 ticks, 1920×1080 `-game`.
- [x] Captured frames from stated positions committed under `docs/playtests/`.
- [x] Captures reviewed against the testable statements in
      `docs/design/stairwell-visual-direction.md`.
- [x] `DefaultEngine.ini` matches the ADR.
- [x] Git LFS pointers for any new/changed `.uasset` / `.umap` / `.png`.
- [x] `git diff --check` clean.
- [x] Documented skipped checks and reasons: no packaged build, no `stat unit`
      GPU split, no stairwell PIE at 35 mm, no FL re-run, no Fab provenance.

## Documentation Updates

- [x] `docs/CURRENT_STATUS.md`
- [x] `docs/SYSTEMDOC.md`
- [x] `docs/JOURNAL.md`
- [x] `docs/FILESTRUCTURE.md` when structure changes
- [x] ADRs when long-lived decisions change

## Handoff and Follow-ups

- Current state: Complete. Ready for operator review of the pull request.
- Next recommended step: owner visual review of the 4C after frame. Do not
  treat this as final art.
- Blockers: none.
- Child tasks: none.
- Resume condition: not applicable; the task is complete.
- Open questions: untracked `/Game/Fab/` and `/Game/Art/Texture/` remain
  referenced and unresolved; still ADR-0005, not this task.

## Finalize When Complete

- Archive this file under `docs/finished/`.
- Restore this template or populate the next approved task.
- Add a signed `docs/JOURNAL.md` entry.
- If Goal or Definition of Done changed, supersede this task instead of
  rewriting it.
