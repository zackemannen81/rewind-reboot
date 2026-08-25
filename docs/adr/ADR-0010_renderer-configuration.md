# ADR-0010 — Renderer configuration for the authored look

Status: Accepted
Date: 2026-08-25
Deciders: grok-look, under the frozen REW-0019 charter
Supersedes: nothing
Superseded by: nothing

## Context

The authored building slice has an accepted first-pass visual grammar in
[`stairwell-visual-direction.md`](../design/stairwell-visual-direction.md):
localized practical light, restrained cool fill, cast shadows, a readable
value range in which navigation surfaces never fall to pure black, a rough
restrained floor response, and emissive sources that read as light rather than
stickers. Auto-exposure is authored and stable.

The project cannot render that grammar today. `Rewind/Config/DefaultEngine.ini`
is still the blockout configuration:

| Setting | Value | Consequence |
| --- | --- | --- |
| `r.DynamicGlobalIlluminationMethod` | `0` | No Lumen GI. Nothing bounces. Emissive surfaces do not light their surroundings. |
| `r.ReflectionMethod` | `0` | No reflections. The floor response the visual direction asks for cannot render. |
| `r.Shadow.Virtual.Enable` | `0` | Virtual shadow maps off. |
| `r.GenerateMeshDistanceFields` | `False` | No mesh distance fields. Software Lumen cannot trace. |
| `r.DefaultFeature.Bloom` | `False` | Emissive sources do not bloom. The 4C sign reads as a sticker. |
| `r.DefaultFeature.AutoExposure` | `False` | Correct. Exposure is authored. This stays. |
| `r.Substrate` | `True` | Already on. This stays. |
| `r.RayTracing` | `False` | Hardware RT off. |

Those settings were the right call for the Five Loops blockout. Reversing them
costs shader compilation, frame time and a hardware floor. That is why this is
a recorded decision and not a config edit made in passing.

[`docs/backlog/art-direction.md`](../backlog/art-direction.md) already named
this as Stage 3 and numbered it ADR-0008. ADR-0008 is taken by
[`ADR-0008_what-an-anchor-is-worth.md`](ADR-0008_what-an-anchor-is-worth.md).
This record is the next free number.

## Decision

Enable Lumen software tracing, Lumen reflections, virtual shadow maps, mesh
distance fields and default bloom. Keep authored exposure. Keep Substrate.
Keep hardware ray tracing off.

Concretely, in `Rewind/Config/DefaultEngine.ini`:

| Setting | Value |
| --- | --- |
| `r.DynamicGlobalIlluminationMethod` | `1` (Lumen) |
| `r.ReflectionMethod` | `1` (Lumen reflections) |
| `r.Shadow.Virtual.Enable` | `1` |
| `r.GenerateMeshDistanceFields` | `True` |
| `r.DefaultFeature.Bloom` | `True` |
| `r.DefaultFeature.AutoExposure` | `False` |
| `r.Substrate` | `True` |
| `r.RayTracing` | `False` |

**Frame-time budget.** 16.67 ms GPU (60 Hz) at 1920×1080 in PIE, measured at
the Apartment 4C start camera after shader warmup, on the named development
machine. The budget is a target for this authored interior slice, not a
packaged-build or outdoor-map claim.

**Hardware floor.** Windows desktop, DirectX 12, Shader Model 6, a discrete
GPU that can run Lumen software tracing via mesh distance fields. Hardware
ray-tracing cores are not required. SM5, mobile, and integrated-only targets
are outside this configuration.

**Named development machine used for the budget.** Hostname `MRWHITE81`,
MEDION MD35219, Intel Core i7-11700F, NVIDIA GeForce RTX 3060 Ti (driver
32.0.15.8129), 16 GB RAM.

Measured frame times at the stated position are recorded in the REW-0019
playtest, not restated here as if they were the decision.

## Alternatives considered

**Keep the blockout configuration (GI 0, reflections 0, bloom off, no distance
fields).** Rejected. The accepted visual grammar cannot be judged: unlit
surfaces stay at zero, the floor has no response, and emissive signs do not
light the wall behind them. The owner compared the current build to target
frames and the difference is this configuration, not missing final art.

**Enable bloom only, leave GI and reflections off.** Rejected. Bloom makes an
emissive surface glow on screen. It does not bounce light onto neighbouring
surfaces and it does not create the restrained floor response. The 4C sign
would still float.

**Screen-space reflections (`r.ReflectionMethod=2`) without Lumen GI.**
Rejected. The camera is authored and often near side-on. Large areas of the
floor and the wall behind a sign have no screen history. SSR would make the
floor response camera-accidental, which is the opposite of an authored look.

**Hardware ray-traced Lumen (`r.RayTracing=True`).** Rejected for this phase.
It raises the hardware floor to RT cores, lengthens shader compile, and is
not required to demonstrate the first-pass grammar. Software Lumen with mesh
distance fields is enough for an interior slice of this size. Hardware RT
remains available as a later decision if outdoor scale or contact quality
demands it.

**Baked static lighting.** Rejected. The lift moves, the loop resets world
state, Substrate is already on, and a bake pipeline is an asset process this
phase does not have. `r.AllowStaticLighting` stays True as an engine default;
this record does not adopt baking.

**Defer until final art.** Rejected. Final art is a non-goal. The current
failure is that the existing project-owned materials and practicals cannot
show the grammar they were authored for. Enabling the renderer is the
cheapest way to make that grammar judgeable.

## Consequences

**Accepted costs.**

- First editor launch after this change compiles a new shader permutation
  set. That is expected and can take many minutes.
- Frame time rises. The budget is 16.67 ms GPU at 1080p on the named
  machine; exceeding it is recorded as evidence, not silently ignored.
- Mesh distance fields increase cook time and memory. The authored slice is
  small enough that this is acceptable; a later wide Handmade map may need
  to revisit the floor.
- Virtual shadow maps and Lumen set a DX12 SM6 desktop floor. The project
  already targeted that class of hardware in
  `DefaultEngine.ini` (`DefaultGraphicsRHI_DX12`, SM6). This record makes
  the floor explicit.
- Light shaping has to be redone against bounce and bloom. Intensities that
  were readable as unlit blockout practicals will blow out once they bounce.

**Accepted benefits.**

- Indirect light exists, so a practical can illuminate a landing instead of
  a disc on the floor.
- Reflections exist, so the 4C floor instance can hold a small practical or
  colour echo, which the visual direction already requires.
- Bloom exists, so an emissive 4C sign can read as a light source.
- Emissive surfaces can contribute to Lumen, so a sign can bleed onto its
  wall when it also has a real light.

**What this does not decide.**

- Final environment art, an environment kit, decals, rain, or third-party
  asset provenance. Those remain non-goals or ADR-0005 questions.
- Camera FOV and camera placement.
- Character shading, Substrate material authoring, or the flat-fill figure
  proposed in the art-direction backlog.
- Hardware ray tracing, Lumen quality cvars beyond the project defaults, or
  a packaged-build scalability ladder.
- Any visual effect that varies over the loop. Those remain world clocks and
  still read elapsed loop time, per the determinism rule.

This record does not grant authority to
[`docs/backlog/art-direction.md`](../backlog/art-direction.md). That file
remains a proposal. The rules this configuration is meant to make judgeable
are the ones already accepted in
[`stairwell-visual-direction.md`](../design/stairwell-visual-direction.md).
