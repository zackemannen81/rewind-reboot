# Current Task

Task ID: REW-0026
Parent Task: None
Status: Ready
Owner: Claude
Created: 2026-08-25
Last updated: 2026-08-25
Charter frozen at: 2026-08-25

## Read First

- `AGENTS.md`
- `docs/TASK_WORKFLOW.md`
- `docs/PROJECT_BRIEF.md`
- `docs/CURRENT_STATUS.md`
- `docs/SYSTEMDOC.md`
- `docs/adr/ADR-0011_asset-source-and-provenance.md`
- `docs/adr/ADR-0005_asset-storage.md`
- `docs/design/stairwell-visual-direction.md`
- `docs/concept/README.md`

## Task Summary

The player is currently the CC0 Quaternius `Tier1_UAL1` mannequin rendered
through `MI_CharacterSilhouette`. It is a placeholder, and it is in every frame
of the game.

The owner has produced a character design, The Returner: a figure in a long
tattered coat, hooded in one reference and messy-haired in another, over dark
trousers and heavy boots. Every scene target under `docs/concept/` shows that
figure as a flat black silhouette against a lit field. Nothing but the outline
is ever visible.

ADR-0011 permits generated assets and requires provenance. The owner holds a
Meshy Pro subscription, so generation is available, and the input that matters
is a clean T-pose reference rather than a stylised scene render.

This task replaces the placeholder with a generated character whose silhouette
matches those targets.

## Task Charter

The charter is editable while status is `Draft` and immutable once status is
`Ready`.

### Goal

The player reads as The Returner in silhouette, in the frames the game
actually shows.

### Primary Deliverable

A generated, rigged character imported as a tracked skeletal mesh, driving the
player pawn with working idle and walk locomotion, with captures from the
authored 4C and stairwell frames as the evidence that the outline reads.

### In Scope

- Generation from the owner's T-pose reference through the Meshy API, with
  texturing disabled: the figure renders near-black, so albedo, roughness and
  metallic maps are never visible and are not worth paying for or tracking.
- Meshy's auto-rig pass and an FBX export carrying skeleton, bind pose and
  skin weights.
- Import as a `USkeletalMesh` with its own `USkeleton`, under
  `Rewind/Content/Characters/Returner/`, alongside the existing `Tier1/`.
- Locomotion: an idle and an in-place walk bound the way `ARewindCharacter`
  already expects, whether those come from Meshy's own clips or from
  retargeting the Quaternius set.
- Provenance recorded per ADR-0011: generator, model, source image, date.
- The source reference images and the exported FBX tracked under
  `Rewind/ArtSource/Characters/`, so the imported assets can be rebuilt.
- Captures from the authored Apartment4C and stairwell frames, committed as
  evidence under `docs/playtests/`.

### Out of Scope

- Animation breadth beyond idle and walk. The 21 Quaternius clips and any
  Meshy extras beyond locomotion are a separate task.
- Facial detail, cloth simulation, and any lit-material treatment. The
  silhouette material is accepted and unchanged.
- The Apartment 4C camera lens, which the operator handles separately.
- The courtyard, the textures re-bind (REW-0025), and anything in the map.
- Retiring `Tier1/`. It stays until the replacement is judged good.
- Changing `MI_CharacterSilhouette` or the accepted visual direction.

### Definition of Done

- The player pawn uses the generated character, not `Tier1_UAL1`.
- Idle and walk both play, and walk is in place: `ARewindCharacter` drives
  locomotion from `CharacterMovement` velocity and a root-motion clip would
  fight it.
- The figure's feet meet the capsule bottom and its forward axis matches
  movement, as the existing mannequin alignment already establishes.
- Captures from the authored 4C and stairwell frames show a silhouette whose
  outline reads as a coated figure: coat break below the knee, a distinct head
  mass, legs separating below the hem.
- Provenance is recorded where a reader will find it.
- `RewindEditor Win64 Development` builds with the editor closed and all
  discoverable `Rewind.*` automation tests pass together.

### Minimum Verification Gates

