# Art Direction: cinematic silhouette, flat-fill figure, neon accent

Status: Proposed
Discovered: 2026-08-22, during REW-0003, from a direction request by MrWhite
Owner: unassigned
Activated: not activated. No `REW` identity is claimed for this.

This is a roadmap, not authority. Nothing below is a rule until a task
restates it in an owning document under `docs/design/`. No stage in it may
start while `docs/PROJECT_BRIEF.md` lists final art as a non-goal.

## Discovery context

The reference set is three things held together:

1. **Four supplied frames**, 2026-08-22. Observable properties, not a title:
   ultra-wide letterboxed framing; a locked, near side-on, long-lens camera;
   large areas of pure black foreground that frame a lit stage; a figure in a
   coat rendered as flat unlit colour fields against a photographic
   environment; cold desaturated blue-grey base with a handful of saturated
   accents (rust red, a red interior window, green CRT); volumetric haze
   separating depth layers; two frames are pushed-in interaction close-ups of
   a diegetic object, a keypad and a cassette radio, with everything else
   falling into darkness; one frame carries RGB-split glitch across the whole
   image.
2. **Another World (1991) and Flashback (1992).** Flat-shaded untextured
   figures in a limited palette; rotoscoped motion with real weight and
   wind-up; a camera that treats the world as a series of composed screens;
   no HUD, or a diegetic one.
3. **Neon.** Saturated emissive accent against the cold base.

`docs/design/README.md` lists five accepted documents. None of them owns how
the game looks. There is therefore no rule to conflict with, and no rule to
build against either.

The previous project's art bible in
[`GDD_ArtStyleAssets.md`](../concepts_sandbox/legacy-rewind/original-gdd/GDD_ArtStyleAssets.md)
already names Another World, Flashback, Inside, brutalism and neon-noir, and
already proposes a loop-end desaturation and wireframe flash. It is sandbox
material. It is evidence that this direction is not new to the project. It is
not authority, and citing it here does not make it authority.

## The property list

Achieving the look means achieving these eight properties. They are listed
separately because they have different costs, different owners and different
risks, and because a task can pass or fail each one on its own.

### P1 — The frame is composed, not followed

The reference frames are shot, not chased. A long lens, a locked or rail-bound
camera, a wide crop, and depth built from separated parallax layers. Another
World and Flashback both work this way: the camera owns the screen, and the
player moves inside it.

This is the property everything else depends on. A flat-fill figure only reads
against a controlled background. Black negative space only works if the camera
cannot look somewhere the composition was not built for.

### P2 — Black is geometry, not a post effect

In the reference frames the darkness has edges: pillars, ceilings and fences
sit in front of the camera as unlit black mass and cut the image into a
letterbox inside the letterbox. A vignette applied in post does not produce
that. Foreground occluders do.

### P3 — The figure is flat fill

The coat is one tan field. The hair is one black mass. There is almost no
shading and no specular. The figure reads as a cutout laid over a photographic
world. That contrast between a flat character and a lit environment is the
direct descendant of Another World's untextured polygons, and it is what makes
a low-detail character legible instead of cheap.

### P4 — The ground is photographic and cold

Concrete, chain-link, rust, wet asphalt, blue-grey. Detail lives in the
environment, not in the character. Saturation is spent, not sprayed: a small
budget of accent colour per frame.

### P5 — Neon is sourced, and it means something

Neon in this direction is not bloom on everything. It is a small number of
emissive surfaces that also emit light, bleed into the haze, and reflect in wet
ground.

The proposal that makes this direction serve the product rather than decorate
it:

> The deterministic world is grey. Knowledge is neon.

The world the player cannot change stays cold, photographic and desaturated.
The things the player learns to read — the turnstile's phase, the patrol's
cone, a committed Anchor on the board, the loop's remaining time — are the
things that glow. A player on loop five sees a different image from a player on
loop one, because they can now read the lights. That is
`docs/PROJECT_BRIEF.md`'s "make the player's growing competence visible to the
player", stated in colour.

This is the one part of this roadmap that is a game rule and not decoration,
and it is the part that most needs an owning design document before anyone
builds it.

### P6 — Motion has weight

Rotoscoped animation reads as weight and commitment: a wind-up, a settle, no
free-floating blends. In engine terms that is short blend times, authored root
motion, few states, and no procedural smoothing that erases the pose.

The current character walks with a `MaxWalkSpeed` of 500 and has no animation
authority at all. Any claim about weight is unbuilt.

### P7 — Interaction is a camera move to a diegetic object

