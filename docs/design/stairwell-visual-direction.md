# Authored building-slice visual direction

Status: Accepted
Owns: the first-pass material, lighting, silhouette and reflection grammar of
the authored stairwell, Apartment 4C, fourth-floor common hall and lift slice
Does not own: final environment art, reusable environment kits, asset-pipeline
policy, Chapter 1 puzzle rules, camera mechanics or the wider Handmade map
Source direction: owner requests on 2026-08-24 and the tracked stairwell, 4C,
lift and interaction reference images under [`docs/concept/`](../concept/)

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

## 4C, common hall and lift extension

The extension uses the stairwell family rather than introducing a second art
style. The common hall retains the separated warm-grey upper plaster and muted
green lower band. Apartment 4C is slightly warmer and more domestic through
localized practicals and sparse blockout furniture, but its worn plaster,
rough floor and restrained values still belong to the same building.

The radio, code panel, loose fuse and both fuse sockets must read as distinct
interaction landmarks from their authored cameras. They may use a small local
value or color accent, but they do not become bright UI props. The lift shaft is
the darkest and most vertical part of the slice: near-black metal, warm cage
light and cool depth separation reveal travel without flattening the shaft.

The apartment floor may carry a slightly stronger soft reflection than the dry
stair treads, enough to catch one practical or cool window echo. It is not a
wet floor. The lift cage and rails separate by specular response and silhouette,
not by bright albedo.

## Composition boundary

The owner-placed `StairwayCamera` at 35 mm is the starting composition. Runtime
regions may follow the player vertically and blend at the upper and entrance
thresholds, while their authored rotation remains fixed. Camera behavior is
owned by [`camera-and-movement.md`](camera-and-movement.md); this document owns
only what must remain visually readable inside those frames.

This treatment is deliberately made from project-owned procedural materials
and basic geometry. It demonstrates the grammar above. It is not evidence that
final textures, authored prop models, decals, rain, neon or finished environment
art exist.

## Testable statements

1. Upper and lower wall fields are separate meshes with distinct material
   instances.
2. A clean PIE frame shows a dark readable player silhouette, localized warm
   practical light, cool separation, cast shadows and legible treads.
3. Base material variation reads finer and lower contrast than a hero crack or
   damp decal would.
4. Floor response is rough enough that reflections remain secondary to the
   route and silhouette.
5. 4C, its common hall and the lift remain recognizably part of the same
   building while interactions read as distinct landmarks.
6. The slice can satisfy all five statements without a third-party asset
   dependency.
