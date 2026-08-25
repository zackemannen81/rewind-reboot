# Renderer configuration and first look pass — 2026-08-25

Status: Complete

## Build and method

- Branch: `grok/rew-0019-renderer-and-look`
- Machine: hostname `MRWHITE81`, MEDION MD35219, Intel Core i7-11700F, NVIDIA
  GeForce RTX 3060 Ti (driver 32.0.15.8129, 8024 MB dedicated), 16 GB RAM,
  Windows 11 25H2
- Engine: Unreal Engine 5.8, `RewindEditor Win64 Development`
- Map: `/Game/Maps/FiveLoops_Stairwell_Blockout`
- Stated capture position: Apartment 4C start camera
  `(750, 1330, 1330)`, rotation `(0, 180, 0)`, horizontal FOV `37.497356`,
  region `Apartment4C_Region`. Player at `(0, 1580, 1298.15)`.
- Frame-time method: the same 1920×1080 windowed `-game` invocation before and
  after the renderer change:
  `-benchmark -seconds=12 -fps=0`. Both runs exited at engine tick 361. Wall
  clock is measured from `UWorld::Bringing up level for play` to
  `FEngineLoop::Tick.Benchmarking` RequestExit.

## Renderer settings

| Setting | Before | After |
| --- | ---: | ---: |
| `r.DynamicGlobalIlluminationMethod` | 0 | 1 (Lumen) |
| `r.ReflectionMethod` | 0 | 1 (Lumen) |
| `r.Shadow.Virtual.Enable` | 0 | 1 |
| `r.GenerateMeshDistanceFields` | False | True |
| `r.DefaultFeature.Bloom` | False | True |
| `r.DefaultFeature.AutoExposure` | False | False |
| `r.Substrate` | True | True |
| `r.RayTracing` | False | False |

Live editor `SearchCVars` after the change reported Bloom 1, mesh distance
fields 1, virtual shadow maps 1, auto-exposure 0. The after `-game` log set
`r.DynamicGlobalIlluminationMethod:1` and `r.ReflectionMethod:1`.

## Frame time