Two of the four reference frames are the same idea: the camera pushes in, the
object fills the frame, a soft key light picks it out, the rest of the scene
drops away. The interface is the object. RE:WIND already has the objects for
this — the radio, the code lock and the Anchor board — and those are exactly
the three places where the player acquires knowledge.

### P8 — The loop has a visual signature

One reference frame is glitched across its full width. RE:WIND has a reason for
that image that a reference does not: the loop ends. A desaturation that starts
in the final stretch, a geometry-only or wireframe flash at reset, and a hard
snap back is a clock the player can read without a HUD.

Constraint, and it is not optional: see "Determinism binds the visuals" below.

## Why it is outside the active charter

- [`../CURRENT_TASK.md`](../CURRENT_TASK.md), REW-0003, lists "Environment
  polish, final art" as out of scope, and its charter is frozen.
- [`../PROJECT_BRIEF.md`](../PROJECT_BRIEF.md) lists environment polish, final
  art, an environment kit and an asset pipeline as non-goals for this phase,
  and states that a non-goal moves only by an explicit decision that changes
  that document, never by a task deciding it is ready.
- REW-0003's remaining work is a clean-save run of FL-01 to FL-16. Changing
  camera, lighting or post now would invalidate that run before it happens, and
  could change whether an interaction is even reachable.

## Roadmap

Stages are ordered by dependency, not by appetite. Each names what it decides,
what it produces, what it costs and what counts as evidence.

### Stage 0 — Gate: pass the Five Loops Test first

Nothing in this roadmap starts until FL-01 to FL-16 have named evidence from a
clean save and REW-0003 is archived, and until a decision has changed the
non-goal list in `docs/PROJECT_BRIEF.md`.

Reason: the product hypothesis is unproven. A beautiful build of a loop that is
not fun is the previous project's failure with a better renderer.

Evidence: an archived task under `docs/finished/` for REW-0003; the brief's
non-goal list changed by an explicit decision.

### Stage 1 — Decide the frame (ADR-0007, camera and perspective)

The first decision, because it constrains level geometry, blockout, animation,
interaction reach and every later art stage. Deciding it late means rebuilding
the space.

The question: is RE:WIND filmed from a constrained, side-on, composed camera in
the Another World and Flashback lineage, or from a free-orbit third-person
camera?

`docs/PROJECT_BRIEF.md` says "third-person psychological sci-fi puzzle game".
That is a statement about the player's relationship to the character, not about
camera freedom. A constrained camera is still third-person. The two documents
are reconcilable, but the ADR must say so explicitly rather than leave a reader
to assume it.

Recommendation to consider, not a decision: constrained camera volumes with a
long lens and a fixed wide aspect, a shallow depth axis, and authored camera
rooms that hand off as the player crosses thresholds. Every one of P1, P2, P3
and P7 is cheaper under that model and expensive without it.

Current state, as evidence: `Rewind/Source/Rewind/RewindCharacter.cpp` builds a
`USpringArmComponent` at `TargetArmLength = 300` with
`bUsePawnControlRotation = true`, and binds `Turn` and `LookUp` to controller
yaw and pitch. The build is a free-orbit third-person camera today. Nothing in
`docs/design/` requires that; it came from a template.

Cost: a camera decision may invalidate blockout proportions authored for an
orbit camera, and it changes how FL-09's patrol window and FL-12's turnstile
are read by a player.

Evidence: a camera test in the existing FiveLoops blockout, not a document. One
build, two camera modes, the same four areas, captures of each.

### Stage 2 — Write the owning documents

Two documents, one area each, per `docs/design/README.md`:

- `docs/design/visual-identity.md` — owns palette, saturation budget, contrast
  policy, the grey-world and neon-knowledge rule, silhouette policy, and what
  neon is allowed to mean. This is where P5 stops being a mood and becomes a
  rule with a testable consequence.
- `docs/acceptance/look-test.md` — owns what a build must demonstrate.
  Identifiers `LT-01` onward, permanent, never renumbered, in the same form as
  [`five-loops-test.md`](../acceptance/five-loops-test.md).

Candidate criteria, to be written properly by the activating task:

- **LT-01** From the standard capture pose in each of the four areas, the
  player silhouette is separable from the background with no outline shader.
- **LT-02** No frame in the four areas spends more than a stated fraction of
  its pixels on accent-saturated colour.
- **LT-03** Every emissive surface above a stated brightness has a light that
  affects the fog, and is not emissive-only.
- **LT-04** A neon element that encodes a loop-clocked fact reads correctly at
  a stated `t` and matches across two idle loops.
