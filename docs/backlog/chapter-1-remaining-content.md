# Chapter 1 remaining content

Status: Proposed
Discovered: 2026-08-25, from the owner's remaining-work list during REW-0016
Owner: unassigned
Activated: not activated. No `REW` identity is claimed for this.

## Discovery context

On 2026-08-25 the owner listed what remains before Chapter 1 is finished. Three
items became chartered tasks in that wave: event-driven rewind and the
loop-break interference (REW-0017), the message and dialog system (REW-0018),
and renderer configuration with a first look pass (REW-0019).

This proposal holds what did not, so the list is not lost. Each item below is
recorded with why it was not chartered, because in every case the reason is a
missing decision or a missing input rather than a lack of appetite.

## Items

### Radio audio

The accepted rule in [`chapter-1-authored.md`](../design/chapter-1-authored.md)
already fixes the broadcast's timing: a 20-second sequence on a 50-second cycle,
speaking `7`, `3`, `1`, `2` at phases 4, 9, 14 and 19. What does not exist is
any audio at all — the digits are currently on-screen debug strings.

Not chartered because generating or sourcing audio is an asset decision, and
`docs/PROJECT_BRIEF.md` still lists sound design beyond an acceptance
criterion's needs as a non-goal. Any audio is also a world clock under
[`loop-and-determinism.md`](../design/loop-and-determinism.md) and must read
elapsed loop time.

### Meshes and textures

The owner named mesh generation through Meshy, or selected Fab assets, and
texture work. Both are blocked behind
[`untracked-fab-dependency.md`](untracked-fab-dependency.md): the repository
cannot take a position on new imports while the imports it already references
are undecided.

### Player animation and interaction breadth

The owner asked that animation and interaction not be limited to the objects the
golden path needs. Concretely, 21 Tier 1 clips are imported and two are bound.
`ARewindCharacter` uses `AnimationSingleNode` and hard-switches Idle and Walk on
a 5 cm/s velocity threshold, with no blend, no blendspace, no play-rate match at
the authored 200 cm/s, and no foot IK on the stairs — where the capsule rides an
invisible ramp over stepped geometry.

Not chartered in this wave because it collides with REW-0018's file ownership of
`RewindCharacter.*`. It is a clean successor task once that merges.

### The unbuilt last part of the level

Not chartered because no owner-approved plan states what it contains. The
settled ground-floor plan under [`../concept/`](../concept/) covers the
courtyard, generator, patrol yard and Transit exit, but a blockout task needs a
stated boundary before it can have a definition of done.

### The Apartment 4C lens

The owner reported the 4C frame as too zoomed in. REW-0015 corrected the
region's player volume while deliberately preserving the owner's adopted 35 mm
frame at `(750, 1330, 1330)`, FOV `37.497356`, so the reported symptom may or may
not survive that change. This wants the owner's eye against a rendered frame,
not a number chosen by argument, and is best judged after REW-0019 lands.

## Why it is outside the active charter

REW-0016 is an operator task. Its charter states that it does not implement
Chapter 1 content and does not decide Echo, asset provenance, audio or mesh
generation.

## Dependencies

- [`untracked-fab-dependency.md`](untracked-fab-dependency.md) gates meshes and
  textures.
- REW-0018 gates the animation work by file ownership.
- REW-0019 gates the lens judgement by making the frame worth judging.
- The non-goal list in [`../PROJECT_BRIEF.md`](../PROJECT_BRIEF.md) gates audio
  and any asset pipeline.

## Suggested verification

- Any loop-varying audio or effect checked across two idle loops at the same
  `t`, in the same form as FL-03.
- A stated repository and LFS cost before any import is committed.
- For animation, a capture of the same route before and after, since "weight" is
  a claim about motion and not about code.
