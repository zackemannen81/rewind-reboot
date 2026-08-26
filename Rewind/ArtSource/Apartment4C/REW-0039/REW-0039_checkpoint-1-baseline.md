# REW-0039 checkpoint 1 — Apartment 4C baseline

Date: 2026-08-26

Actor: `codex-4c-art`

Branch/base: `codex/rew-0039-4c-concept-art` at `90d3d62`

Mode: read-only map inspection and unsaved SceneCapture; no implementation
asset or map was saved.

## Inputs and identity

| Input | Dimensions | Bytes | SHA-256 |
| --- | ---: | ---: | --- |
| Operator target `C:\code\rewind-reboot\docs\concept\apartment-4c-interior-light-enhanced.png` | 1536 × 1024 | 2,692,286 | `A337E60C0AE9BEC4B3240C22EC3DC726EB4ADD1BB1C7292758A614CABC4C268A` |
| Operator baseline `C:\code\rewind-reboot\Rewind\Saved\Screenshots\WindowsEditor\RewindPIE.png` | 1638 × 1623 | 2,788,327 | `A9CD7F13B891FD908786EF59ABDFCFEB22B95CAF2801C1F1196C3BE3E307D585` |
| Clone exact-camera baseline `../../Screenshots/REW-0039_00_exact-camera_baseline.png` | 1600 × 900 | 1,173,227 | `A44FEA943C85F3EF9F1F24BADA8E52264346A9CFD6915BA4B7883B8B7620E192` |

The clone capture was made by
`Rewind/ArtSource/Scripts/REW-0039_capture_exact_camera.py` after a 45-second
shader warmup at `(1969.78, 1300, 1390)`, rotation `(0, 180, 0)`, horizontal
FOV `37.5`. Its Returner is an unsaved posed stand-in at the accepted
PlayerStart. Later runtime evidence must use standard PIE; this capture is the
matched art-comparison baseline.

The complete machine-readable actor, component, bounds, collision, light,
asset and material report is
`REW-0039_baseline_inventory.json`. The inventory loaded 103 map actors,
recorded 76 actors overlapping the 4C query box and enumerated 78 assets under
the task-relevant content roots. The script made no save call.

## Camera and gameplay preservation boundary

| Object | Baseline |
| --- | --- |
| `Apartment4C_Region` | centre `(-2.5, 1503, 1390)`, travel Y, offset `(1972.283, 0, 0)`, player extent `(242.5, 650, 210)`, FOV `37.5` |
| Exact start comparison | `(1969.78, 1300, 1390)`, `(0, 180, 0)`, FOV `37.5` |
| `Stairwell_PlayerStart` | `(50, 1300, 1296)`, yaw `-90` |
| `4C_CodeDoor_7312` | `(-255, 1023, 1310)` |
| `4C_Fuse` | `(-250, 1180, 1350)` |
| `4C_BuildingFuseSocket` | `(-246, 1195, 1344)` |
| `4C_Radio` | `(-225, 1340, 1260)` |

These gameplay and camera transforms are not art-placement candidates. Visual
meshes may be composed around them while the actors remain unchanged.

## Existing prop/bounds baseline

All placed Meshy components below currently use `BlockAll` query-and-physics
collision and the engine `WorldGridMaterial`. The tracked task-specific
`MI_REW_Prop_*` instances exist and point to matching project-owned base,
normal and roughness textures, but the prop meshes are not bound to them.

| Prop | World location | Bounds size, cm | Baseline disposition |
| --- | --- | --- | --- |
| Pendant lamp A | `(-70, 910, 1462)` | `51.5 × 51.5 × 118.0` | placed, WorldGrid, BlockAll |
| Kitchen table | `(-62, 928, 1200)` | `73.6 × 135.5 × 115.9` | placed; visually too tall for a dining table |
| Shelf | `(-220, 955.1, 1430)` | `59.4 × 189.7 × 89.6` | placed across the kitchen/door bay |
| Door | `(-250, 1048, 1200)` | `1.3 × 116.0 × 221.6` | placed over the preserved code-door area |
| Coat | `(-240, 1150, 1300)` | `18.8 × 61.8 × 189.9` | placed; long silhouette, BlockAll |
| 4C sign | `(-250, 1190, 1420)` | `6.0 × 38.0 × 30.2` | placed but WorldGrid rather than its emissive material |
| Radio mesh | `(-230, 1250, 1305)` | `36.3 × 60.8 × 34.8` | placed 90 cm in Y from the gameplay radio actor |
| Desk | `(-210, 1300, 1240)` | `0 × 0 × 0` | actor exists with no static mesh; no tracked desk asset |
| Pendant lamp B | `(-190, 1655, 1462)` | `51.5 × 51.5 × 118.0` | placed, WorldGrid, BlockAll |
| Low table/footstool | `(90, 1664.9, 1200)` | `190.1 × 180.8 × 33.7` | placed; footprint is too large for the target low table |
| Sofa | `(-110, 1665, 1200)` | `191.9 × 109.3 × 91.3` | placed and floor-snapped |
| Window railing/plant | `(-250, 1989.8, 1200)` | `52.3 × 190.3 × 88.9` | placed in the balcony bay |
| Chair | not placed | tracked asset | `SM_REW_Chair` and `MI_REW_Prop_chair` exist |
| Drapes | not placed | tracked asset | `SM_REW_Drapes` and `MI_REW_Prop_drapes` exist |
| Oven/cabinets | absent | no tracked asset | requires existing cube geometry/simple project-owned geometry |
| Rug | absent | no tracked asset | requires a thin simple project-owned surface |

