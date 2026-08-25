# The default map depends on untracked content

Status: Proposed
Discovered: 2026-08-25, during REW-0015, from a strings scan of the saved map
Owner: unassigned
Activated: not activated. No `REW` identity is claimed for this.

## Discovery context

`Rewind/Content/Maps/FiveLoops_Stairwell_Blockout.umap` is tracked and is the
editor and game default. Its package references include:

- `/Game/Fab/Armchair_and_ottoman/scene/StaticMeshes/scene`
- `/Game/Fab/Coffee_Table/fbx/StaticMeshes/fbx`
- `/Game/Fab/Desk_Lamp/desk_lamp/StaticMeshes/desk_lamp`
- `/Game/Fab/PTish_Radio/ptish_radio/StaticMeshes/ptish_radio`
- `/Game/Fab/TILABLE_FLOOR_2_LOW_POLY/...`
- `/Game/Art/Texture/4c_city_Mat`, `4c_door_Mat`, `4c_door_lock_Mat`,
  `4c_neon_sign_Mat`, `4c_stove_Mat`, `4c_tiles_Mat`, `4c_walltexture_Mat`

None of those packages is tracked. `Rewind/Content/Fab/` measures 342 MB and
`Rewind/Content/Art/Texture/` measures 244 KB, both untracked in the owner's
working tree only.

The references first appear in the REW-0014 archive commit. That task's journal
entry correctly records that it left the owner's new content unstaged; what it
could not see is that the map it saved had already bound to it.

Method note: this was read from the package binary, not from the editor's
reference viewer. The exact live set should be confirmed in-editor before any
task acts on it.

## Why it matters now

A fresh clone resolves none of these. The default map opens with missing
references for every actor that uses them. The three delegated Chapter 1 agents
each work in such a clone, so this is not hypothetical.

`docs/CURRENT_STATUS.md` said, until REW-0015, that locally imported Fab and
environment packs "remain outside the tracked dependency closure". That was true
when written and is no longer true of the map.

## Proposed outcome

An explicit decision, recorded under `docs/adr/`, choosing one of:

1. **Commit them.** Track the selected subset through Git LFS. Requires a
   provenance and licence check per pack, and a measured LFS quota cost.
   `docs/adr/ADR-0005_asset-storage.md` already states that a large art import
   is a new decision.
2. **Replace them.** Rebuild the dressing from project-owned assets, and remove
   the references from the map.
3. **Unbind them.** Keep the packs local and strip the references, accepting an
   undressed default map until a selected prop set is decided.

Doing nothing is not one of the three: the repository currently claims a default
map it cannot reproduce.

## Why it is outside the active charter

REW-0015 was a bounded control-defect fix. REW-0019's charter explicitly forbids
resolving it, because a look pass must not become an asset-import decision by
accident.

## Dependencies

- [`ADR-0005`](../adr/ADR-0005_asset-storage.md)
- The non-goal list in [`../PROJECT_BRIEF.md`](../PROJECT_BRIEF.md), which still
  excludes an asset pipeline

## Suggested verification

- The editor's reference viewer, not a strings scan, for the authoritative set.
- A clone-and-open test: the chosen option is right when a fresh clone opens the
  default map with no missing references.
- Measured repository and LFS size before and after.
