# Authored stairwell visual direction

Status: Accepted
Owns: the first-pass material, lighting, silhouette and reflection grammar of
the standalone authored stairwell presentation slice
Does not own: final environment art, reusable environment kits, asset-pipeline
policy, Chapter 1 puzzle rules, camera mechanics or the wider Handmade map
Source direction: owner request on 2026-08-24 and the tracked
[`stairwell-4c-landing.png`](../concept/stairwell-4c-landing.png) target

A rule that is not in this document is not a visual rule for this slice.

## Surface separation

The stairwell wall is two deliberately separate meshes and reads as two
materials:

- the upper field is dirty, rough plaster in a desaturated warm grey;
- the lower band is dirty painted plaster with a muted institutional green
  tint;
- the split remains geometrically crisp even when grime crosses the frame.

Stairs and landings are one dark, worn circulation family. They may suggest
concrete or stone, but must remain legible as the walkable route. Guardrails
and closed doors are darker than the wall fields and separate by silhouette
and roughness rather than bright color.

Procedural variation is a base breakup, not a hero texture. It stays fine and
low contrast. Large cracks, damp streaks and damage are sparse authored decals
or later dressing; they are not simulated by turning the base noise into
marble.

## Light and value

Light is localized around practical fixtures. Warm pools establish occupied
landings; restrained cool fill separates the vertical stack and keeps the next
flight readable. Direct highlights may identify a fixture or doorway, but
navigation surfaces may not disappear into crushed black between them.

Every practical casts shadows. Ambient fill may lift the route, but it may not
flatten the guardrails, door recesses or the alternating flights into one
value. Exposure is authored and stable; automatic exposure does not brighten
darkness differently because the camera crossed a threshold.

The player reads as a dark figure against a lighter field. The material is
near-black rather than unlit black so rim and local spill can describe the
pose. The silhouette must remain recognizable without becoming the brightest
object in the frame.

Floor response is rough and restrained. Reflections may hold a small practical
or color echo, but the stairwell is not wet, mirrored or rain-dressed in this
slice.

## Composition boundary

The owner-placed `StairwayCamera` at 35 mm is the starting composition. Runtime
regions may follow the player vertically and blend at the upper and entrance
thresholds, while their authored rotation remains fixed. Camera behavior is
owned by [`camera-and-movement.md`](camera-and-movement.md); this document owns
only what must remain visually readable inside those frames.

This treatment is deliberately made from project-owned procedural materials
and basic geometry. It demonstrates the grammar above. It is not evidence that
final textures, props, decals, fixtures, rain, neon or environment art exist.

## Testable statements

1. Upper and lower wall fields are separate meshes with distinct material
   instances.
2. A clean PIE frame shows a dark readable player silhouette, localized warm
   practical light, cool separation, cast shadows and legible treads.
3. Base material variation reads finer and lower contrast than a hero crack or
   damp decal would.
4. Floor response is rough enough that reflections remain secondary to the
   route and silhouette.
5. The slice can satisfy all four statements without a third-party asset
   dependency.
