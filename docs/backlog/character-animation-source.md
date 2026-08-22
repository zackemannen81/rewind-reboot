# Character animation source

Status: Proposed
Discovered: 2026-08-23, from an asset inventory by MrWhite
Owner: unassigned
Activated: not activated. No `REW` identity is claimed for this.

Where blockout character animation comes from, and how much of it enters the
repository. Not a decision about the character.

## Discovery context

Two archives on the owner's machine, outside this repository:
`Universal Animation Library[Source].zip` (46 MB) and
`Universal Animation Library 2[Source].zip` (50 MB), by Quaternius.

Verified by reading the archives directly:

| | UAL1 | UAL2 |
| --- | --- | --- |
| Animations | 120 | 134 |
| Skeleton | one skin, 65 joints | the same |
| Unreal-ready format | `UAL1.glb`, 21 MB | `UAL2.glb`, 20 MB |
| Root-motion variant | `UAL1_RM.glb` | `UAL2_RM.glb` |

**Licence: CC0 1.0 Universal**, stated in both `License.txt` and `README.txt`.
Public domain dedication, no attribution required, commercial use permitted.
That satisfies the `AGENTS.md` rule against committing third-party licensed
assets whose terms have not been checked: the terms were checked and they
impose nothing.

**The skeleton uses Unreal Mannequin bone naming.** Read from the GLB: `root`,
`pelvis`, `spine_01` to `spine_03`, `neck_01`, `clavicle_l`, `upperarm_l`,
`lowerarm_l`, `hand_l`, `thigh_l`, `calf_l`, `foot_l`, `ball_l`, and the
`index_01_l` style finger chains. That is Epic's convention, not Mixamo's
`mixamorig:` naming.

The joint count confirms it: the UE4 Mannequin hierarchy without IK bones is 53
joints, plus the 12 leaf bones a Blender export adds — five fingertip leaves per
hand and two `ball_leaf` — gives exactly the 65 in the skin.

Consequences for retargeting, which are better than a custom rig would allow:

- Onto a Mannequin-compatible character, close to drop-in. The leaf bones are
  harmless and Epic's `ik_foot_*` and `ik_hand_*` bones are absent, which
  matters only for content that expects them.
- Onto a Mixamo character, one IK Retargeter pass, because the names differ
  entirely. Routine, but it is a translation this rig does not otherwise need.
- A `root` bone exists, which is what makes the `_RM` variants behave.

A female mannequin mesh ships inside UAL2.

## Why a subset, and not the library

254 animations is not the size of the problem; it is the size of the import.
`ADR-0005` puts Unreal binaries in Git LFS, and `docs/CURRENT_STATUS.md`
records the LFS quota as a known risk with the note that a large art import is
a new decision. Each imported animation becomes a `.uasset`.

The content also does not match the game. UAL2 is dominated by material RE:WIND
has no use for: 31 `Sword_*` clips, 21 `Zombie_*`, 7 `Bow_*`, 7 `Melee_*`, plus
farming, fishing, mining and spellcasting. RE:WIND has no combat.

So the proposal is a named subset, in tiers, with each tier justified by work
that is actually chartered.

## Tier 1 — what ADR-0007 needs to be answered with a real body

Twenty-one clips. Enough to walk the FL chain, read the character from an
authored camera angle, and wake up in Apartment 4C.

From UAL1: `Idle_Loop`, `Idle_LookAround_Loop`, `Walk_Loop`,
`Walk_Formal_Loop`, `Jog_Fwd_Loop`, `Jog_Bwd_Loop`, `Jog_Left_Loop`,
`Jog_Right_Loop`, `Turn90_L`, `Turn90_R`, `Interact`, `Fixing_Kneeling`,
`PickUp_Kneeling`, `Counter_Enter`, `Counter_Idle_Loop`, `Counter_Exit`,
`Death01`.

From UAL2: `Turn180_L`, `Turn180_R`, `IdleToLay`, `LayToIdle`.