- **LT-05** The interaction close-up on the radio, the lock and the Anchor
  board frames the object, and does not use a screen-space menu.
- **LT-06** The loop-end signature is visible without the debug overlay and is
  identical across two idle loops at the same `t`.
- **LT-07** Frame time on the reference machine stays inside a stated budget
  with the full stack enabled.

Cost: documentation only. No binaries.

Evidence: manual review that no rule is duplicated between the new document and
the five accepted ones, and that every `LT` criterion cites its owner.

### Stage 3 — Decide the renderer (ADR-0008)

**The current project configuration cannot produce the target look.** This is
the most concrete finding in this roadmap. In `Rewind/Config/DefaultEngine.ini`
today:

| Setting | Value | Consequence for this direction |
| --- | --- | --- |
| `r.DynamicGlobalIlluminationMethod` | `0` | No Lumen GI. Neon does not bleed onto nearby surfaces |
| `r.ReflectionMethod` | `0` | No reflections at all. Wet ground cannot carry neon |
| `r.Shadow.Virtual.Enable` | `0` | No virtual shadow maps. Hard contact shadows and shafts through fences are weaker |
| `r.GenerateMeshDistanceFields` | `False` | No distance fields. Blocks several fog and shadow techniques |
| `r.DefaultFeature.Bloom` | `False` | Emissive sources do not glow by default |
| `r.DefaultFeature.AutoExposure` | `False` | Correct for this direction. Keep it. Exposure must be authored, not adaptive |
| `r.Substrate` | `True` | Already on. Useful for flat-fill and emissive materials |

Those settings are a deliberate blockout configuration and they were the right
call for REW-0003. Reversing them is a real cost — build times, shader
compilation, frame time, and a hardware floor for the game — so it is a
decision record, not a config edit made in passing.

The ADR decides which of Lumen GI, Lumen or screen-space reflections, virtual
shadow maps and distance fields are turned on, what the frame-time budget is,
and what the target hardware floor is.

Evidence: the same scene captured under each candidate configuration, plus
measured frame time. Not an assertion that it looks better.

### Stage 4 — The grey pass, on the existing blockout

The first stage that produces an image, and it deliberately produces it without
new art. P1, P2 and P4 only.

Means in Unreal 5.8:

- A post-process volume owning the tone curve: authored exposure, a crushed toe
  so black is black, film grain, a restrained vignette, no auto exposure.
- Exponential height fog with volumetric fog on, so depth layers separate.
- Foreground occluder meshes as black unlit mass, per P2, built from the boxes
  that already exist.
- The camera model from Stage 1, with a fixed aspect ratio.
- Lighting rebuilt as composition: a small number of motivated key lights, most
  of the scene falling off to nothing. Today
  `Rewind/Source/Rewind/RewindProofLayout.cpp` spawns one directional light at
  50000, a sky light at 1.5, and point lights at 5000 with an 1800 radius, in
  code, to make a blockout visible. That is functional lighting. It is not
  composed lighting, and it should move out of `RewindProofLayout` into the map
  or a dedicated lighting actor when this stage runs.

Cost: small. Materials, a post-process volume and light placement are small
assets. This stage is the cheapest possible proof that the direction is
reachable, which is exactly why it comes before any mesh or texture work.

Evidence: four captures, one per area, at a stated `t`, reviewed against the
property list.

### Stage 5 — The figure

P3 and P6.

- Flat fill via an unlit or near-unlit material with a hard ramp, not a
  photoreal skin shader. With Substrate already enabled, a slab with no
  roughness variation plus a small ramp lookup is the direct route. A
  post-process material keyed on custom depth and stencil is the alternative
  when the figure must stay flat regardless of scene lighting.
- Silhouette first: the coat shape, the hair mass and the stance carry the
  character. Facial detail is not part of this direction.
- Animation with short blends, authored poses, and no procedural smoothing.

Cost: the first real binary cost. A character mesh and an animation set are the
point where [`ADR-0005`](../adr/ADR-0005_asset-storage.md) and the LFS quota
start to matter. Budget it in the activating task, not afterwards.

Evidence: the figure captured against all four areas, checked for LT-01.

### Stage 6 — The neon pass

P5, and this is where the roadmap touches game rules.

- Emissive material plus a real light per neon source, with volumetric
  scattering so the colour lives in the fog.
- Wet-surface response, which depends on the Stage 3 reflection decision.
- Accent colour spent against the LT-02 budget.
- Every neon element that encodes a fact the player is meant to learn is
  registered against the rule in `docs/design/visual-identity.md`, so the
  language stays consistent: if cyan means "timed and readable", nothing
  decorative may be cyan.

