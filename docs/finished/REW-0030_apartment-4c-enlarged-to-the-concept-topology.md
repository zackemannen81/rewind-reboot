# Current Task

Task ID: REW-0030
Parent Task: None
Status: Complete
Owner: Claude
Created: 2026-08-26
Last updated: 2026-08-26
Charter frozen at: 2026-08-26

## Read First

- `AGENTS.md`
- `docs/TASK_WORKFLOW.md`
- `docs/PROJECT_BRIEF.md`
- `docs/CURRENT_STATUS.md`
- `docs/design/chapter-1-authored.md`
- `docs/design/camera-and-movement.md`
- `docs/design/stairwell-visual-direction.md`
- `docs/adr/ADR-0010_renderer-configuration.md`
- `docs/adr/ADR-0011_asset-source-and-provenance.md`
- `docs/concept/README.md`
- `docs/finished/REW-0027_import-the-environment-texture-kit-and-build-surface-instances.md`
- `docs/finished/REW-0028_grime-decal-instances-from-the-environment-kit-atlases.md`

## Task Summary

Apartment 4C did not match the concept target: wrong proportions between door,
balcony and end walls, and a camera that framed a fraction of the room. The
owner asked for the topology to match the concept — four bays, and the room
grown to suit.

Investigation found the more basic problem. **4C had no shell at all.** No
floor, no walls, no ceiling. `inspect_4c_shell.py` found that every cube near
the room was a small fixture — a counter, a fusebox plate, a balcony lintel,
four thin posts — and the only enclosure was `Cube8`, the 3291×3895×5000
building envelope the whole level sits inside. The props stood against a wall
line that existed only in the placement data.

So this task built the room rather than resized it.

## Task Charter

Build Apartment 4C's shell to the concept topology, move its existing contents
to suit, reframe its camera, and light it.

Scale from the owner's stated metric: the player is 1.8 m.

| | |
|---|---|
| Interior | 500 deep × 1300 wide × 380 tall |
| Bays | four at 325, structural columns on the interior boundaries |
| Door | held at Y 1048 in bay 1 — a fixed point, not to be moved |
| Balcony | bay 4, opening centred at Y 1990 |
| Camera | framing rule `width / (2·tan(fov/2))` on the existing 37.5° lens |

Out of scope: the courtyard and street blockout, audio, the message system,
and the loop-echo VFX. Those are their own tasks.

## Constraints

- The owner's authored placements are the irreplaceable part. Assets return at
  the **same package paths** or every placement is orphaned.
- No `git add -A` or `git add .` in this repository, ever.
- `docs/baseline/` and `docs/concepts_sandbox/legacy-rewind/` are never edited.
- The camera volume boundary with `TopHall_Region` at X −245 must stay exact.
  A gap there is what stranded the controller in REW-0015.

## Evidence

`capture_region.py` was written for this task and renders a region's authored
shot to PNG headlessly, through the region's own `CameraOffset` and
`FieldOfView`. Every claim below is from an image, not from a log line.

| Claim | Evidence |
|---|---|
| 4C had no shell | `inspect_4c_shell.py`: 13 static meshes overlap the volume, largest is `Cube8` at 3291×3895×5000; none is a wall |
| Room built to spec | `inspect_4c_shell.py` after: floor 1180→1200, ceiling 1580→1600, walls 1200→1580, columns at Y 1178/1503/1828 |
| Door held | door mesh bounds Y 990→1106 fits the wall gap Y 988→1108 exactly |
| Props were floating | snap deltas of +20, +40, +45, +51, +110 cm |
| Room was black in play | `4c_after.png`, rendered at the game's own −0.7 EV |
| Room reads now | `4c_lit4.png`, `4c_grime.png`, same exposure |
| Prop footprint | 580 MB → 3.1 MB, `du -sh Rewind/Content/Props` |

## Outcome

Complete. Four commits on `claude/rew-0030-4c-props-and-room` (merged as #32)
and one on `claude/rew-0030-decals`.

The lighting turned out to be the real look-and-feel problem, not the geometry.
The level runs **manual** exposure at −0.7 EV with no auto-exposure, so light
intensities are absolute — and the two pendant lamps meant to light the
apartment were authored at 1.4 and 2.3 cd while their neighbours sat at 110 and
160. The room rendered black because it was black.

Follow-on work is charted separately as REW-0031: the environment kit ships
base colour only, with no normal or roughness maps anywhere, so every surface
in the game is perfectly flat.