- [ ] Editor-closed build succeeds.
- [ ] All discoverable `Rewind.*` tests pass together, run headless.
- [ ] Captures from at least two authored frames, committed.
- [ ] Tracked binary cost measured and stated.
- [ ] The journal entry names what was verified and what was not.

## References

- `Rewind/Source/Rewind/RewindCharacter.cpp`, the mannequin binding and
  alignment
- `Rewind/Content/Characters/Tier1/`, the placeholder being replaced
- `docs/concept/apartment-4c-walkable-and-interactions.png` and the other
  scene targets, which show the silhouette in context
- `docs/backlog/character-animation-source.md`

## Checklist

- [x] Claim REW-0026 and charter the task.
- [x] Submit the T-pose reference to Meshy with texturing disabled.
- [ ] Run the rigging pass and export FBX.
- [ ] Track the sources under `Rewind/ArtSource/Characters/`.
- [ ] Import the skeletal mesh and its animations.
- [ ] Bind idle and walk in `ARewindCharacter`; fix alignment.
- [ ] Build and run the full test suite.
- [ ] Capture the authored 4C and stairwell frames.
- [ ] Measure the tracked binary cost.
- [ ] Update `CURRENT_STATUS.md`, `SYSTEMDOC.md`, `JOURNAL.md`.

## Decisions and Notes

- A checkpoint after each step or substep is required.
- **Why a whole character rather than a coat on the existing mannequin.** The
  owner asked which is best. Meshy's auto-rig works on a whole figure and will
  not skin a loose garment to an existing skeleton, so a separately generated
  coat arrives static and would need manual skin weighting in Blender. The
  approach that sounds simpler is the only one needing a tool this project does
  not use.
- **Why imperfect deformation is acceptable here.** The figure renders
  near-black. Clipping that would be glaring on a lit character is invisible
  when the coat and the leg are the same flat black. The bar is a correct
  outline, not a correct simulation.
- **Why the T-pose reference and not the coated render.** `coatman.png` has the
  hair silhouette the scene targets show, but its arms rest against the body,
  and auto-riggers fuse limbs into the torso when they touch. The T-pose keeps
  the arms clear. That is the likeliest reason the owner's earlier attempts
  produced poor meshes.
- **Open risk, stated before the work.** The T-pose is hooded and the scene
  targets show messy hair. In silhouette a hood is a smooth dome and hair is a
  jagged mass, which are different outlines. If the capture reads wrong, the
  remedy is another generation, not a rework.
- Texturing is disabled deliberately. The earlier attempts spent credits on
  albedo, roughness and metallic maps totalling 10 MB that the silhouette
  material makes invisible.
- Classify discoveries using `docs/TASK_WORKFLOW.md`.

## Charter Amendment Log

Only non-semantic corrections are allowed after `Ready`.

-none

## Verification

- [ ] Editor-closed build.
- [ ] Full `Rewind.*` automation run, headless.
- [ ] Captures from the authored 4C and stairwell frames.
- [ ] Measured tracked size of everything added.
- [ ] Document skipped checks and reasons.

## Documentation Updates

- [ ] `docs/CURRENT_STATUS.md`
- [ ] `docs/SYSTEMDOC.md`
- [ ] `docs/JOURNAL.md`
- [ ] `docs/FILESTRUCTURE.md` when structure changes
- [ ] ADRs when long-lived decisions change

## Handoff and Follow-ups

- Current state: chartered; generation submitted to Meshy with texturing off.
- Next recommended step: rig, export FBX, import, bind, capture.
- Blockers: none.
- Child tasks: none.
- Resume condition: not paused.
- Open questions: whether the hooded outline satisfies the owner against scene
  targets that show hair; and whether Meshy's own clips or retargeted
  Quaternius clips give the better walk.

## Finalize When Complete

- Archive this file under `docs/finished/`.
- Restore this template or populate the next approved task.
- Add a signed `docs/JOURNAL.md` entry.
- If Goal or Definition of Done changed, supersede this task instead of
  rewriting it.