Cost: moderate. Mostly materials and lights.

Evidence: LT-02, LT-03, LT-04.

### Stage 7 — Interaction close-ups and diegetic UI

P7. The radio, the code lock and the Anchor board get authored close-up
framings, a key light, and their own readable surface.

The current build types digits with number keys bound in
`RewindCharacter::SetupPlayerInputComponent`, and prints a control hint plus the
`t=` elapsed-time value through `AddOnScreenDebugMessage`. That overlay is a
development instrument for stated-time checks. It is not UI, and this stage must
state in writing that it is not, so that no later task treats an engine debug
string as the game's interface.

The Anchor board is the highest-value object here. It is where knowledge becomes
visible, and it is the natural home for the "ghost chalk" idea the sandbox art
bible already describes. That idea has to be restated in an owning document to
be real.

Cost: small to moderate. Camera work, materials, and a small amount of code to
drive framing.

Evidence: LT-05, and a playtest in which a first-time player finds the code
without a text prompt.

### Stage 8 — The loop signature

P8. Desaturation over the final stretch, a geometry-only or wireframe flash at
reset, a hard snap back at `t = 0`.

Cost: one post-process material and a driver that reads elapsed loop time.

Evidence: LT-06, checked at a stated `t` across two idle loops, in the same form
as FL-03 and FL-08.

### Stage 9 — Only now, an environment kit

Meshes, textures, a brutalist kit, decals, an asset pipeline. Everything this
roadmap deliberately did not need in order to prove the direction.

This stage does not start without its own decision record covering asset budget,
LFS quota and source. `docs/CURRENT_STATUS.md` already records the LFS quota as
a known risk and states that a large art import is a new decision.

## Determinism binds the visuals

This is the constraint that makes RE:WIND's art direction different from the
same direction in any other game, and it is easy to violate by accident.

`docs/SYSTEMDOC.md` requires world clocks to read
`URewindLoopSubsystem::GetElapsedLoopTime` rather than engine time.
[`ADR-0002`](../adr/ADR-0002_world-determinism.md) fixes what may vary across
loops. FL-03 and FL-08 check that two idle loops match at the same `t`.

Therefore: **any visual effect that varies over the loop is a world clock, and
obeys the same rule.** A flickering neon sign, a fog animation, a pulsing
light, a loop-end desaturation, a glitch — each either reads elapsed loop time
or is constant. An effect driven by engine time or by frame delta will drift
between loops, and it will drift in exactly the register the player is being
asked to learn.

An effect that is genuinely decorative and carries no learnable fact may be
free-running, but that is a stated exemption per effect, not a default.

## Hard constraints

- **The non-goal gate.** Final art is a non-goal in `docs/PROJECT_BRIEF.md`.
  Activation requires changing that document by explicit decision first.
- **One owning document per rule.** Palette, contrast and neon meaning belong in
  one new design document. They may not be restated inside
  `chapter-1-authored.md` or anywhere else.
- **Reference frames are not committed.** The four supplied frames appear to be
  captures from a commercial game. `AGENTS.md` forbids committing third-party
  licensed assets whose terms have not been checked. The activating task
  describes the target in words and captures its own frames from its own build.
  Reference images stay outside the repository.
- **The blockout stays a blockout until Stage 0 passes.**
- **The debug overlay is not UI**, and no stage may quietly promote it.

## Dependencies

- REW-0003 complete and archived, with FL-01 to FL-16 evidence.
- An explicit decision changing the non-goal list in `docs/PROJECT_BRIEF.md`.
- ADR-0007, camera and perspective, before any level or animation work.
- ADR-0008, renderer configuration, before Stage 4 produces judgeable images.
- A later asset-budget decision before Stage 9, on top of
  [`ADR-0005`](../adr/ADR-0005_asset-storage.md).

## Suggested verification

- A written `LT` criterion for every property a build claims to have.
- Captures from a named build at a stated `t`, from fixed poses, rather than
  screenshots chosen because they came out well.
- Any loop-varying visual checked across two idle loops at the same `t`, in the
  same form as FL-03.
- Measured frame time against the Stage 3 budget, on a named machine.
- A first-time-player observation for P7, since legibility is the claim.

## What this proposal does not decide

It does not decide the camera, the renderer configuration, the palette, the
character design, the animation source, the asset budget, or whether any of this
happens at all. It does not change any accepted design document, it does not
touch REW-0003's frozen charter, and it grants no authority to the sandbox art
bible it cites.