Notes on four of them. `Fixing_Kneeling` is the fuse box and the generator.
`Counter_*` is the code lock and the Anchor board. `IdleToLay` and `LayToIdle`
are waking in 4C, which is where every loop starts. The turn clips matter more
under ADR-0007 than they would otherwise: an authored camera makes a turn
visible where a follow camera hides it.

## Tier 2 — only if traversal work is activated

Roughly twenty-five clips, and none of them are needed unless
[`traversal-as-knowledge.md`](traversal-as-knowledge.md) becomes a chartered
task. Listing them so a later task does not have to re-derive the set.

Crouch and crawl sets and `Climb_*` from UAL1. `StepUp`, `ClimbUp_1m`,
`ClimbUp_2m`, `GetOffWall_2m`, `SafetyVault` and `Slide_Start/Loop/Exit` from
UAL2, plus the full directional `Walk_*` set that UAL1 lacks.

These are the verbs a branching route needs. They are worth nothing until there
is a branch.

## Tier 3 — never

Sword, bow, spell, shield, zombie, melee, farming, fishing, mining. Roughly
half of UAL2. Recorded so that "import the pack" is visibly the wrong shape of
action.

## What this is not

**Not a decision about the character.** These clips are clean, competent and
neutral. They are not rotoscoped, and they do not have the wind-up and settle
that [`art-direction.md`](art-direction.md) describes under P6. Used unedited
they read as stock. That is acceptable for a blockout and is not acceptable as
an answer to what RE:WIND's character moves like.

The value here is that they unblock near-term questions cheaply: whether the
authored camera reads, whether the interaction framings work, whether the space
scales sensibly against a human body. All three are hard to judge against a
capsule.

Two constraints a later character task inherits, recorded now because both are
cheaper to honour than to discover.

**Proportions bind harder than bone names do.** These clips store bone rotation
plus root translation. A character with different limb ratios receives the
poses but drifts at the contact points: feet slide, and hands miss the fuse box
they are supposed to touch. There is no IK correction in the raw clips. A
character modelled against the shipped armature's proportions avoids the whole
class of problem; one modelled freely inherits it.

**A long coat is the specific risk for this character.** The reference frames
this project is aiming at put the figure in one, and a skinned long coat
deforms badly on a standard humanoid rig — legs pass through it in the walk
cycle. The three ways out cost very different amounts: keep the coat above the
knee, which is a silhouette decision and therefore an art-direction one; add
coat bones, which these 65-joint clips will not drive; or simulate cloth, which
introduces a cost this project has nowhere else. It should be decided before
the mesh is built, not after.

## Dependencies

- `ADR-0005`, asset storage. The subset must be **measured** at import rather
  than estimated, and recorded, because the quota is a stated risk.
- [`ADR-0007`](../adr/ADR-0007_camera-and-perspective.md), accepted. Tier 1
  exists to serve it.
- A retarget decision, **only if** the project's own character is rigged
  independently. Both archives ship their Blender sources, `UAL1.blend`,
  `UAL2.blend` and `Mannequin_F.blend`, so a character skinned onto the
  existing armature needs no retargeting at all. That is the cheapest path and
  the one to rule out first, before any retarget work is planned.
- Tier 2 depends on traversal work being chartered, and nothing else.

## Suggested verification

- The imported subset's size on disk, measured and recorded against the LFS
  budget, not estimated.
- The character read from the authored camera angle rather than from a free
  orbit, since ADR-0007 makes that the requirement.
- Whether root motion or in-place is used, stated once. The player body is
  discarded at every loop start under `world-state-model.md` and is not a world
  clock, so neither choice affects determinism. This is a movement-feel
  decision, not a correctness one.

## What this proposal does not decide

It does not import anything, choose a character mesh, adopt a rig, decide
between the root-motion and in-place variants, or commit to Quaternius as a
long-term source. It does not touch art direction, which stays gated on the
non-goal list in `docs/PROJECT_BRIEF.md`.