Same position class (the map's PlayerStart / 4C start), 1920×1080 windowed
`-game`, 361 ticks:

| Pass | Play wall clock | Implied frame time | Implied FPS |
| --- | ---: | ---: | ---: |
| Before (blockout renderer) | 3.084 s (`04:04:50.977` → `04:04:54.061`) | 8.54 ms | 117 |
| After (Lumen + VSM + MDF + bloom + shaped lights) | 4.705 s (`04:13:42.081` → `04:13:46.786`) | 13.03 ms | 77 |

The ADR-0010 budget is 16.67 ms GPU at 1920×1080 on this machine. The after
measurement is 13.03 ms, 3.64 ms inside the budget. This is a `-game`
windowed Development measurement, not a packaged shipping build, and not a
stat-unit GPU-only split.

## Captures

Images live in
[`renderer-look-2026-08-25/`](renderer-look-2026-08-25/).

| Image | What it is |
| --- | --- |
| [`before-4c-start.png`](renderer-look-2026-08-25/before-4c-start.png) | Clean PIE game-viewport PNG at the stated 4C start camera, blockout renderer |
| [`after-4c-start.png`](renderer-look-2026-08-25/after-4c-start.png) | The same camera after ADR-0010 and the light pass |
| [`after-stairwell-composition.png`](renderer-look-2026-08-25/after-stairwell-composition.png) | Editor viewport capture from `(1520, 0, 1150)`, yaw 180, after the change. Supporting, not the DoD frame: editor light gizmos and a wide lens are present |

## What the 4C after frame shows

Observed in [`after-4c-start.png`](renderer-look-2026-08-25/after-4c-start.png),
compared with the before frame at the same camera:

- **Indirect light.** Magenta from the 4C sign reaches the plaster around it.
  The before frame had a hot circular disc and almost no wall bleed.
- **4C sign lighting the wall behind it.** The sign is a project-owned
  emissive panel (`Cube3` at `(-250, 1190, 1420)`) plus `4C_SignLight` at
  `(-232, 1190, 1420)`, 24 cd, source radius 11 cm, magenta. The after frame
  shows the wall field beside and below the panel lit in magenta. A small
  `4C` letterform sits on the panel.
- **Readable value range.** The checker floor is visible from the left
  silhouette to the right doorway. It is dark, not pure black. The before
  frame crushed more of the left floor and wall into undifferentiable black.
- **Cast shadows.** The foreground column still throws a shadow on the floor.
- **Player silhouette.** The figure on the left remains a dark readable cutout
  against a lighter field.
- **Cool separation.** The tiled doorway on the right still carries a cool
  downlight distinct from the magenta practical.

## Against `stairwell-visual-direction.md` testable statements

These are the accepted testable statements. This capture is evidence for some
of them and not for others.

1. Upper and lower wall fields are separate meshes with distinct material
   instances. **Not re-verified here.** The 4C start camera does not show the
   stairwell split. Ownership of that statement remains the REW-0011 capture.
2. A clean PIE frame shows a dark readable player silhouette, localized warm
   practical light, cool separation, cast shadows and legible treads.
   **Supported for silhouette, localized practical (now magenta 4C plus the
   existing warm/cool 4C lights), cool doorway separation and cast shadows.
   Not supported for legible treads:** treads are not in the 4C start frame.
   The supporting stairwell editor capture shows warm landings and cool shaft
   fill; the flights between them remain very dark in that wide shot and are
   not claimed as tread-legible.
3. Base material variation reads finer and lower contrast than a hero crack.
   **Not judged from these frames.**
4. Floor response is rough enough that reflections remain secondary to the
   route and silhouette. **Supported in the 4C after frame:** the checker
   floor is readable and does not become a mirror. This is Lumen reflection
   plus the existing `MI_4CFloor` instance, not a wet-floor treatment.
5. 4C, its common hall and the lift remain recognizably part of the same
   building while interactions read as distinct landmarks. **Partially
   supported.** The 4C after frame is the same building language. Hall and
   lift were not recaptured in PIE. The stairwell editor capture shows the 4C
   magenta and the stair warm/cool as one volume.
6. The slice can satisfy all five statements without a third-party asset
   dependency. **Supported for this pass:** the added sign material is
   project-owned `/Game/Art/Materials/BuildingSlice/M_4CSignEmissive`.
   Untracked `/Game/Fab/` and `/Game/Art/Texture/` packages remain referenced
   by the saved map, remain unresolved in this clone, and were not added,
   removed or committed.

## Light pass, as authored

Existing practicals were kept and retuned for bounce (source radius 14–18 cm
on warm keys, cool fills at low candela with large source size). A stationary
skylight `Authored_CoolSkyFill` at intensity 0.22 provides restrained hemisphere
fill so navigation surfaces do not fall to zero. The directional light was
dropped to 0.25 and set not to cast shadows, so it does not flatten the
interior. Auto-exposure stays authored.

`Cube3` on the 4C door wall received the project-owned unlit emissive
material. That mesh already existed; this pass did not import a sign model.

## Repeatable checks

`RewindEditor Win64 Development` built editor-closed in 3.87 s, target up to
date (no C++ change). All nine discoverable `Rewind.*` automation tests passed
together headless (`-NullRHI`), `TEST COMPLETE. EXIT CODE: 0`:

- `Rewind.Camera.Region.PlayerVolumeClamp`
- `Rewind.Camera.Region.VerticalTravel`
- `Rewind.Chapter1.RouteBudget`
- `Rewind.Chapter1.VerticalRoutes`
- `Rewind.Ids.MatchDesign`
- `Rewind.Loop.Clock.IgnoresStopped`
- `Rewind.Loop.Clock.StepIndependent`
- `Rewind.Project.DefaultAuthoredMap`
- `Rewind.Save.StoresSessionBuckets`

`Rewind.Project.DefaultAuthoredMap` still asserts both startup maps.

## What was not verified

- Packaged / cooked build.
- GPU-only `stat unit` split (Game / Draw / GPU). The budget comparison is
  wall-clock frame time over 361 ticks.
- Frame-rate independence, outdoor maps, or the procedural FiveLoops proof.
- Camera FOV and placement, which the owner is judging separately.
- Final art, decals, rain, an environment kit, or third-party provenance.
- Re-running FL-01 to FL-18.
- A stairwell PIE frame at the 35 mm `StairwayCamera` composition.
