# ADR-0007 — Camera and perspective

Status: Accepted
Date: 2026-08-23
Deciders: MrWhite
Supersedes: nothing
Superseded by: nothing

## Context

The camera in the build is the engine template. `ARewindCharacter` creates a
`USpringArmComponent` at `TargetArmLength = 300` with
`bUsePawnControlRotation = true`, and binds `Turn` and `LookUp` to controller
yaw and pitch. Nothing in `docs/design/` asks for that. It arrived with the
Unreal third-person template and has never been decided.

The Five Loops Test passed under it, so it blocks nothing today. It constrains
everything after today. Camera choice fixes level geometry, sight lines,
interaction reach, animation requirements and every lighting decision, and each
of those is expensive to redo. The evidence from
[`five-loops-2026-08-22.md`](../playtests/five-loops-2026-08-22.md) also showed
the authored space has no time pressure, so level work is imminent. That work
should happen under a decided camera rather than produce a second space built
for an undecided one.

`docs/PROJECT_BRIEF.md` calls RE:WIND "a third-person psychological sci-fi
puzzle game". That names the player's relationship to the character. It does
not say the camera is player-controlled, and the two are separable: a
constrained camera is still third-person. This record says so explicitly so
that no later reader treats the brief as having settled the question.

Two non-authority sources point the same way and are noted as evidence of
intent rather than as rules. The imported art bible names Another World,
Flashback and Inside, minimalist retrofuturism, strong silhouettes and high
contrast. The imported GDD describes "tredjeperson med lätt tank-camera-känsla;
tight svängradie, inga floatiga hopp". Neither is authority, and neither is
being implemented by this record. They are recorded because the conflict
register exists to stop the imported material being read as settled, and here
it happens not to conflict.

## Decision

**The camera is authored, not player-controlled.**

1. **Third person is retained.** The character is visible and is not the
   viewpoint. First person is out.
2. **The player does not aim the camera.** Look input stops driving it, and
   `Turn` and `LookUp` cease to be camera controls.
3. **The camera follows the player inside an authored composition** rather than
   cutting between static positions. Framing, angle and distance are authored
   per region of the space; the camera moves within what the author allowed.
4. **Every playable region declares its camera.** A region with no authored
   camera is unfinished, not a region that falls back to a free camera.

The choice between following and cutting is part of this decision, and it is
made on gameplay grounds rather than taste. FL-09 and FL-12 require the player
to read a patrol window and a turnstile phase in order to act on them. A
cutting camera can put the thing being timed off screen at the moment the
player must judge it. A following camera inside a fixed composition keeps the
timed stage in frame while still letting the author own the frame.

## Alternatives considered

**Keep the free-orbit third-person camera.** Rejected. It makes every frame the
player's accident. The look this project is aiming at depends on foreground
occluders reading as black mass and on the figure reading against a controlled
background, and neither survives a camera that can be pointed anywhere. It also
makes lighting unboundedly expensive, because every angle must be lit.

**A fully fixed camera that cuts between static positions**, the survival-horror
grammar. Rejected on the gameplay ground above: the criteria that make this a
knowledge game are timing criteria, and a cut can hide the clock the player is
reading. Kept as a technique available inside a region, not as the model.

**First person.** Rejected. It contradicts the brief and removes the silhouette,
which is the one visual element the art direction is built on.

**Defer until the art phase.** Rejected, and this is the alternative with the
strongest surface appeal. Deferring costs nothing today and costs a rebuild
later, because the next level work either assumes a camera or is built twice.
Deciding now is cheap precisely because so little level content exists.

## Consequences

**Accepted costs:**

- The existing blockout was proportioned for an orbit camera. Some of it will
  not frame well and will be rebuilt.
- Level geometry stops being free to extend. A new room is not finished until
  its camera is authored.
- Animation must read from the authored angle. A character that reads from
  every direction is no longer the requirement, and a character that reads from
  the authored one becomes a hard requirement.
- Input changes: `Turn` and `LookUp` lose their current meaning.
- FL-09 and FL-12 legibility must be re-verified under the new camera. They
  pass today under a camera the player can point at the patrol.

**Accepted benefits:**

- Composition becomes something an author controls, which is the precondition
  for the reference look rather than a consequence of it.
- Lighting cost collapses. A known camera axis means a small number of authored
  key lights instead of coverage from every angle.
- **A stable frame makes world changes legible.** This is the benefit that
  matters most and it is mechanical, not decorative. When the player has seen
  the same composition many times, moving one object in it is something they
  notice. Under a free camera the author has to hope the player is looking the
  right way. RE:WIND's progression is the player learning the world, and a
  stable frame is what makes a world learnable by sight.

## What this does not decide

Field of view, aspect ratio, whether regions are volumes or rails, how
transitions blend, and whether any region uses a cut. Those are grammar and
belong in a design document under `docs/design/` when a task writes one.

It does not decide the authored space's scale or the loop duration. Those are
coupled to each other and are recorded in
[`loop-pressure-and-interaction.md`](../backlog/loop-pressure-and-interaction.md).

It does not adopt any art direction. That remains gated on the non-goal list in
`docs/PROJECT_BRIEF.md`.

It grants no authority to the imported GDD or art bible. Both are cited above
as evidence of intent and remain non-authority under
`docs/concepts_sandbox/legacy-rewind/`.
