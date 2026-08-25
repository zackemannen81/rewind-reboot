# Authored Building Slice — 2026-08-24

Status: Complete

## Build and method

- Branch: `codex/rew-0012-authored-4c-lift`
- Base commit: `d094aae`
- Build: Unreal Editor Win64 Development, UE 5.8, successful with the editor
  closed in 8.91 seconds after the final C++ change.
- Map: `/Game/Maps/FiveLoops_Stairwell_Blockout`
- Input: project-owned MCP PIE input through Unreal mappings. Every evidence
  run ended with `ReleaseAllKeys`; the final state reported no held keys before
  PIE stopped.
- Evidence source: saved-map inspection, named `LogRewind` transitions,
  possessed-player and active-camera state, and clean game-viewport captures.

## Saved authored map

The final save reopened at the expected map path with 52 actors and no dirty
package. Both `EditorStartupMap` and `GameDefaultMap` resolve to that map. The
standard PlayerStart is `(-900, 1650, 1296)`, yaw 180, and standard PIE
possessed `RewindCharacter_0` at Z `1298.15` with `RewindCameraRig_0` as the
sole view target. The procedural FiveLoops proof did not appear.

The measured blockout uses a 300 cm room height, a 700 by 1200 cm Apartment
4C, a 360 cm common-hall width and 1200 cm between floor 4 and entrance. The
critical gameplay actors reopened at these transforms:

| Actor | Location (cm) |
| --- | --- |
| Radio | `(-1160, 1370, 1355)` |
| Code lock / 4C door | `(-600, 790, 1320)` |
| Loose fuse | `(-1170, 1780, 1330)` |
| Building fuse socket | `(-1270, 1780, 1350)` |
| Ground fuse socket | `(-575, 1120, 130)` |
| Lift actor | `(-780, 500, 0)` |

The saved lift is a cutaway cage with a blocking floor and open hall-side
threshold. Three low-intensity blue fill lights keep the full shaft readable
without flattening the dark silhouette treatment.

## Camera coverage

The accepted entrance, stair and upper-threshold regions remain unchanged.
Four new half-open regions extend them without a reported gap, overlap,
first-person fallback or second view target:

| Region | Travel | Offset (cm) | Rotation | FOV |
| --- | --- | --- | --- | ---: |
| `Apartment4C_Region` | Y | `(330, 0, 70)` | `(-4, 180, 0)` | 55 |
| `TopHall_Region` | Y | `(-140, -500, 75)` | `(-13, 75, 0)` | 65 |
| `LiftShaft_Region` | Z | `(0, -500, -40)` | `(0, 90, 0)` | 60 |
| `GroundHall_Region` | Y | `(-140, -500, 120)` | `(-7, 75, 0)` | 65 |

The powered lift run reported `LiftShaft_Region` during travel,
`GroundHall_Region` after the lower handoff and `TopHall_Region` after the
upper handoff. Moving onward from the lower hall transferred to the accepted
`Stairwell_Entrance` region.

## Interaction and route evidence

- Radio interaction cycled from channel 1 to channels 2 and 3 and emitted its
  loop-clocked sequence output from the placed actor.
- Manual `1111` at t=7.33 s was rejected and left the player in 4C. Manual
  `7312` was accepted and the player crossed into `TopHall_Region`.
- The loose fuse was picked up at t=4.00 s and seated in the building socket at
  t=4.67 s. The lift changed to `POWERED`.
- With the building socket empty, a separate clean-baseline run reached the
  cage and interaction logged `Lift: refused, fuse not in the building socket`
  at t=218.34 s. The player was not carried.
- Powered descent began at t=19.00 s and reached entrance at t=25.00 s,
  measuring exactly 6.00 seconds. The capsule was handed to the ground-hall
  side at `(-540, 500, 98.15)` and movement continued.
- Powered ascent began at t=29.00 s and reached 4C at t=35.00 s, measuring
  exactly 6.00 seconds. The final upper handoff was
  `(-540, 500, 1298.15)` in `TopHall_Region`.

The saved ground socket was not directly seated during this authored-map run.
Its exclusivity is supported by the shared C++ fuse state, which permits only
one `Seated` destination and explicitly refuses an empty socket when the fuse
is in the other, plus the prior formal Loop B/Loop C evidence in
[`chapter-1-three-loops-2026-08-23.md`](chapter-1-three-loops-2026-08-23.md),
where the same one fuse was observed in each exclusive destination across
separate loops.

The inherited stair geometry was not replayed end-to-end after the surrounding
extension. REW-0011's accepted saved-map run proved floor 4 to entrance and
back on that unchanged geometry. This run additionally proved the new upper
hall-to-stair and ground-hall-to-`Stairwell_Entrance` handoffs. The named
`Rewind.Chapter1.VerticalRoutes` automation test also passed in the final
build. This is composite evidence, not a claim that a second full stair route
was recorded here.

## Visual evidence

Clean viewport captures showed the 4C and hall/lift compositions under the
delivered state: near-black player silhouette, separate dirty upper/lower wall
values, localized warm practicals, cool shaft separation, cast shadows,
stable manual exposure and a moderately reflective 4C floor. The material and
lighting remain a first-pass authored presentation treatment, not final art.

## Repeatable checks

All eight discoverable `Rewind.*` automation tests passed together, 8/8 with
zero failures in 0.077351 seconds:

- `Rewind.Camera.Region.VerticalTravel`
- `Rewind.Chapter1.RouteBudget`
- `Rewind.Chapter1.VerticalRoutes`
- `Rewind.Ids.MatchDesign`
- both loop-clock tests
- `Rewind.Project.DefaultAuthoredMap`
- `Rewind.Save.StoresSessionBuckets`

The new default-map test asserts both startup settings. The map and new
`MI_4CFloor` asset resolve through Git LFS. The new material depends only on
the tracked project-owned stairwell material family; no unapproved Fab or
marketplace asset was added.

## Not verified

No packaged build was made. The full inherited stair route and ground-socket
seating were supported by composite evidence as stated above rather than
replayed end-to-end in this record. This record does not claim final textures,
decals, prop dressing, audio, rain, neon city dressing, event-driven rewind or
any playable space beyond the bounded authored building slice.