The empty desk actor is an observed missing dependency, not permission to read
or modify the owner's canonical untracked prop-source directory. The inherited
Meshy provenance is recorded at `Rewind/ArtSource/Props/PROVENANCE.md`: project
owner's Meshy Pro output, remeshed on 2026-08-26. REW-0039 has made no Meshy
call and spent zero credits.

## Surface/material baseline

- Shell floor: `MI_REW_FloorTile_Interior`, with accepted base/normal/roughness
  family; bounds `(-280,833,1180)` to `(1150,2173,1200)`.
- Shell ceiling: `MI_REW_PlasterGray_Aged`; same apron extent as the floor,
  Z `1580..1600`.
- Shell back/end walls: `MI_REW_PlasterGreen_Aged`; the target is predominantly
  warm-grey plaster, so the all-green field is a visual mismatch.
- Three bay columns: `MI_REW_BrickDark_Aged`.
- Twenty-six overlapping decal actors were observed. REW-0030 documents twenty
  room placements; the broader query also catches adjacent decals. No decal is
  removed or used as a substitute for base-surface correction.
- Eleven `MI_REW_Prop_*` instances inherit `M_REW_Surface`, bind the expected
  project-owned `_BC`, `_N` and `_R` maps, preserve vector `TileSize`, and use
  roughness values `0.45..0.92`. All twelve prop meshes (including unplaced
  chair/drapes) still save `WorldGridMaterial` in their only material slot.
- `M_4CSignEmissive` exists under the owned BuildingSlice path, but the placed
  sign mesh is not bound to it.

## Light baseline

| Light | Position | Intensity | Radius | Colour |
| --- | --- | ---: | ---: | --- |
| `PointLight4` | `(-60,920,1320)` | 130 | 760 | warm `(255,232,203)` |
| `SpotLight` | `(-230,1060,1449)` | 30 | 521 | cool `(180,243,255)` |
| `4C_SignLight` | `(-262,1190,1420)` | 24 | 260 | magenta `(242,46,158)` |
| `PointLight` | `(-110,1190,1420)` | 16 | 320 | pale magenta |
| `PointLight2` | `(-120,1354,1300)` | 12 | 420 | warm |
| `4C_Radio_Cool` | `(100,1460,1350)` | 14 | 260 | blue `(46,92,255)` |
| `PointLight5` | `(-190,1655,1320)` | 130 | 760 | warm `(255,232,203)` |
| `4C_Rear_Warm` | `(-300,1950,1375)` | 60 | 330 | pink-warm |
| `SpotLight2` | `(-320,1990,1320)` | 46 | 1000 | magenta-pink |

The light count is adequate, but two 130-candela omni lights flatten and blow
their bays while the window has no cold city source. This is a hierarchy and
shaping defect, not a request for more global light.

## Exact-camera visual rubric baseline

1. **Window/city:** the balcony bay is a black opening with a railing/plant;
   there is no rainy city backdrop or readable cold window source.
2. **Sofa/table/rug:** sofa and oversized footstool are visible; no rug exists,
   and the grouping lacks the target's low domestic pool.
3. **Radio cabinet/chair/shelf:** the radio mesh is small and displaced from
   its gameplay actor, the desk actor has no mesh, the shelf is in the kitchen
   bay and the tracked chair is unplaced.
4. **Neon/technical wall:** gameplay fuse/socket objects exist, but the 4C sign
   mesh is not emissive and the wall lacks the target's conduit hierarchy.
5. **Door/keypad/coat:** the door and coat are recognizable; the gameplay lock
   remains reachable, but the local pool blows the door material instead of
   modelling it.
6. **Kitchen:** only a tall table/counter cluster reads. No oven or cabinet
   composition exists.
7. **Material readability:** shell detail is present but crushed; props render
   WorldGrid, so wood/metal/plaster distinctions do not read.
8. **Lighting hierarchy:** isolated warm, magenta and blue lights exist, but
   warm blowouts dominate and no cold window field balances them.
9. **Returner silhouette:** recognizable against the lit wall, though the
   checkpoint capture is an unsaved stand-in and not runtime animation
   evidence.
10. **Reflection control:** the operator baseline is dominated by mirror-wet
    floor and ceiling response. The matched SceneCapture still shows a bright
    checker apron and reflected ceiling; both remain stronger than the route.
11. **Placement/collision risks:** all placed Meshy props are BlockAll; the
    oversized footstool is the largest route risk, the kitchen table is
    implausibly tall, the desk is an empty actor, and the radio visual/gameplay
    positions disagree. Gameplay actor locations are preserved.

## Checkpoint conclusion

The existing tracked closure contains enough reusable silhouette assets to
begin. Missing desk/cabinet, oven/cabinets and rug can be built from simple
project-owned geometry; no Meshy call is justified at this checkpoint. The
first implementation step is to bind the already-existing detailed material
instances, correct the all-green/mirror response and capture the same exact
camera before composing new zones.
