# Camera and movement space

Status: Accepted
Owns: how the world is framed, and the volume the player may move in inside a
frame
Does not own: loop and determinism rules, Anchors, world-state apply order,
authored Chapter 1 facts, art direction, level content
Source decision: [`ADR-0007`](../adr/ADR-0007_camera-and-perspective.md)

A rule that is not in this document is not a camera or movement-space rule.

ADR-0007 decided that the camera is authored and not player-controlled. This
document states the rules that follow from it.

## Regions

The authored space is divided into **camera regions**. Every playable location
belongs to exactly one region.

A region declares all of the following. A region that declares fewer is
unfinished, not a region that falls back to a default.

1. The camera's rotation, which is fixed for the region.
2. A single **travel axis**, the one axis along which the camera may move.
3. **Bounds** on that axis: the camera's first and last permitted position.
4. A **dead zone**, the centred band the player may move within before the
   camera responds.
5. The **player volume**: the bounded space the player may occupy in the region.

Regions are authored per location, not derived from geometry. Two rooms of the
same shape may frame differently, and that is the point.

## The camera inside a region

**Rotation does not change during play.** No look input reaches it, and no
gameplay event rotates it. A region that needs a different angle is a different
region.

**The camera moves on one axis only**, the region's travel axis. It does not
track the player on the other two.

**It does not move while the player is inside the dead zone.** When the player
leaves the dead zone, the camera follows so that the player remains at the
zone's edge, and it stops again when the player re-enters.

**Its position is clamped to the region's bounds**, so a region can never show
space that was not composed for it.

**Motion is eased, not snapped**, except on an authored cut and at loop start,
where the camera takes its region's position immediately.

**The player is not kept centred.** Centring is what removes composition: a
frame that always holds the player in the middle is a frame that never holds
anything else on purpose.

## Handoff between regions

When the player crosses a region threshold, the entering region's camera takes
over.

**A blend is the default.** A cut is permitted only where the threshold is a
real visual break, such as a doorway or an elevator, and it is authored on the
threshold rather than chosen at runtime.

**A region that contains an object the player must time keeps that object
framed for the whole region.** FL-09 and FL-12 require the player to read a
patrol window and a turnstile phase in order to act on them. A frame that hides
the thing being timed makes the criterion unreadable, whatever the composition
looks like.

## Interaction framing

An interaction may move the camera to an authored close-up for the duration of
that interaction, and returns to the region's camera when it ends.

A close-up is part of the region's authored set. It is not a separate region and
it does not change the player volume.

**A close-up may not be used in a region that contains a timed object**, for the
same reason as the handoff rule. The four objects that want close-ups — the
radio, the code lock, the fuse box and the Anchor board — are in Apartment 4C
and the courtyard, and neither holds a timed object, so the rule costs nothing
where it matters.

## The player's movement space

**The player moves inside a bounded volume, not on a plane.**

A hard plane constraint was rejected on evidence. The authored objects are
spread laterally by design: the Anchor board sits at `y = -360`, the radio and
the generator at `y = -220`, the fuse box at `y = +220`. Locking the player to
`y = 0` makes four of the eight interactables unreachable. The space was built
with lateral spread, and a camera decision does not get to delete it.

The volume is authored per region and is as narrow as the region's content
allows. Under a long lens, movement along the depth axis reads as small screen
movement, which is what makes a narrow band worth having: the player can reach
things without the composition changing much.

The player volume is not the same as the region's collision geometry. It may be
narrower. Where it is, that is an authored decision and not an accident of what
happens to be walled.

## What the camera may not do

**It never writes world state.** It reads positions and nothing else. This is
not tidiness: a camera that can write is a camera that can enter the
determinism model, and
[`loop-and-determinism.md`](loop-and-determinism.md) requires world
configuration at time `t` to be a function of baseline, Anchors, `t` and
this-loop input.

**No world object reads the camera.** Nothing about patrol phase, turnstile
phase, gate state or knowledge may depend on where the camera is or what it can
see.

The camera may use per-frame delta for its own smoothing.
`loop-and-determinism.md` already permits this: frame delta may move a
presentation, not a phase. The camera is presentation.

## Testable statements

These are the statements later acceptance may cite. They are not themselves
acceptance criteria.

1. Every playable location is inside exactly one region, and every region
   declares rotation, travel axis, bounds, dead zone and player volume.
2. Within a region, camera rotation does not change during play.
3. The camera does not move while the player is inside the dead zone.
4. The camera's position on its travel axis is always within the region's
   authored bounds.
5. Every authored interactable is reachable by the player from inside the player
   volume of its region.
6. In a region containing an object whose phase the player must read, that
   object is in frame from every position in the player volume.
7. No world object's state at time `t` changes if the camera is moved.
