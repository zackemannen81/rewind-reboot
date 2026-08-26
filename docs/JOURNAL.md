# Journal

Newest first. Append only: entries are never edited or reflowed, because other
records cite them and because their value is that they record what was believed
at the time.

## 2026-08-26 — REW-0036 activated, Chapter 1 audio recovery supervisor

- Date: 2026-08-26
- Author: Codex (process supervisor)
- Task: REW-0036
- Branch: `codex/rew-0036-audio-recovery-supervisor`
- Trigger: the owner reported that nothing was heard after tuning the radio to
  channel 3 and asked for a new master/operator wave through the Docs-First
  multi-agent orchestrator add-on.
- Observed diagnosis: standard PIE loaded `RewindCharacter_0` at approximately
  `(50,1300,1298)`, `RewindRadio_0` at `(-225,1340,1260)` and the authored
  camera near `(1970,1300,1390)`. The player was inside `RadioRange = 320`, but
  the camera/default audio listener was about 22 m from the radio. The loaded
  radio exposed `Root` and `Mesh` only, not the `RadioBed` declared by merged
  source. The canonical `UnrealEditor-Rewind.dll` timestamp was 01:05 local;
  REW-0035 source was written at 05:07. Unreal selected `LG TV (NVIDIA High
  Definition Audio)` as its WASAPI render device. PIE keys were released and
  PIE was stopped cleanly.
- Control-plane setup: REW-0036 and child REW-0037 were claimed on `main` and
  pushed as `c4c18af`. The frozen wave plan gives REW-0037 one physical clone,
  one branch, one worker and exclusive camera/radio/test ownership. The
  operator keeps the canonical clone, task-ID register, editor, merge authority
  and combined verification.
- Decision: spatial gameplay audio uses the possessed player's position and
  the authored camera's orientation. The camera remains the view target;
  `RadioRange`, channel/timing rules and audio assets do not change.
- Process plane: the already-built stdio server at
  `C:\code\docs-first-multiagent-orchestrator-addon` is registered in the
  project Codex config. This running session cannot hot-load its five tools, so
  worker launch waits for a fresh repository-root session and a harmless MCP
  smoke task. Repository state, not this process note, is the recovery
  authority.
- Not yet verified: no REW-0037 worker has launched; no fresh canonical build,
  automation suite, corrected PIE run or owner-heard output exists yet.
- Signature: Codex (process supervisor)

## 2026-08-26 — REW-0035, Wire Chapter 1 audio into gameplay

- Date: 2026-08-26
- Author: codex-audiowire
- Task: REW-0035
- Branch: `codex/rew-0035-audio-wiring`
- Change: wired all fifteen REW-0033 SoundWaves. Existing radio digit events
  retain their screen messages and now play their matched `7`, `3`, `1`, `2`
  one-shots at the radio. An attached `UAudioComponent` switches between
  station/static beds; `Channel` is the only radio state, with `0` off and
  baseline restoring channel `1` static. On/off/tune are dry 2D clicks.
  `ARewindCharacter` plays the six interior footsteps every 80 cm travelled,
  selecting uniformly from the five indices other than the immediately prior
  one.
- Attenuation: created `/Game/Audio/Chapter1/A_REW_Radio_Attenuation` by
  commandlet. It is spherical, linear, has zero inner extent and a 320.0 cm
  falloff. `RadioRange = 320.0`, so station/static beds and digit playback end
  at the same distance as the credit boundary; audibility cannot extend beyond
  credit. No map was opened or changed.
- Determinism: radio phase and digit selection remain clock-derived from the
  existing elapsed-loop schedule. No loop count, timer accumulator or
  persisted radio audio state was added. `RestoreFromBaseline()` resets channel,
  listening, fragment, sequence, and bed selection. Footstep randomness is
  cosmetic only.
- Verification: with Unreal Editor closed, `Build.bat RewindEditor Win64
  Development` succeeded. The requested `UnrealEditor-Cmd` full-suite command
  completed with `TEST COMPLETE. EXIT CODE: 0`; 24 tests completed successfully,
  including new `Rewind.Radio.Audio.DigitSelection` and
  `Rewind.Radio.Audio.ResetAtRewind`. The attenuation commandlet exited `0`
  and read back `320.0` cm. `git diff --check` was clean.
- Not run: interactive PIE/audio playback. The code compiles and its schedule
  and reset behavior are unit-tested; this actor did not hear it in play.
- Signature: codex-audiowire

## 2026-08-26 — REW-0034, Chapter 2 Conduit design authority

- Date: 2026-08-26
- Author: codex-gdd
- Task: REW-0034
- Branch: `codex/rew-0034-chapter2-gdd`
- Pull request: #37 against `main`; not merged by this actor
- Change: accepted `docs/design/chapter-2-authored.md` and C2-01 through C2-25
  in `docs/acceptance/chapter-2-conduit-test.md`. Conduit is five connected
  spaces — platforms, East tunnel, pressure conduit, relay/inspection aisle
  and signal control — with nine discrete challenges and eight learning-run
  shapes. Collection indexes list both documents.
- ADR-0002 resolution: route 1. The Conductor has a fixed 48-second patrol and
  a fixed 12-second response selected by a detection made in the current loop.
  Detection time and response are LoopWorld. It reads no loop count, saved
  route, earlier-loop alert or death location, so no ADR-0013 is proposed.
- False information: railway displays are truthful for railway operations and
  misleading to a pedestrian. `LINE CLEAR` coincides with live traction. The
  public board says scheduled East until the phase-144 interlock decision;
  trolley occupation then diverts alternating trains West. The correction
  leaves 8 seconds to door opening while the crossover takes 10, so the player
  must predict rather than react.
- Timing: four independent 72-, 60-, 48- and 180-second schedules have a
  720-second joint horizon. The 12-second trolley isolation window contains a
  nine-second tunnel crossing. The learned route causes East detection at
  `t = 64`, uses the door during `[68,72)`, commits signal inputs at
  `t = 72,74,76,78`, reaches West by `t = 85` and boards during `[152,160)`.
  Approach-lock arithmetic gives West at `t = 144/504` and East at
  `t = 324/684`.
- State boundary: trolley, purge, relay, Conductor, signal, authorization and
  train state are LoopWorld; PlayerBody returns to Intake. The existing
  `radio_code_7312` knowledge and `courtyard_gate_open` Anchor persist, but
  Conduit creates no fact, Anchor, Insight or second store. Invalid signal
  commit and a missed authorized departure each run an exact two-second causal
  prelude; deaths are immediate; there is no global timeout.
- Phase boundary: `PROJECT_BRIEF.md` now records the owner's documentation-only
  exception. Accepted Chapter 2 design and acceptance do not authorize code,
  maps, assets or implementation planning. Those remain a later owner decision
  and bounded task.
- Verification: explicit PowerShell assertions passed for the LCM, distances,
  windows, response phases, signal timings, approach diversions and platform
  lead; counts are 9 challenges, 8 learning loops and 25 unique acceptance
  ids. Changed-document relative links resolve, fences close, design ownership
  was reviewed manually, `git diff --check origin/main...HEAD` was clean and
  the path audit found no `Rewind/`, baseline, imported legacy,
  `CURRENT_STATUS.md`, `SYSTEMDOC.md` or `FILESTRUCTURE.md` change.
- Not run: Unreal build, editor, PIE, automation tests and packaged build. The
  task is documentation-only and explicitly forbids editor work; no build
  claim is made.
- Open owner decisions: Conductor fiction, campaign-completion persistence and
  the implementation phase gate. Each carries a recommendation in the design.
- Signature: codex-gdd

## 2026-08-26 — REW-0033, Chapter 1 audio: radio voice, station static and footsteps

- Date: 2026-08-26
- Author: codex-audio
- Task: REW-0033
- Branch: `codex/rew-0033-chapter1-audio`
- Change: generated and imported 15 project-owned mono 44.1 kHz, 16-bit PCM
  SoundWaves under `/Game/Audio/Chapter1/`: four separately schedulable Zira
  radio digits for `7312`, two 10-second radio beds, three dry mechanical radio
  interactions and six varied hard-interior footsteps. No C++ or map changed.
  `Rewind/ArtSource/Scripts/generate_chapter1_audio.py` is seed-fixed at 7312;
  it uses only offline Windows SAPI plus numpy/scipy and includes the old-radio
  band-pass, AM wobble, flanger, small Schroeder reverb, filtered hiss/crackle
  and final -3 dBFS headroom chain. The separate importer saves only the two
  `_Loop` SoundWaves with their `looping` property true.
- Provenance: `Rewind/ArtSource/Audio/README.md` declares local SAPI Zira and
  deterministic synthesis; no downloaded or third-party sound source entered
  the repository, satisfying ADR-0011.
- WAV evidence: the committed `generation_report.json` read every written WAV
  back. All 15 report mono/44.1 kHz/16-bit PCM and peak `-3.000097 dBFS`; their
  individual durations and RMS readings are retained in the archived task.
  The station/static loops are 10.000000 s with wrap sample delta `0` PCM
  units. Above-4-kHz FFT-power fractions fell from `0.2025489484` to
  `0.0000690231` for station and from `0.8189756888` to `0.0017209403` for
  static; individual speech measurements are in the archive. Two generator
  runs had byte-identical SHA-256 results across all WAVs (2,507,260 bytes
  total).
- Unreal evidence: headless `UnrealEditor-Cmd` import at
  `2026.08.26-02.15.07` through `02.15.08` logged every import then
  `VERIFY PASS: 15 SoundWaves exist; only ['A_REW_Radio_Static_Loop',
  'A_REW_Radio_Station_Loop'] loop` in `Rewind/Saved/Logs/Rewind.log`.
  Saved-asset duration read-back matched each WAV, and every non-loop asset
  reported `looping=False`.
- Verification limits: this task intentionally performed no runtime or PIE
  playback because radio trigger wiring and footstep animation notifies are
  outside its frozen scope. The commandlet did not open or edit a map.
## 2026-08-26 — REW-0032, surface detail maps without losing 4C decals

- Date: 2026-08-26
- Author: codex-surfaces2
- Task: REW-0032
- Branch: `codex/rew-0032-surface-detail-redo`
- Change: retained REW-0031's sixteen derived source maps unchanged, reimported
  them as non-sRGB wrapped `TC_Normalmap`/`TC_Masks` textures, and bound every
  one explicitly to its matching kit instance. The master keeps REW-0027's
  `BaseColorTex` TextureObject → `WorldAlignedTexture` graph and its **vector**
  `TileSize`; REW-0030 prop instances therefore remain compatible. Roughness
  uses the same world-aligned projection. The normal uses a
  `SAMPLERTYPE_NORMAL` tangent-space sample: both tested world-aligned normal
  outputs made the decals vanish.
- Bisect evidence: with a fresh commandlet process after explicit
  `recompile_material`, old master + new instance detail bindings rendered the
  same decal-bearing wall as old master + old instances. The earlier contrary
  result was stale material/shader state, not an instance texture override.
  Extending the master isolated the actual incompatibility to world-aligned
  normal output. The tangent-space normal route keeps the decals.
- Render evidence: `Rewind/ArtSource/Screenshots/REW-0032_Apartment4C_before_worldaligned.png`
  and `REW-0032_Apartment4C_after_detailmaps.png` are 1600 × 900 captures of
  `Apartment4C_Region` at manual −0.7 EV. The paired 2× crops
  `REW-0032_Apartment4C_before_decal_crop_2x.png` and
  `REW-0032_Apartment4C_after_decal_crop_2x.png` cover the identical wall
  region: cracks and peeling-plaster decals remain plainly visible in the
  detail-enabled result. The neon-lit `MI_REW_BrickDark_Aged` bay column remains
  purple/dark brick rather than turning into REW-0031's white strip.
- Verification: `verify_surface_bindings.py` read back the vector TileSize
  contract, tangent normal sampler, all sixteen texture settings and all eight
  expected instance values. The material-update and verification commandlet
  logs contain no `LogMaterial: Error`; `git diff --check` is clean and no map
  path changed. No checks were skipped.
- Signature: codex-surfaces2

## 2026-08-26 — REW-0031 reverted: the surface rebuild eats every decal

Reverted from main. The normal and roughness maps do work -- wall relief and
tile structure are plainly visible with them -- but the rebuilt `M_REW_Surface`
takes two things with it that matter more than flatness does:

- **Every decal in the room stops rendering.** Not faint: absent. A magnified,
  exposure-lifted crop of the same wall shows crack decals clearly with the old
  materials and a completely clean wall with the new ones.
- **`MI_REW_BrickDark_Aged` blows out.** The bay column goes from dark brick to
  a white strip catching the neon.

What it is not, each ruled out by test rather than by argument:

| Suspect | Test | Result |
|---|---|---|
| Decal actors lost | count in map | 20 present, all with materials, all visible |
| Master material settings | full property dump, old vs new | identical |
| Instance settings, incl. base property overrides | full property dump | identical but for the texture bindings |
| Roughness scalar multiplying the roughness texture | set all eight to 1.0, capture | no change |
| DBuffer decal path | `r.DBuffer=False` at load | no change |
| Decal opacity | read back | 0.72, not zero |

Bisected as far as: old master + old instances renders decals; old master + new
instances does not; new master + new instances does not. The instances differ
from the old ones *only* in their texture parameter values, which should not be
able to do this. That contradiction is the thread REW-0032 has to pull.

Reverting rather than keeping it is a judgement about which flaw costs more.
Flat surfaces are a texture problem the room survives. Losing the grime removes
the decay the apartment is supposed to carry, and a blown-out white column in
the middle of the frame is worse than a flat brick one.

The branch and PR #34 are intact, the derived sources are regenerable, and the
work itself verified clean on its own terms. Nothing is lost by reverting except
the time to redo the master.

## 2026-08-26 — REW-0031 resolved: the baseline was broken, not the normal maps

codex-normals delivered REW-0031 correctly and then blocked itself on its own
evidence, reporting that its before and after captures decoded to identical
pixels and declining to archive, stage, commit or open a PR. That was good
discipline applied to a bad measurement.

The delivered work verifies clean. All eight `MI_REW_*` instances reference
their own `_N` and `_R` maps; every `_N` is `TC_Normalmap` with sRGB off and
every `_R` is `TC_Masks` with sRGB off; the master wires `MP_NORMAL` from the
normal sampler's RGB and `MP_ROUGHNESS` from the roughness sampler. Nothing
about the binding chain was wrong.

The baseline was. To capture a "flat" reference the agent replaced all eight
bindings with `DefaultNormal` and a white roughness — and hit the failure this
project documented earlier the same day: **setting a property from Python does
not dirty the owning package**, so the flattening never persisted and both
captures rendered the real maps. Its two images were the same picture twice.

Settled with an A/B that could not collapse that way: `4c_lit5.png`, captured
from the map state this branch forked from, against a fresh capture on the
branch. Those are genuinely different content states — one has no normal maps
in existence at all. Wall relief and tile structure are plainly visible in the
second and absent from the first.

Two consequences worth recording:

- `TileSize` changed from a **vector** parameter to a **scalar** in the rebuilt
  master. Any instance that overrode it as a vector silently lost the override
  and now inherits the master default. The `MI_REW_Prop_*` instances written by
  REW-0030 are exactly that case and need re-running.
- Surfaces read warmer and flatter in colour than before. That is the roughness
  maps doing their job: more diffuse surfaces reflect more of the lamp colour
  and less of their own. It is correct, not a regression, but it costs some of
  the colour separation the room had and is worth a tuning pass.

The lesson generalises past this task. An agent that measures its own work with
a broken instrument will block on correct work, which is a much better failure
than shipping broken work with a confident log line — but the operator still
has to check which of the two happened.

## 2026-08-26 — REW-0031, surface normal and roughness maps (in progress)

- Date: 2026-08-26
- Author: codex-normals
- Task: REW-0031
- Branch: `codex/rew-0031-surface-normals`
- Change: generated eight 1254 × 1254 Sobel normal PNGs and eight
  material-tuned grayscale roughness PNGs from the tracked project-owned
  `EnvironmentTextureKit-v1` base-colour JPEGs. The sources live under
  `Derived_REW-0031/`; `generate_surface_detail_maps.py` and
  `import_surface_detail_maps.py` reproduce the source generation, import and
  binding without opening or saving a map. Imported `T_REW_Surface_*_N`
  textures use `TC_Normalmap`/non-sRGB; `_R` textures use
  `TC_Masks`/non-sRGB, are wrapped, and cap at 2048.
- Material/binding evidence: the headless importer rebuilt `M_REW_Surface`
  with `BaseColorTex`, `NormalTex` and `RoughnessTex` sharing its world-space
  `TileSize` coordinate path, and multiplies the roughness texture by the
  retained `Roughness` scalar. Saved-asset read-back logged master defaults
  for all three texture parameters, the required settings on every new texture
  and explicit, surface-specific `NormalTex`/`RoughnessTex` values on all
  eight `MI_REW_*` instances. No `LogMaterial: Error` occurred in that run.
- Render evidence and blocker: `REW-0031_Apartment4C_before.png` and
  `REW-0031_Apartment4C_after.png` were captured at the region camera with
  manual -0.7 EV and copied to `Rewind/ArtSource/Screenshots/`. A read-only
  map inspection confirms the 4C shell components use the eight updated
  surface instances. For the baseline, all eight bindings were persistently
  replaced with `/Engine/EngineMaterials/DefaultNormal` and white roughness,
  captured from a fresh commandlet, then restored by the importer before the
  after capture. The decoded before/after pixels are nevertheless identical.
  The required visible-difference gate is therefore not met; this task is not
  archived, staged, committed or submitted for PR.
- Verification: final importer pass ended `VERIFY PASS: 16 linear maps and 8
  explicit per-instance parameter bindings read back` and `DONE`; `git diff
  --check` is clean. An earlier master-expression reflection check used a
  protected Unreal property and failed; it was removed, then the final clean
  importer pass above was run. No map was edited.
- Handoff: diagnose the final-colour scene-capture/material render path that
  makes persisted flat and derived bindings render identically, then replace
  the non-demonstrative screenshot pair before completing the REW-0031
  charter.
- Signature: codex-normals
## 2026-08-26 — REW-0030, Apartment 4C had no room in it

The brief was to grow 4C to the concept topology. The room turned out not to
exist. `inspect_4c_shell.py` found no floor, no walls and no ceiling anywhere
near it: every cube in range was a small fixture — a counter, a fusebox plate,
a balcony lintel, four thin posts — and the only enclosure was `Cube8`, the
3291×3895×5000 building envelope the whole level sits inside. The props stood
against a wall line that existed only in their placement data.

That reframed the task from resizing to building. The shell is now 500 deep ×
1300 wide × 380 tall, four bays of 325 with structural columns on the interior
boundaries, the door held at Y 1048 and the balcony opening moved to bay 4.
Scale came from the owner's own metric, a 1.8 m player.

The camera moved from 687 back to 1972 on the same 37.5° lens. Distance now
follows the rule the stairwell regions already used —
`width / (2·tan(fov/2))` plus about 3% margin — which means the earlier
"too zoomed in" complaint was the **distance**, not the lens. Floor and ceiling
run out to X 1150 so their near edges fall outside the frustum; without that, a
1300×380 room in a 16:9 frame leaves roughly a fifth of the shot empty above
and another fifth below.

### The lighting was the actual problem

The geometry was never why the room looked wrong. The level runs **manual**
exposure at −0.7 EV from an unbound `Stairwell_BlockoutExposure` volume — no
auto-exposure, no adaptation — so light intensities are absolute. The two
pendant lamps meant to light the apartment were authored at **1.4 and 2.3 cd**
while their neighbours sat at 110 and 160. The room rendered black because it
was black. No amount of texturing would have fixed that.

### Four bugs, each caught only by looking at the result

This is the entry's real content. Every one of these reported success.

- **A material instance with an unset texture parameter falls back silently to
  the engine's `DefaultTexture`.** The prop-dressing script reported twelve
  props dressed while all twelve were untextured — the kit's textures live in
  `Art/Textures/Surfaces/`, not `Art/Textures/`. Only a read-back of the
  assigned parameters caught it.
- **`set_editor_property("light_color", …)` stores the value but never pushes
  it into the component's render state.** `set_light_color()` does.
- **Setting a component property from Python does not dirty the map package.**
  `save_dirty_packages` therefore wrote nothing, returned `True`, and an entire
  colour-grading pass was lost. The evidence was a render that came back
  byte-for-byte identical to the previous one. Actors are now `modify()`'d
  before their components are touched.
- **Props floated 20 to 110 cm above the floor.** Invisible while there was no
  floor to float above.

The common thread: a log line saying the work was done is not evidence the work
was done. `capture_region.py` exists now for exactly this — it renders a
region's authored shot to PNG headlessly, through the region's own
`CameraOffset` and `FieldOfView`, so look-and-feel is judged from the image.

### Also

Prop footprint went from 580 MB to 3.1 MB. The previous pass shipped the
FBX-embedded 4K texture sets and capped them with `MaxTextureSize`, which is the
wrong tool: it caps what the renderer *builds* while the `.uasset` still stores
the full-resolution source. The props are untextured now and dressed from
instances of the shared `M_REW_Surface`, which also makes them read as one
building rather than twelve separate downloads.

REW-0028's eighteen decal instances had never been placed. Twenty are now
placed to the building's own logic — water under the balcony header, peeling
plaster where hands reach, cracks radiating from the fusebox and down the
columns, grime along the walking line.

Follow-on charted as REW-0031: the environment kit ships base colour only. There
is not one normal or roughness map in it, so every surface in the game is
perfectly flat.

## 2026-08-26 — REW-0028, grime decal instances from environment-kit atlases

- Date: 2026-08-26
- Author: codex-decals
- Task: REW-0028
- Branch: `codex/rew-0028-grime-decals`
- Change: imported six LFS-tracked texture assets from the owner-supplied
  Environment Texture Kit v1: `D_REW_PeelingPlaster_Atlas_Mask`,
  `D_REW_LeaksRust_Atlas_Mask`, `D_REW_WallCracks_Atlas_Mask`,
  `T_REW_Neon_4C_Emissive`, `T_REW_Neon_REWIND_Emissive` and
  `T_REW_Neon_TRANSIT_Emissive`. Created sixteen instances on the existing
  `/Game/Art/Materials/Decals/M_REW_GrimeDecal`:
  `MI_REW_Decal_PeelingPlaster1` through `MI_REW_Decal_PeelingPlaster4`,
  `MI_REW_Decal_LeaksRust1` through `MI_REW_Decal_LeaksRust6`, and
  `MI_REW_Decal_WallCracks1` through `MI_REW_Decal_WallCracks6`.
- Import contract: mask textures are non-sRGB, `TC_Masks`, Clamp; neon textures
  are sRGB, Default compression with alpha retained, Clamp. The committed
  `Rewind/ArtSource/Scripts/import_rew_grime_decals.py` makes the import
  repeatable from the six tracked source PNGs. `EnvironmentKitV1/REW-0028-decal-provenance.md`
  records known provenance and the source README's omitted generator/model/date
  rather than inventing them.
- Atlas finding: all three masks are 1254 × 1254. Direct visual inspection and
  the kit README's stated mask counts established a 2 × 2 peeling-plaster grid
  and 3 × 2 grids for leaks/rust and wall cracks. Instances are row-major from
  the top-left. Their UVScale/UVOffset overrides, masked source and dark tint
  were set by the script; leaks/rust alone receives the dark rusty-brown tint.
- Verification: a headless importer run read back every instance's parent,
  `Mask`, `UVScale` and `UVOffset` and logged `verified` for all 16 before
  `DONE: imported 6 textures, configured 16 decal instances, and verified 16`.
  Editor-closed `RewindEditor Win64 Development` succeeded (`Result:
  Succeeded`, 57.82 s). Headless `Automation RunTests Rewind;Quit` recorded
  22 `Rewind.*` successes, 0 failures and `TEST COMPLETE. EXIT CODE: 0`.
- Not verified: no `.umap` was modified and no `DecalActor` was placed, by this
  task's scope; consequently there is no in-level visual-placement evidence or
  neon material-treatment evidence. Packaged builds and final art integration
  were not run.
- Signature: codex-decals
## 2026-08-26 — REW-0027, environment texture kit surface instances

- Date: 2026-08-26
- Author: codex-surfaces
- Task: REW-0027
- Branch: `codex/rew-0027-surface-instances`
- Change: retained the twelve quality-94 JPG sources and provenance record at
  `Rewind/ArtSource/Textures/EnvironmentTextureKit-v1/`, and added the
  reproducible headless import at
  `Rewind/ArtSource/Scripts/import_environment_texture_kit.py`. The script
  imports no decal atlas or neon source, creates no map asset, and does not
  edit `M_REW_Surface`.
- Assets created: textures
  `T_REW_Surface_PlasterGray_Aged_BC`, `T_REW_Surface_PlasterGreen_Aged_BC`,
  `T_REW_Surface_BrickDark_Aged_BC`,
  `T_REW_Surface_PavingStone_Courtyard_BC`,
  `T_REW_Surface_FloorTile_Interior_BC`,
  `T_REW_Surface_CeramicTile_Ivory_Aged_BC`,
  `T_REW_Surface_MetalBlack_Painted_Aged_BC`,
  `T_REW_Surface_WoodDark_Worn_BC`, `T_REW_Backdrop_CityRain_Night_BC`,
  `T_REW_Poster_TransitMap_BC`, `T_REW_Signs_FloorCourtyard_Atlas_BC` and
  `T_REW_Label_FuseBoxInstructions_BC` under
  `/Game/Art/Textures/Surfaces`; instances `MI_REW_PlasterGray_Aged`,
  `MI_REW_PlasterGreen_Aged`, `MI_REW_BrickDark_Aged`,
  `MI_REW_PavingStone_Courtyard`, `MI_REW_FloorTile_Interior`,
  `MI_REW_CeramicTile_Ivory_Aged`, `MI_REW_MetalBlack_Painted_Aged` and
  `MI_REW_WoodDark_Worn` under `/Game/Art/Materials/Surfaces`.
- Settings: all twelve use sRGB and `MaxTextureSize=1024`; the eight surface
  textures use Wrap and World texture group, CityRain uses Clamp and World,
  and the three close-readable poster/sign assets use Clamp and UI. The eight
  instances inherit `M_REW_Surface`, set `BaseColorTex`, white `Tint`, the kit
  dry roughness start value, and TileSize in centimetres. Brick (120 cm) and
  paving (200 cm) repeat larger than ceramic tile (40 cm).
- Verification: headless import log recorded `VERIFY PASS: 12 textures and 8
  material instances read back from saved assets`; this read back each asset's
  existence, sRGB, address mode, texture group, `MaxTextureSize`, instance
  parent, `BaseColorTex` and `TileSize`. The editor-closed
  `RewindEditor Win64 Development` build succeeded in 30.33 s. Headless
  `Automation RunTests Rewind` discovered 22 tests; all 22 returned
  `Result={Success}` and the run ended `TEST COMPLETE. EXIT CODE: 0`.
- Cost: the retained JPEGs total 5.53 MiB, imported texture packages 5.66 MiB
  and material instances 40.6 KiB: 11.23 MiB added through Git LFS.
- Not verified: no level was modified or assigned these instances, so there is
  no in-game texel-density or final visual judgement. The kit README reserves
  that judgement for a later look pass. No packaged build was made.
- Signature: codex-surfaces
## 2026-08-26 — REW-0029, bind The Returner as the player character

- Date: 2026-08-26
- Author: codex-character
- Task: REW-0029
- Branch: `codex/rew-0029-bind-returner`
- Change: `ARewindCharacter` now loads
  `/Game/Characters/Returner/Returner`, `A_Returner_Alert` and
  `A_Returner_Walk` on the Returner's own skeleton. Its existing
  velocity-threshold locomotion remains unchanged: Alert starts as the looping
  stationary animation and Walk starts when planar velocity exceeds 5 cm/s.
  `MI_CharacterSilhouette` remains applied in a loop over every material slot.
  Tier 1 assets remain untouched and unbound.
- Alignment evidence: headless `inspect_returner_alignment.py` read mesh bounds
  with origin `(0, 0.000002, 84.999999)` and extent
  `(75.518257, 26.450047, 84.999985)`, making the imported mesh minimum Z
  effectively zero: it is feet-origin. The same editor run read the 24-bone
  skeleton reference pose; its composed `Head` to `headfront` vector was
  `(-0.000000, 9.886821, 0.000048)`, i.e. +Y. The capsule remains 96 cm in
  half-height, so the binding uses relative location `(0, 0, -96)` and yaw
  `-90°`, which maps the Returner's +Y facing vector to the pawn +X movement
  direction.
- Verification: with no Unreal Editor process for this clone, `Build.bat
  RewindEditor Win64 Development` succeeded in 29.30 s. Headless
  `Automation RunTests Rewind` completed 22/22 success and logged
  `TEST COMPLETE. EXIT CODE: 0`. `git diff --check` passed before commits.
- Not verified: no PIE frame or live input run. The agent session had no
  verified editor MCP endpoint and `RewindEditor.RewindPIEInputToolset`, so no
  desktop-input substitute was used. No `.umap`, texture or material asset was
  opened or modified; no packaged build was made.
- Signature: codex-character

## 2026-08-25 — Register repair: REW-0020 was never claimed on main

- Date: 2026-08-25
- Author: Claude
- Task: none. Operator repair.
- Branch: `claude/register-rew-0020`
- Defect: REW-0020 is complete, merged and archived, and its identity was
  never in `docs/TASK_IDS.md` on `main`. The claim was made on
  `claude/rew-0020-charter`, which was never merged; REW-0021's claim commit
  then appended REW-0021 onward to `main` directly, so the register jumped
  from REW-0019 to REW-0021 while `docs/finished/` held a REW-0020 archive.
- Why it matters: the register's job is to make an identity taken before a
  charter freezes. A number missing from it is a number a future task can
  claim twice, and REW-0020 is cited by the archive, by the journal and by
  three automation tests.
- Repair: the row is appended at the end of the table rather than inserted
  in numeric order. `docs/TASK_IDS.md` states the rule — never insert into
  the middle, never sort — because the append point is what converts a
  simultaneous claim into a merge conflict instead of a silent duplicate.
  Ordering is not a property the register promises; uniqueness is.
- Its Work column names the archive rather than `docs/CURRENT_TASK.md`,
  because the task is finished and that is where its record actually lives.
- Root cause worth remembering: chartering on a branch that is then never
  merged loses the claim silently. The claim must reach `main` on its own,
  which is what the workflow already says and what this missed.
- Verification: `grep REW-0020 docs/TASK_IDS.md` now matches; the archive
  and the register agree. Documentation only.
- Signature: Claude

## 2026-08-25 — REW-0020, authored courtyard and the ground-fuse checkpoint

- Date: 2026-08-25
- Author: grok-courtyard
- Task: REW-0020
- Branch: `grok/rew-0020-authored-courtyard`
- Change: `ARewindAuthoredCourtyard` builds the courtyard on
  `Rewind.SkipProofLayout` maps only: approach hall, plaza, generator,
  courtyard gate, Transit exit, chain-link occluder, two camera regions, and
  `GroundFuseGate` at the hall-to-courtyard seam. The procedural proof keeps
  its checkpoint at X = 4110. The existing courtyard fuse socket is moved to
  that seam and the generator is wired to the gate. Developer commands
  `Rewind.SetActiveAnchor`, `Rewind.PlacePlayerForVerification` and
  `Rewind.SeatFuseForVerification` exist only for verification.
- Why it was needed: REW-0017's rewind could not be seen on the authored
  default map, which has no courtyard, so `GroundFuseGate` never existed in
  play. Putting the checkpoint anywhere else would invent a causal contract
  no design document states.
- Attach: PIE logged `GroundHall` +Y seam Y `1500`, threshold
  `(-430, 2700, 150)`, nine camera regions. Named tests
  `Rewind.AuthoredCourtyard.SpatialConstants`,
  `Rewind.AuthoredCourtyard.RegionAbutment` and
  `Rewind.AuthoredCourtyard.SkipProofLayoutSpawnsCheckpoint` assert the
  constants, the abutment, and that SkipProofLayout does not force X = 4110.
- Verification: editor-closed `RewindEditor Win64 Development` succeeded
  (final confirm 1.78 s, target up to date). Headless `Rewind.*`: 22/22
  Success, `TEST COMPLETE. EXIT CODE: 0`. Three PIE runs, keys released
  before stop, recorded in
  `docs/playtests/authored-courtyard-fl17-2026-08-25.md`. Fail: `t=142.76`
  `Checkpoint GroundFuseGate: FAIL`, prelude 2.00 s, `LOOP BREAK
  interference=0.17`, next loop in 4C. Anchor: `SetActiveAnchor
  courtyard_gate_open`, `t=33.00` pass. Power: `Fuse: seated in the
  courtyard socket` then `t=48.33` pass. Interference capture during a
  prelude:
  `docs/playtests/authored-courtyard-fl17-2026-08-25/interference-prelude.png`.
- FL-17: the three checkpoint outcomes were observed in PIE. The power
  seating used `Rewind.SeatFuseForVerification` because the 4C fuse at
  `(-250, 1180, 1350)` sits 5 cm outside `Apartment4C_Region` beside the
  building socket, so this-loop pickup did not land. That is the predicate
  FL-17 names, not a played Loop B from 4C.
- Not verified: FL-18; packaged build; frame-rate variation; carrying the
  fuse by walking from 4C; street and Transit interiors; a judged first-pass
  look. The moon key blows the plaza floor under the authored interior
  exposure.
- Signature: grok-courtyard

## 2026-08-25 — REW-0023, first-run copy on the message system

- Date: 2026-08-25
- Author: grok-copy
- Task: REW-0023
- Branch: `grok/rew-0023-tutorial-copy`
- Change: a first-run copy set in `FRewindMessageCatalog`, driven by the
  existing message subsystem, plus `docs/design/tutorial-and-first-run.md`
  which owns first-run copy, the naming rule, and first-time gating. Gated
  lines are recorded as knowledge facts `seen.<MessageId>` on
  `URewindSessionSubsystem`. The message subsystem API is unchanged.
- Why it was needed: a first-time player started in 4C with no statement
  that the world loops or that knowledge persists, while several existing
  catalog lines printed `7312` or told the player to seat the fuse.
- The naming rule, restated rather than invented: the game names what a
  thing is and never what to do with it. The radio may be described as
  carrying a voice. The digits are never printed in first-run copy.
- Discoveries routed as notes, not new rules: `BeginPlay` does not re-run
  at loop start, so return copy is triggered from the rewind prelude
  ending; death has no prelude, so `Apartment.Returned` is not shown then;
  `ARewindStairwell` is proof-layout only, so stairs copy keys off a stair
  camera region; a GameInstanceSubsystem cannot be `NewObject`'d in the
  gating test, so the test uses the same `TSet<FName>` the session stores.

Delivered first-run set, quoted from `RewindMessageCatalog.cpp`:

```text
Apartment.Waking        Apartment 4C. The room starts over. You do not.
Character.Controls      WASD move. E interact.
Radio.Present           A radio.
Lock.Prompt             A lock. Four digits.
Fuse.Carried            A fuse.
Socket.EmptyAtRest      An empty socket.
Lift.NoPower            The cage is still. The socket is empty.
Stairs.Remain           The stairs remain.
Loop.Break              The world will not hold.
Apartment.Returned      The room is as it was. You remember.
```

`Lift.NoPower` is first-run copy in its words and a refused action in when
it speaks, so it is not first-time gated.

Operational catalog lines rewritten so they do not state a solution:

```text
Lock.CodeBuffer         Code: {0}
Lock.Accepted           The lock opens.
Lock.Rejected           {0} does not open it.
Fuse.SeatedBuilding     The fuse is seated.
Fuse.SeatedCourtyard    The fuse is seated.
Fuse.TakenBuilding      The fuse is in hand.
Fuse.TakenCourtyard     The fuse is in hand.
Socket.EmptyOther       Empty. The fuse is elsewhere.
Generator.Dead          The generator is dead.
Generator.Online        The generator is running.
Anchor.Accepted         This will hold.
Anchor.Refused          Nothing to hold.
Radio.ChannelStatic     Radio: channel {0}. Static.
Radio.ChannelVoice      Radio: channel {0}. A voice, under the static.
Radio.DigitSeven        Radio:  ...seven...
Radio.DigitThree        Radio:  ...three...
Radio.DigitOne          Radio:  ...one...
Radio.DigitTwo          Radio:  ...two...
Radio.CodeObtained      The sequence is yours.
```

Radio spoken digits remain the puzzle speaking. They are not first-run
copy. First-run copy does not reprint them. No catalog template contains
`7312`. No catalog line describes a ghost.

- Verification: RewindEditor Win64 Development, editor closed, after the
  gating-test fix and unused-helper removal: Result Succeeded, total
  execution time 20.11 seconds.
  Headless `UnrealEditor-Cmd` `-ExecCmds='Automation RunTests Rewind;Quit'`
  `-unattended -nopause -nosplash -NullRHI`. Found 19 tests. All 19
  `Result={Success}`, including `Rewind.Message.FirstRun.Gating` and
  `Rewind.Message.FirstRun.OmitsSolutions`. `TEST COMPLETE. EXIT CODE: 0`.
  An earlier run of the same command failed `Rewind.Message.FirstRun.Gating`
  with ensure `Object None of class RewindSessionSubsystem ... created in
  invalid Outer Package`; the test was changed to the knowledge `TSet` and
  re-run. `git diff --check` not yet run at journal time; run at commit.
- Not verified: no PIE on this clone, so the first-run path was not walked
  in a running viewport. Overlay timing against the authored frame was not
  judged. Death does not show `Apartment.Returned`. The courtyard is out of
  scope. No packaged build. No localisation.
- Signature: grok-copy
## 2026-08-25 — REW-0024, the owner's textures were never third-party

- Date: 2026-08-25
- Author: Claude
- Task: REW-0024
- Branch: `claude/rew-0024-owner-textures`
- Change: the seven `4c_*` JPEG texture sources are tracked at
  `Rewind/ArtSource/Textures/`, outside `Content/` so the editor never scans
  them. ADR-0011 carries a dated amendment correcting the clause that named
  `/Game/Art/Texture/` beside `/Game/Fab/`.
- The error being corrected: ADR-0011 was written on the assumption that both
  untracked content directories were third-party imports. Only `/Game/Fab/`
  was. The owner stated on 2026-08-25 that the `4c_*` textures are hand-made
  by them and imported from JPEG, which makes them project-owned and squarely
  inside the record's own first rule. Nothing in ADR-0011's reasoning ever
  applied to them: no unverified provenance, no licence to check, and the
  sources total 76 KB against the 342 MB the record was actually about.
- Consequence: REW-0022 unbound the authored map from those textures along
  with the Fab packs. That part of REW-0022 was correct against the record as
  it then read and wrong against the facts. REW-0025 re-imports the textures
  and re-binds what should not have been unbound. It is sequenced after
  REW-0020 because both edit the same `.umap`, which cannot be merged.
- Why the sources are tracked rather than the imported assets: a `.uasset`
  can be rebuilt from a JPEG, and the JPEG is the thing the owner actually
  authored. Tracking the source means the imported form is never the only
  copy, which is what went wrong when `Rewind/Content/Art/Texture/` left the
  disk between sessions and took the only build of those textures with it.
- Verification: documentation and 76 KB of source art. `git diff --check`
  clean; the amendment is dated and does not rewrite the original text, per
  the ADR convention that superseded reasoning stays visible.
- Not verified: no build, no test, no editor. Nothing executable changed, and
  the textures are not imported yet; that is REW-0025.
- Signature: Claude

## 2026-08-25 — REW-0022, unbind the authored map from untracked content

- Date: 2026-08-25
- Author: grok-unbind
- Task: REW-0022
- Branch: `grok/rew-0022-unbind-untracked-content`
- Change: `/Game/Maps/FiveLoops_Stairwell_Blockout` no longer depends on
  untracked `/Game/Fab/` or `/Game/Art/Texture/` packages. Six StaticMeshActors
  that existed only to hold third-party meshes were deleted: `scene` (armchair)
  at `(-130, 1470, 1240)`, `fbx` (coffee table) at `(-220, 1340, 1200)`,
  `ptish_radio` at `(-230, 1330, 1260)`, `desk_lamp` at `(-229, 1357, 1260)`,
  `electrical_panel` at `(-250, 1170, 1350)`, and an unnamed `StaticMeshActor`
  at `(-250, 1440, 1270)`. Surfaces whose untracked materials had already
  collapsed to the engine cube default received project-owned instances from
  `Rewind/Content/Art/Materials/`: `MI_StairFloor` on stair/hall landings,
  `MI_4CFloor` on the 4C floor (`Floor_4_Landing2`), `MI_StairWall_Upper` /
  `MI_StairWall_Lower` on the stairwell envelope, `MI_StairDoor` on closed
  doors and the 4C code lock, and the same family on remaining 4C cube
  dressing. Gameplay actors, seven camera regions, lights and collision were
  not moved. Apartment 4C is undressed; that is the ADR-0011 outcome.
- Why they were needed: the REW-0015 saved map referenced those untracked
  packages. REW-0019's save in a clone that lacked the files had already
  dropped the package paths and left empty-mesh actors plus WorldGrid
  overrides. Editor Asset Registry on this clone, with `Rewind/Content/Fab/`
  and `Rewind/Content/Art/Texture/` absent, listed no `/Game/Fab/` or
  `/Game/Art/Texture/` dependency before the edit; the empty-mesh actors were
  the remaining bind.
- Verification: `RewindEditor Win64 Development` built editor-closed, result
  Succeeded, target up to date after the map save (first closed build 57.06 s
  with four actions; post-save closed build 1.94 s, 0 actions). All seventeen
  discoverable `Rewind.*` tests passed together headless:
  `TEST COMPLETE. EXIT CODE: 0`. Clean-open: this clone never had the
  untracked directories, so they were not renamed; a fresh
  `UnrealEditor-Cmd` load of the saved default map reported Asset Registry
  `untracked=[]`, `missing_mesh=[]`, actor count 59, and no `LogLinker` /
  `Can't find file` lines for `/Game/Fab/` or `/Game/Art/Texture/`. A strings
  scan of the saved umap found zero `Fab`, `Art/Texture`, `desk_lamp` or
  `ptish_radio` tokens. Gameplay and camera transforms matched the pre-edit
  inspect, including `Apartment4C_Region` at `(62.5, 1368, 1340)` and
  `4c_camera` at `(750, 1330, 1330)`.
- Not verified: no packaged build, no live PIE walk of 4C-hall-stairs-lift,
  no owner visual review of the undressed 4C, and no criterion in
  `docs/acceptance/five-loops-test.md` was re-run. `docs/SYSTEMDOC.md` still
  describes the saved map as referencing those untracked packages; it was
  outside this task's file ownership.
- Signature: grok-unbind

## 2026-08-25 — REW-0021, the asset-source and Echo decisions

- Date: 2026-08-25
- Author: Claude
- Task: REW-0021
- Branch: `claude/rew-0021-decisions`
- Change: two decisions the owner made on 2026-08-25 are recorded as
  `ADR-0011_asset-source-and-provenance.md` and `ADR-0012_echo-semantics.md`,
  both Accepted. Art assets are project-owned or generated, and third-party
  packs do not enter the tracked dependency closure. Echo is narrative only:
  nothing samples the player's transform, nothing persists a path, nothing
  replays a ghost.
- Why they were needed: the tracked default map referenced 342 MB of
  untracked `/Game/Fab/` and `/Game/Art/Texture/` packages, so the repository
  claimed a map it could not reproduce, which three agent clones proved
  concrete during REW-0016. Echo had been the first open decision in
  `docs/PROJECT_BRIEF.md` since the reboot began and was blocking task
  decomposition.
- Consequence recorded rather than glossed: Apartment 4C returns to undressed
  blockout when REW-0022 unbinds the map. That is a visible regression in the
  owner's editor and is accepted deliberately.
- Documents corrected in the same change: the ADR index and its open-decision
  paragraph, `PROJECT_BRIEF.md`'s open-decision list, both backlog proposals
  and their index rows, and the `CURRENT_STATUS.md` rows that described Echo
  as unwritten rather than decided against.
- Verification: documentation only. Links and fenced blocks reviewed by hand;
  `git diff --check` clean. No build, no test, no editor: nothing executable
  changed.
- Not verified: nothing that requires a build. The decisions themselves are
  not evidence about the game, and ADR-0011's consequences are not yet
  implemented; REW-0022 does that.
- Signature: Claude

## 2026-08-25 — REW-0016 complete, Chapter 1 wave run through delegated agents

- Date: 2026-08-25
- Author: Claude
- Task: REW-0016
- Branch: `claude/rew-0016-operator`
- Change: REW-0017, REW-0018 and REW-0019 were chartered, frozen, delegated to
  three `grok` agents in three separate clones under the owner's
  `rewind-multiagents` directory, then reviewed and merged. Identities were
  claimed on `main` in advance so no agent had to edit `docs/TASK_IDS.md`.
  Each agent transcribed an operator-frozen charter, branched, committed,
  pushed and opened its own pull request; the operator merged.
- Delegation mechanism: the owner's `powershell-agent-mcp` stdio server. A
  stdio MCP server cannot be loaded into an already-running session, so it was
  driven through a localhost HTTP bridge holding one long-lived server
  process, which is what keeps its in-memory agent registry usable across
  calls.
- Review: claims were checked against the tree rather than accepted from the
  journal entries. The four REW-0017 tests and four REW-0018 tests were
  confirmed present in source; `bUseWholeSpaceDeadline` was confirmed `false`,
  so the 240-second timeout is genuinely off by default;
  `AddOnScreenDebugMessage` was confirmed absent from the eight migrated
  actors and present only in `RewindLoopSubsystem.cpp`, which REW-0017 owned;
  every charter archive and restored template was confirmed.
- Verification: the merged tree built editor-closed in 23.12 seconds. All
  seventeen discoverable `Rewind.*` tests ran together: 17 passed, 0 failed,
  `TEST COMPLETE. EXIT CODE: 0`. Each agent had verified only its own four,
  because on its branch the other four did not exist.
- Operator defect, recorded rather than smoothed over: `RewindAnchorBoard.*`
  was granted to both REW-0017 and REW-0018. The file-ownership split existed
  to prevent exactly that and was written wrong. Both branches edited the
  file; the conflict was resolved by hand keeping both changes, because the
  merged log format string carries three placeholders and REW-0018's side
  passed two. Taking either side alone would have shipped a broken call.
- Second operator defect: two agents ran the REW-0018 brief in the same clone
  for about seven minutes, after the operator launched one while the owner
  launched another. The duplicate was stopped with the orchestrator's
  `halt-agent`. The surviving branch's file set was checked for orphans before
  merge and was coherent.
- Stale-status catch: REW-0019 branched before REW-0017 merged, so its
  `docs/CURRENT_STATUS.md` still described ADR-0009 as accepted but not
  implemented, and the loop as ending at 240 seconds. Both were true when it
  branched and false at merge. The `main` rows were kept for those.
- Not verified: no PIE, no packaged build and no playtest of the merged
  result. This task merged and verified other tasks' work; it did not play the
  game. Each child's own unverified list stands, including that REW-0017's
  causal checkpoint exists only on the procedural proof map, because the
  authored default map has no courtyard.
- Routed, not built: `docs/backlog/untracked-fab-dependency.md`,
  `docs/backlog/echo-semantics.md` and
  `docs/backlog/chapter-1-remaining-content.md`.
- Signature: Claude

## 2026-08-25 — REW-0019 complete, renderer configuration and first look pass

- Date: 2026-08-25
- Author: grok-look
- Task: REW-0019
- Branch: `grok/rew-0019-renderer-and-look`
- Decision: [`ADR-0010`](adr/ADR-0010_renderer-configuration.md) enables Lumen
  GI, Lumen reflections, virtual shadow maps, mesh distance fields and default
  bloom. Auto-exposure stays off. Hardware ray tracing stays off. Frame-time
  budget 16.67 ms at 1920×1080 on `MRWHITE81` (RTX 3060 Ti).
- Config: `Rewind/Config/DefaultEngine.ini` matches the ADR. Live
  `SearchCVars` after the change reported Bloom 1, MDF 1, VSM 1, auto-exposure
  0. The after `-game` log set `r.DynamicGlobalIlluminationMethod:1` and
  `r.ReflectionMethod:1`.
- Light pass: existing practicals retuned for source size and bounce; cool
  fills kept large and dim; directional light dropped to 0.25 without shadows;
  new stationary skylight `Authored_CoolSkyFill` at 0.22; `4C_SignLight` plus
  project-owned `M_4CSignEmissive` on the existing `Cube3` sign panel at
  `(-250, 1190, 1420)`. Untracked `/Game/Fab/` and `/Game/Art/Texture/` were
  not added, removed or committed.
- Frame time, same 1920×1080 `-game -benchmark` method, 361 ticks from 4C
  PlayerStart: before 8.54 ms (117 fps), after 13.03 ms (77 fps). Inside the
  16.67 ms budget.
- Visual evidence:
  [`docs/playtests/renderer-look-2026-08-25.md`](playtests/renderer-look-2026-08-25.md).
  The after 4C PIE frame at `(750, 1330, 1330)` shows magenta wall bleed from
  the sign, a readable checker floor that is not pure black, cast shadows and
  a dark player silhouette. It supports visual-direction statements 4 and the
  silhouette/practical/shadow parts of 2. It does not show stair treads
  (statement 2 incomplete) and does not re-verify wall-band separation
  (statement 1).
- Verification: `RewindEditor Win64 Development` built editor-closed in 3.87 s,
  target up to date. All nine discoverable `Rewind.*` automation tests passed
  together headless, `TEST COMPLETE. EXIT CODE: 0`.
- Not verified: packaged build, `stat unit` GPU split, stairwell PIE at the
  35 mm StairwayCamera, FL criteria, final art, Fab provenance.
- Signature: grok-look
## 2026-08-25 — REW-0018 complete, player-facing text leaves the debug overlay

- Date: 2026-08-25
- Author: grok-message
- Task: REW-0018
- Branch: `grok/rew-0018-message-and-dialog`
- Change: player-facing text in the authored slice is owned by
  `URewindMessageSubsystem`. Actors speak by catalog id. Copy and duration
  live in `FRewindMessageCatalog`. Presentation is a project Slate overlay at
  the bottom of the viewport, light text on a dark backing, hit-test
  invisible. `docs/design/player-messages.md` owns the channel and states that
  the debug overlay is not UI. Migrated call sites: RewindCodeLock,
  RewindRadio, RewindFuse, RewindFuseSocket, RewindGenerator, RewindLift,
  RewindAnchorBoard, RewindCharacter. `RewindLoopSubsystem` was not edited;
  its `t=` overlay remains.
- Verification: `RewindEditor Win64 Development` built editor-closed. A first
  pass failed on `RewindMessageTest.cpp` `TestEqual` int32 versus size_t;
  after the cast, the same command succeeded in 16.61 seconds. Headless
  `UnrealEditor-Cmd` `-ExecCmds='Automation RunTests Rewind;Quit'`
  `-unattended -nopause -nosplash -NullRHI` found 13 `Rewind.*` tests and
  completed all 13 with `Result={Success}`, `TEST COMPLETE. EXIT CODE: 0`,
  including `Rewind.Message.Queue.Queueing`, `Rewind.Message.Queue.Ordering`,
  `Rewind.Message.Queue.Expiry` and `Rewind.Message.Catalog.ResolvesSliceCopy`.
  After the migration, `AddOnScreenDebugMessage` remains only in
  `RewindLoopSubsystem.cpp`. Player-facing literals for the slice now live in
  the catalog, not in the listed actors.
- Not verified: no interactive editor and no PIE on this clone, so the overlay
  was not judged against the authored camera composition in a running
  viewport. No packaged build. Each migrated line was not triggered in play;
  reachability is the actor call plus catalog resolution, not a playtest.
  Whether rewind should clear the queue is not a rule and was not implemented.
- Signature: grok-message
## 2026-08-25 — REW-0017 complete, event-driven rewind and the loop-break signature

- Date: 2026-08-25
- Author: grok-loop
- Task: REW-0017
- Branch: `grok/rew-0017-event-driven-rewind`
- Change: `URewindLoopSubsystem` now ends a loop on causal-contract failure,
  death or a successful first-time Anchor commit. The automatic 240-second
  timeout is off by default; `bUseWholeSpaceDeadline` keeps an authored
  whole-space deadline available. `ARewindCausalCheckpoint` evaluates the
  named `GroundFuseGate` predicate on crossing, not continuously, and latches
  a rewind that `NotifyExited` cannot cancel. Contract failure and first-time
  commit run a prelude clamped to [1.0, 3.0] seconds of elapsed loop time
  before loop-start apply. Death still ends immediately. The board latches
  rewind only when `TryCommit` succeeds and a pending write remains; rejected
  and redundant commits do not. `ARewindLoopBreakSignature` raises film-grain,
  fringe and desaturation from elapsed loop time during the prelude. The
  GroundFuseGate volume is spawned in C++ at the proof-layout hall-to-courtyard
  seam (X = 4110); skip-layout maps do not receive it.
- Verification: `RewindEditor Win64 Development` built editor-closed in 137.38 s
  (full makefile) and again in 17.30 s after the commit-boundary test fix.
  Headless `UnrealEditor-Cmd` `-ExecCmds='Automation RunTests Rewind;Quit'`
  `-unattended -nopause -nosplash -NullRHI` found 13 `Rewind.*` tests and
  completed `**** TEST COMPLETE. EXIT CODE: 0 ****`. Every result was Success,
  including `Rewind.Loop.Prelude.Bounds`, `Rewind.Loop.Prelude.Latch`,
  `Rewind.Loop.AnchorCommit.Boundary` and
  `Rewind.Loop.BreakSignature.ReadsLoopTime`. Evidence is
  `Rewind/Saved/Logs/Rewind.log` from the 2026-08-25 03:43 run.
- Not verified: no packaged build; no live PIE crossing of GroundFuseGate or
  first-time Anchor commit on the procedural proof map; no idle observation
  past 240 seconds in PIE; no frame-rate variation; no FL-17 or FL-18 playtest.
  The default authored map has no courtyard, so the checkpoint is absent there.
  Final static, sound and lighting remain REW-0019. `URewindSessionSubsystem`
  was not edited; the commit boundary is the board's accepted-and-pending
  rule plus the loop subsystem latch.
- Signature: grok-loop

## 2026-08-25 — REW-0015 complete, player control restored in 4C

- Date: 2026-08-25
- Author: Claude
- Task: REW-0015
- Branch: `claude/rew-0015-restore-player-control`
- Reported defect: the controller became unresponsive after using the radio,
  after taking the fuse, and when leaving Apartment 4C. None of those three
  actors touches input; the only `DisableMovement` in the project belongs to
  the lift and is re-enabled on exit.
- Measured cause: `Apartment4C_Region` declared a player volume of
  `X [-130, 370)` while the room's back wall traces at `X = -250`. The radio
  sits at `X = -225`, the fuse at `X = -250` and the 4C doorway between them.
  `TopHall_Region` reaches only `X < -245`, so a 115 cm band belonged to no
  region. `ARewindCharacter::GetScreenAxes` returned false there and
  `MoveForward` and `MoveRight` dropped the input entirely, while
  `ARewindCameraRig` held its last good frame. The picture kept working and the
  controller did not, which is why the interactions read as the cause.
- Change: movement now holds the last contained region's screen axes instead of
  dropping input, falls back to the view target's rotation if no region has ever
  contained the player, and logs entry into and exit from the no-region state.
  `ClampToPlayerVolume` insets 1 mm inside its half-open positive faces, so the
  clamp can no longer place the player outside the region that clamped them.
  `Apartment4C_Region` moved to centre `(62.5, 1368, 1340)` with extent
  `(307.5, 515, 160)`, covering `X [-245, 370)`, and its `CameraOffset` moved to
  `(687.5, -250, -10)` so the owner's adopted 35 mm frame is unchanged.
- Verification: `RewindEditor Win64 Development` built editor-closed in 10.30 s.
  All nine discoverable `Rewind.*` automation tests passed together, including
  the new `Rewind.Camera.Region.PlayerVolumeClamp`, which asserts that a clamped
  location is still contained by the region that clamped it. PIE walked from the
  4C start to the back wall at `X = -207.7`, interacted, walked back out, and
  crossed the doorway into `TopHall_Region`; every sample reported
  `Apartment4C_Region` and camera `(750, 1330, 1330)` at FOV `37.497356`,
  unchanged from REW-0014. Held keys were released before every stop. The owner
  confirmed play works.
- Not verified: no packaged build, no frame-rate variation, no fuse-seating or
  lift route, and no criterion in `docs/acceptance/five-loops-test.md` was
  re-run. This task changed movement and one region volume.
- Discovered, not fixed: owner-placed props block the walking line to the radio.
  The Fab armchair occupies `X [-215, -45]`, `Y [1358, 1582]`, leaving 15 cm to
  the back wall, and a 20 by 20 cm column stands at `X [-170, -150]`,
  `Y [1570, 1590]`. Both are dressing decisions, not control defects.
- Open and unowned: the saved default map references untracked
  `/Game/Fab/` and `/Game/Art/Texture/` packages. A fresh clone resolves neither.
- Exception: the REW-0015 identity claim was not merged to `main` before the
  charter froze. REW-0001 recorded the same exception.
- Signature: Claude

## 2026-08-24 — REW-0014 complete, owner-authored 4C start camera

- Date: 2026-08-24
- Author: Codex
- Task: REW-0014
- Branch: `codex/rew-0014-owner-4c-camera`
- Change: the owner-placed `4c_camera` remains an editor reference at world
  position `(750, 1330, 1330)`, rotation `(0, 180, 0)` and 35 mm on a 23.76
  mm-wide 16:9 filmback. `Apartment4C_Region` now derives that exact initial
  runtime frame through offset `(640, -250, -10)`, the same rotation and
  horizontal FOV `37.497356`.
- Saved and PIE evidence: a clean editor restart reopened
  `/Game/Maps/FiveLoops_Stairwell_Blockout` with both reference and region
  values intact. Standard PIE reported the sole view target
  `RewindCameraRig_0`, active region `Apartment4C_Region`, camera position
  `(750, 1330, 1330)`, rotation `(0, 180, 0)` and FOV `37.497356`. A clean
  generated viewport capture showed the authored 4C composition.
- Verification: held PIE keys were empty before stop.
  `Rewind.Camera.Region.VerticalTravel` and
  `Rewind.Project.DefaultAuthoredMap` passed together, 2/2 in 0.027780 seconds
  with no errors or warnings. No C++ changed, so no build was required. The map
  remains under Git LFS and documentation diff checks passed.
- Preserved owner work: the pre-existing untracked
  `Rewind/Content/Art/Texture/` tree was neither inspected, edited nor staged.
  Additional owner content at `Rewind/Content/Art/Materials/4c_door.uasset` and
  under `Rewind/Content/Fab/` appeared during verification and was likewise
  left untouched and unstaged.
- Not verified: no packaged build, full automation suite, wider route playtest
  or other camera-region review; none was required by this bounded framing task.
- Handoff: owner visually review the 4C opening composition. Adjust the
  reference camera first if another frame is desired, then re-derive the region
  offset from PlayerStart rather than adding a competing runtime camera.
- Signature: Codex

## 2026-08-24 — REW-0013 complete, owner-correct 4C and lift layout

- Date: 2026-08-24
- Author: Codex
- Task: REW-0013
- Branch: `codex/rew-0013-correct-building-side`
- Change: `/Game/Maps/FiveLoops_Stairwell_Blockout` now uses the owner's three
  existing openings on one corridor wall in the order stairs, lift and 4C.
  The wrong-side REW-0012 duplicates and primitive furniture were removed.
  Apartment 4C occupies its existing 400 by 1050 cm footprint at 300 cm room
  height, while the shaft aligns to the middle opening at `(-40, 630)`.
- Presentation: gameplay actors, localized lighting and the Apartment4C,
  TopHall, LiftShaft and GroundHall camera regions were relocated to the
  corrected spaces. Their Y/Y/Z/Y travel and 60/58/60/58 degree FOV preserve
  the fixed-camera silhouette grammar. No Fab or Marketplace binary was
  imported or selected; the corrected rooms intentionally remain undressed.
- Saved-map and gameplay evidence: the clean saved reopen reported 51 actors
  and no dirty package. Standard PIE spawned at `(0, 1580, 1298.15)`, rejected
  `1111`, accepted `7312`, crossed from 4C through the common hall to the
  unchanged stair region, and entered the cutaway cage without a camera gap or
  duplicate view. Clean viewport frames cover 4C, all three openings and lift.
- Lift evidence: an empty building socket refused travel. After seating the
  one fuse, descent and ascent each measured exactly 6.00 seconds. The rotated
  authored lift transformed its local hall offset and released at X `-280`
  into GroundHall and TopHall respectively. Held PIE inputs were empty before
  every stop; the procedural proof retains its default local handoff.
- Verification: UE 5.8 `RewindEditor Win64 Development` built editor-closed in
  44.27 seconds. All eight discoverable `Rewind.*` automation tests passed
  together, 8/8 in 0.075142 seconds with no errors or warnings. The changed map
  remains under the existing Git LFS rule; live documentation and diff checks
  passed.
- Not verified: no packaged build, final art, selected Fab prop set, final
  audio, courtyard, streets, Transit, rain/neon dressing or ADR-0009
  event-driven rewind implementation.
- Handoff: owner visual review the corrected authored map. Import and dress
  with specific Fab assets only through a separately bounded provenance,
  license and repository-cost review.
- Signature: Codex

## 2026-08-24 — REW-0012 complete, authored 4C and lift slice

- Date: 2026-08-24
- Author: Codex
- Task: REW-0012
- Branch: `codex/rew-0012-authored-4c-lift`
- Change: `/Game/Maps/FiveLoops_Stairwell_Blockout` is now the editor and game
  default and extends the accepted stairwell with a 700 by 1200 by 300 cm 4C,
  fourth-floor common hall, entrance hall and cutaway lift shaft/cage. The
  saved map contains the deterministic radio, manual `7312` lock, one carried
  fuse, building and ground sockets, and 52 actors after final reopen. The
  procedural proof remains preserved separately and is not overlaid in PIE.
- Camera and presentation: four new half-open Apartment4C, TopHall, LiftShaft
  and GroundHall regions extend the three accepted stair regions. They use
  Y/Y/Z/Y travel and explicit 55/65/60/65 degree FOV. Project-owned materials,
  a moderately reflective 4C floor, localized warm practicals, cool shaft fill,
  cast shadows and stable exposure continue the first-pass visual grammar; no
  unapproved Fab or marketplace asset was added.
- Gameplay: wrong `1111` was rejected and `7312` opened the hall. The placed
  fuse powered the building socket. An empty-socket run logged lift refusal.
  The powered cage measured exactly 6.00 seconds down and 6.00 seconds up and
  handed the capsule cleanly to GroundHall and TopHall instead of wedging it on
  a coplanar floor seam. The unchanged stair route is backed by REW-0011 plus
  current upper/lower handoff evidence; the second socket is backed by the
  shared exclusive fuse state plus the formal prior two-destination playtest.
- Verification: UE 5.8 `RewindEditor Win64 Development` built editor-closed in
  8.91 seconds after the final C++ change. All eight discoverable `Rewind.*`
  tests passed together, 8/8 in 0.077351 seconds; the new
  `Rewind.Project.DefaultAuthoredMap` locks both startup settings. Every PIE
  evidence run released held inputs before stop. Saved binaries resolve through
  Git LFS; live links, fences and the staged diff were checked.
- Not verified: no packaged build, final art, final audio, authored decals,
  rain/neon dressing, event-driven rewind, courtyard, streets or Transit. The
  full inherited stair route and ground-socket seating were not replayed in
  this task; their composite evidence is explicit in the playtest record.
- Handoff: owner playtest the authored default map. Any courtyard extension or
  ADR-0009 implementation requires a new bounded charter.
- Signature: Codex

## 2026-08-24 — REW-0011 complete, authored stairwell presentation slice

- Date: 2026-08-24
- Author: Codex
- Task: REW-0011
- Branch: `codex/rew-0011-stairwell-camera-lighting`
- Change: the isolated owner-scale stairwell now has three authored vertical
  camera regions. Entrance, stairs and upper threshold cover Z `[-100, 180)`,
  `[180, 1180)` and `[1180, 1500)` with half-open thresholds, Z-only travel,
  fixed authored rotations and explicit 42/37.5/37.5 degree horizontal FOV.
  The upper frame starts at `(1520, 0, 1150)`, yaw 180, preserving the owner's
  35 mm `StairwayCamera` composition.
- Presentation: two project-owned procedural masters and six instances provide
  rough upper plaster, a separate muted-green lower wall band, dark floor,
  metal, door and near-black character surfaces. Five local shadow-casting
  practicals, cool fill, manual exposure, ambient occlusion and restrained SSR
  establish a first-pass visual grammar. The eight assets total 108,671 bytes;
  no third-party environment binary is tracked or referenced.
- Traversal correction: reverse PIE found the capsule meeting the underside of
  each intermediate landing 56 cm before its top. Floors 1-4 retain their outer
  boundary but reduce landing depth from 180 to 130 cm, moving only the stair-
  side inner edge by 50 cm. The final input route reached Z 1298.15, 1002.75,
  706.73, 402.75 and 106.73 cm down, then 399.84, 706.49, 999.84 and 1298.15 cm
  back up. Region order was Upper, Stairs, Entrance, Stairs, Upper; every state
  reported the authored rig as view target and no held key remained.
- Camera and visual evidence: start PIE reported camera `(1520, 0, 1150)`,
  rotation `(0, 180, 0)`, FOV 37.5 and `Stairwell_Upper`. At the entrance it
  reported `(1400, 0, 260)`, rotation `(-3, 180, 0)`, FOV 42 and
  `Stairwell_Entrance`. Clean game-viewport PNGs showed the dark silhouette,
  separated plaster bands, localized warm/cool pools, cast rail shadows and
  readable alternating flights. The procedural breakup was reduced and made
  finer after the first capture read as marble.
- Saved-map and storage evidence: the map reopened with 29 actors and three
  camera regions. Its dependency closure contains engine basics, `/Script`
  modules and the five project material instances it uses; no `/Game/Fab`
  reference remains. The map is 445,289 bytes. All `.uasset` and `.umap`
  additions resolve to Git LFS under ADR-0005.
- Verification: UE 5.8 `RewindEditor Win64 Development` built successfully in
  7.85 seconds after the final code change. All seven `Rewind.*` automation
  tests passed, 7/7 in 0.06182 seconds with zero errors and warnings;
  `Rewind.Camera.Region.VerticalTravel` asserts Z travel, offset, bounds, FOV
  and exact-one ownership around a shared threshold. Links, fences, design
  ownership, staged LFS objects and `git diff --check` were reviewed.
- Not verified: no packaged build, final textures, authored damage decals,
  prop dressing, rain, neon, audio or wider Handmade-map camera coverage. The
  isolated slice does not implement ADR-0009 event-driven rewind or claim the
  full Chapter 1 level playable.
- Handoff: the wider Handmade adoption remains a backlog proposal. The next
  task may choose event-driven rewind implementation or one separately
  chartered adjacent-space slice; neither is implied by this completion.
- Signature: Codex

## 2026-08-24 — REW-0010 complete, owner-scale stairwell blockout

- Date: 2026-08-24
- Author: Codex
- Task: REW-0010
- Branch: `codex/rew-0010-stairwell-blockout`
- Change: preserved the owner-authored Handmade2 construction as the untouched
  stable reference `/Game/Maps/Reference/FiveLoops_Handmade2_Reference` and
  built `/Game/Maps/FiveLoops_Stairwell_Blockout` as a separate human-scale
  four-floor switchback. Floor surfaces are at Z = 0, 300, 600, 900 and
  1200 cm. Four alternating flights each use seventeen 17.65 cm risers,
  28 cm treads and 170 cm width; invisible ramps provide continuous capsule
  collision while visible steps retain the authored shape.
- Door and circulation correction: floors 1-3 have static closed doors on the
  same side as 4C. The entrance and fourth-floor openings remain open. The
  stairs follow the owner-corrected guardrail direction rather than the earlier
  sketch's outer first run.
- Runtime isolation: the standalone map tags its WorldSettings with
  `Rewind.SkipProofLayout`. `URewindWorldStateSubsystem` honors that opt-out, so
  the C++ Five Loops proof is not generated over the authored map in PIE. The
  existing proof remains the default for maps without the tag.
- Character correction: the imported mannequin is offset to the capsule bottom
  and rotated -90 degrees relative to the character root. Feet now meet the
  collision floor and forward locomotion uses the animation's forward facing.
- Verification: the stable reference and its source each enumerated 26 actors;
  `stairwayCamera` matched at `(17020, -6330, 1360)`, rotation `(0, 180, 0)`.
  The saved target reopened with 33 actors. Standard possessed PIE descended
  all four flights and reported Z = 1298, 998, 698, 398 and 98 cm at the five
  levels; held keys were released before PIE stopped. An MCP viewport capture
  showed the complete four-floor composition. Unreal Editor Win64 Development
  built successfully after the C++ changes.
- Storage evidence: the target map is 441,048 bytes and the reference is
  462,724 bytes. Both map paths resolve Git attributes to LFS for filter, diff
  and merge with text unset. Locally imported Fab, OldWall and PaintedBrickWall
  content was inventoried but remains untracked; REW-0010 neither approves its
  license/provenance nor spends repository storage on it.
- Not verified: no packaged build and no final environment-art pass. Runtime
  vertical camera following, complete authored-map camera coverage, reflections
  and final lighting remain outside the frozen stairwell charter.
- Handoff: merge REW-0010 before claiming its successor. The next bounded task
  should adopt playable camera coverage around the authored stairwell and its
  adjacent upper/ground circulation; a separate decision must move environment
  art off the project brief's non-goal list and approve imported-asset storage.
- Signature: Codex

## 2026-08-24 — REW-0009 complete, rewind is causal rather than global

- Date: 2026-08-24
- Author: Codex
- Task: REW-0009
- Branch: `codex/rew-0009-event-driven-rewind`
- Change: ADR-0009 makes event-driven rewind the default. A loop now ends by
  authored causal-contract failure, player death or successful first-time
  Anchor commit. Contract failure and commit latch a perceptible one-to-three
  second prelude before loop-start apply. Elapsed time still starts at zero and
  drives radio, patrol, turnstile and deliberately timed situations; it no
  longer ends Chapter 1 merely by reaching a global duration.
- Chapter 1 authority: `GroundFuseGate` checks ground-floor power or an
  already active `courtyard_gate_open` Anchor at the no-return threshold.
  Missing both causes the signaled rewind. Carrying the fuse down passes the
  checkpoint and enables the generator. A successful explicit gate-Anchor
  commit then ends that loop, so the next loop can keep the fuse in the
  building socket, use the lift and continue toward Transit. The second socket
  is now named by its authored location, ground floor, rather than by the
  timer-driven implementation's courtyard label.
- Acceptance: FL-02 and FL-03 are amended without renumbering; FL-01 and FL-07
  use the ground-floor power name; FL-17 and FL-18 cover the causal checkpoint,
  prelude bounds and Anchor-commit boundary. Earlier playtests remain evidence
  for the timer-driven builds and their old criteria, not for the amended
  surface.
- Owner blockout inspection: the live saved Handmade level was inspected
  without modification. It contains four large alternating stair flights and
  three owner-placed 35 mm, 16:9 CineCameraActors for 4C, the upper hallway and
  the stairwell. The map remained owner work outside this task and was not
  moved, committed or cited by path from this immutable record.
- Routed work: `docs/backlog/event-driven-rewind-implementation.md` owns the
  proposed C++ checkpoint/prelude/commit work.
  `docs/backlog/handmade-level-adoption.md` owns stabilizing and adopting the
  owner blockout while preserving its layout and camera transforms. The older
  global loop-pressure proposal is resolved by ADR-0009; its measurements
  remain historical.
- Current implementation gap: `URewindLoopSubsystem` still ends automatically
  at 240 seconds or on death, has no causal-contract or Anchor-commit reason,
  and has no latched prelude. The current build therefore does not pass amended
  FL-02 or new FL-17/FL-18.
- Verification: searched live authority and acceptance for conflicting timer
  rules and reviewed every remaining match as history, rejected alternative or
  explicit implementation gap; fifteen changed Markdown files had zero broken
  relative links and zero unclosed fences; ownership reviewed manually;
  `git diff --check` clean; diff-type audit found no C++, header, Blueprint,
  `.uasset`, `.umap`, project or config change.
- Not run: Unreal build, PIE, automation tests and packaged build. REW-0009 was
  frozen as authority-only, and the status documents explicitly do not claim
  the new rule is implemented.
- Handoff: activate the event-driven implementation proposal first. Give the
  owner blockout a stable tracked path before any immutable task cites it, then
  activate level adoption against ADR-0007 and ADR-0009.
- Signature: Codex

## 2026-08-23 — REW-0007 complete, Chapter 1 has its authored spatial chain

- Date: 2026-08-23
- Author: Codex
- Task: REW-0007
- Branch: `rew-0007/chapter-1-spatial-chain`
- Change: completed the chain that REW-0006 could not finish under its
  superseded radio timing. FiveLoops now builds 4C and its common upper
  hallway, a powered six-second cage lift, three physical switchback flights,
  an entrance hallway, one large courtyard, a 170 m service branch folded
  around its patrol yard and Transit Hub. Eleven authored camera regions cover
  that route. The player uses the imported UAL1 mannequin with idle/walk
  animation, and project PIE input gained exact game-time holds, queued input
  sequences and restricted `Rewind.*` console execution.
- Owner construction reference: nine images now live under `docs/concept/`.
  The final paired plans settle top and bottom: stairs, lift and 4C share the
  top corridor; both vertical routes meet one long entrance hallway; its
  fuse-box threshold opens into the large courtyard containing gate, generator,
  patrol area and the route to Transit. These images directed blockout
  placement and cameras without becoming game-rule authority or finished-art
  scope.
- Timing evidence: the formal stairs route reached the entrance in 55.67
  seconds and the powered lift in 6.00, a 49.67-second difference against the
  required 30. Loop B carried the only fuse to the courtyard, opened and
  committed the gate at 211.34 seconds, and timed out at 240 without entering
  Transit Hub. Loop C applied the held gate, left the fuse for the lift and
  entered Transit Hub at 90.67 seconds. The naive/learned difference is
  therefore at least 149.33 seconds, also above the required 30.
- Three-loop evidence: from `Rewind.CleanSave`, Loop A heard `7312` across the
  complete radio sequence and persisted the fact; Loop B spent the fuse on the
  generator and committed `courtyard_gate_open`; Loop C used that held change,
  the same fuse's building use and the lift to reach the hub. The durable
  record is `docs/playtests/chapter-1-three-loops-2026-08-23.md`. Diagnostic
  route-calibration loops between formal B and C are explicitly excluded.
- Build and automation evidence: Unreal Editor Win64 Development built
  successfully under UE 5.8 after the final seam fix. All six discoverable
  `Rewind.*` tests passed together, 6/6 with no errors in 0.0411 seconds. The
  camera enumeration logged all eleven regions complete; baseline logged fuse
  at rest, unpowered lift at 4C, generator offline and gate closed.
- Assets: 31 CC0 Tier 1 `.uasset` files measure 13,220,676 bytes (12.61 MiB)
  under Git LFS: 21 in-place animation assets and both pack mannequins. UAL1
  idle/walk are bound; the rest remain imported, not claimed as integrated.
- Corrections made while playing: opposing camera cuts initially inverted a
  held movement vector, so character input now latches world direction across
  a cut. Stair endpoint rails and the ground-floor seam were opened where the
  capsule route required them. Route automation was moved from MCP-call timing
  to game-time queues so network latency could not become acceptance evidence.
- Verification: successful editor build; formal clean-save A/B/C PIE run;
  measured vertical and full routes; region and baseline enumeration; six
  named automation tests; import size; links and fences reviewed manually;
  `git diff --check` clean. No packaged build was made and frame rate was not
  varied. Finished visual fidelity and enjoyment were not tested and were
  outside the frozen charter.
- Handoff: REW-0007 is archived Complete and `docs/CURRENT_TASK.md` is restored
  from the template. No successor is approved. The next implementation needs
  an owner decision between bounded visual/art-direction work against the
  concept set and a player-experience test of the proven blockout; neither is
  automatically authorized by this task.
- Signature: Codex

## 2026-08-23 — REW-0008 complete, agents can drive and test PIE

- Date: 2026-08-23
- Author: Codex
- Task: REW-0008
- Branch: `rew-0008/editor-automation`
- Change: added the editor-only `RewindEditor` module and registered
  `RewindEditor.RewindPIEInputToolset`. It reads player-zero state and sends
  press, release and tap events through Unreal's simulated player-input path,
  tracks project-injected held keys, supplies release-all, and clears tracking
  when PIE ends. `AutomationTestToolset` is enabled. `.codex/config.toml` now
  gives Codex the same localhost MCP endpoint already present in the committed
  `.mcp.json` files. `AGENTS.md` routes Unreal work to the new canonical
  `docs/EDITOR_AUTOMATION.md` instead of carrying the full machine procedure.
- Build evidence: Unreal Editor Win64 Development succeeded with UE 5.8 after
  compiling and linking `UnrealEditor-RewindEditor.dll`. No engine plugin
  source was changed.
- MCP evidence: a raw streamable-HTTP handshake on `127.0.0.1:8000/mcp`
  negotiated protocol `2025-06-18`. `list_toolsets` returned the project input
  toolset, `EditorAppToolset`, `LogsToolset` and `AutomationTestToolset`.
- PIE evidence: MCP started standard PIE and confirmed it running. Player zero
  possessed `RewindCharacter_0` at `(-150, 0, 98.15)`. Pressing `W` through the
  project toolset and then releasing it moved the pawn 337.90 cm to
  `(-150, 337.90, 98.15)`. The held-key list contained `W` while pressed and
  was empty after release; release-all reported no remaining keys. The same run
  returned a 623,908-character base64 PNG viewport payload, read 58
  `LogRewind` entries and stopped PIE cleanly.
- Automation evidence: discovery reached `ready` and listed four existing
  `Rewind.*` tests. `Rewind.Ids.MatchDesign` ran by name and passed 1/1 in
  0.00695 seconds with zero errors and zero warnings; status and result calls
  both reported the completed pass.
- Verification: successful editor build; live MCP inventory; start, state,
  input, release-all, viewport, log and stop calls; named automation-test run;
  live-document links and fences reviewed; `git diff --check` clean. No
  packaged build was made, and no REW-0007 gameplay acceptance was run. The
  Desktop app's packaged `codex.exe` could not be invoked from PowerShell due
  to WindowsApps access control, so `codex mcp list --json` was not evidence;
  the raw live handshake and tool calls were.
- Handoff: REW-0008 is archived Complete. REW-0007 is restored unchanged in
  its frozen gameplay charter at `Ready`; its tooling blocker is removed. The
  next work remains the lift and stairs, followed by the authored spatial
  chain and its task-owned playtest gates.
- Signature: Codex

## 2026-08-23 — REW-0006 superseded after the played radio rule changed

- Date: 2026-08-23
- Author: Codex
- Task: REW-0006
- Branch: `rew-0006/radio-sequence`
- Change: REW-0006 is archived as `Superseded` by REW-0007. Its frozen scope
  required a 45-second radio sequence repeating every 60 seconds. The played
  implementation exposed that rule as punitive, the design and code moved to
  20 seconds repeating every 50, and the owner confirmed that 20/50 is the
  rule to keep. The task is superseded rather than silently rewriting frozen
  scope. The same change claims REW-0007 and leaves its successor charter in
  `Draft` until that claim lands on `main`.
- Delivered before supersession: a four-channel radio driven by elapsed loop
  time, digits at phases 4, 9, 14 and 19, individual digits kept only in the
  player's memory, and `radio_code_7312` granted after a complete sequence;
  one carried LoopWorld fuse; two exclusive sockets; the generator requiring
  the courtyard socket; the 240-second loop duration authored in settings;
  and the fuse-box labelling decision. The radio work is commits `ec0d2ac`,
  `cb70695`, `d7610af`, `bf09036` and `fd0e3e6`; the fuse work is `4f61bc0`.
- Evidence already present in commit history: `cb70695` records PIE observing
  the 240-second duration, broadcast boundaries, fuse Baseline, both sockets
  and the 4C camera. `bf09036` records a played path from fuse pickup through
  gate Anchor and hub entry and the late-listener radio defect it fixed. No
  durable playtest artifact was added, so those observations are not evidence
  for the amended FL criteria.
- Verification for this supersession wave: authoritative documents and
  relative links reviewed manually; fenced blocks reviewed; `git diff --check`
  clean. No Unreal build, PIE run or new playtest was performed in this wave.
- Not completed: lift, stairs, landing and shaft, spatial rebuild, complete
  camera-region enumeration, animation import, timing measurements and the
  three-loop acceptance run. The existing blockout can still reach the hub
  without the intended lift-or-stairs consequence.
- Handoff: merge this branch so the REW-0007 identity claim lands on `main`.
  Then branch from updated `main`, move REW-0007 from `Draft` to `Ready`, and
  implement the lift and stairs as the first unchecked work. No gameplay
  decision blocks that implementation.
- Signature: Codex

## 2026-08-23 — REW-0005, what an Anchor is worth and the Chapter 1 chain

- Date: 2026-08-23
- Author: Claude
- Task: REW-0005
- Branch: `rew-0005/anchor-consequence`
- Change: rules only, no implementation. `ADR-0008` states why an Anchor
  exists: holding an override must release something the loop would
  otherwise commit, and that thing must have another use. Name the
  resource, name its two uses, or it is a shortcut. Scarcity becomes a
  design rule with its own argument rather than a consequence of the first
  proof's scope. `anchors.md` restates both. `chapter-1-authored.md` gains
  the chain: a radio that broadcasts on the loop clock and costs 45 seconds
  of a 240-second loop, one carried fuse with two exclusive sockets, a lift
  that runs only while the fuse is in the building socket, stairs that cost
  at least one turnstile period more, and the three loops stated as shapes
  the space must support.
- Verification: every new rule checked against ADR-0002, and the only
  loop-count mentions in the design documents are the ones forbidding it.
  Ownership checked by search: the numbers live in one document each.
  Manual link and fence review. `git diff --check` clean. Nothing was built
  and nothing was played, so nothing here is evidence about a build.
- Finding that changed a decision already made: `courtyard_gate_open` does
  not pass ADR-0008's test as the chain is implemented today. Holding the
  gate removes two steps and leaves every route available, only faster, so
  there is no contested resource to name. It passes under the chain this
  task wrote. That is a design gap rather than a defect in ADR-0003 or in
  the implementation, and it is what the Five Loops playtest measured
  without being able to say so.
- Second finding: FL-13 became unsatisfiable. It required the whole chain
  inside one loop, and the chain now makes that impossible on purpose,
  because carrying the fuse to the courtyard kills the lift. It was amended
  with its identifier intact, along with FL-07's wording, and the
  amendments are recorded in the acceptance document. The existing playtest
  evidence stays true of the criteria as they read before, and is not
  evidence for these.
- Did not do: any implementation. No C++, no geometry, no assets. The rules
  are deliberately ahead of the build, and `docs/CURRENT_STATUS.md` says so.
- Handoff: REW-0004 is restored to `docs/CURRENT_TASK.md` from
  `docs/paused/`, its resume condition met. **Its frozen out-of-scope list
  now conflicts with what this task decided**: it excludes branching routes,
  and the lift-or-stairs choice is one. A frozen charter is superseded
  rather than rewritten, so the next actor should decide whether to
  supersede REW-0004 before resuming it. That is recorded in its handoff and
  is not resolved here.
- Signature: Claude

## 2026-08-23 — REW-0003 complete, all sixteen FL criteria evidenced

- Date: 2026-08-23
- Author: Claude
- Task: REW-0003, re-assigned from Grok on 2026-08-23
- Branch: `rew-0003/run-evidence`
- Change: the Five Loops Test is demonstrated. FL-01 to FL-16 each have
  named evidence in `docs/playtests/five-loops-2026-08-22.md`, a new
  collection created because `docs/acceptance/` owns criteria and mixing
  evidence into it would break that ownership. Four defects were fixed
  first: exposure was never authored against a 50000 lux sun; the
  outdoor run was three slabs in a void with 20 cm and 30 cm gaps at the
  two doorways; the patrol teleported between two poses behind a barrier
  covering 200 cm of a 760 cm corridor; and the courtyard gate and the
  turnstile did not span the corridor either, at 400 cm and 450 cm. A
  `LogRewind` run log was added so evidence is read from stamped lines
  rather than transcribed from screenshots, and `.mcp.json` points at
  the editor's MCP endpoint, which drove the idle-pair run.
- Verification: four runs, all on editor Win64 Development builds, with
  raw logs committed beside the record. A two-loop idle run with no
  player input; a played run from a clean session; a scripted sequence
  covering clean save and a baseline against two learned runs; and a
  full editor quit and reopen. `git diff --check` clean, links and
  fences reviewed by hand.
- Finding that matters more than the pass: FL-14 passes on its wording
  and its number is close to meaningless. Every turnstile crossing in
  every run landed between `t = 30.24` and `t = 30.93`, and the
  turnstile opens at `t = 30`. The player arrived early and waited every
  time, including on the baseline run that did the whole chain. One
  learned run finished at `t = 8.37` and stood at the turnstile for
  twenty-two seconds. The 0.69 s difference measures the gate cycle, not
  the player. The loop has no time pressure, and the record says so
  plainly.
- Second finding: three criteria were undemonstrable before this wave
  and nobody knew, because a closed gate and a closed turnstile could
  both be walked around. Any earlier impression of reaching the hub
  through the turnstile is void. It was caught by measuring geometry
  rather than by playing, which is what "Evidence Discipline" is for.
- Did not run: a packaged build; frame rate was not varied, so the
  clock's independence from frame rate is evidence at one rate only;
  FL-03, FL-08 and FL-12 hold to tick resolution of about 0.25 s rather
  than to exact equality at an arbitrary `t`, because transitions are
  logged on the tick that observes them. The offsets are constant inside
  each loop, which is tick phase and not drift.
- Routed, not built: three proposals in `docs/backlog/` — art direction,
  loop pressure and interaction cost, and traversal as a knowledge axis.
  None is authority. The third argues the proof can pass FL-14 without
  representing spatial knowledge, and the FL-14 result above is that
  argument arriving as data.
- Handoff: `docs/CURRENT_TASK.md` is restored from the template and no
  task is active. The central product question is now open in a way it
  was not before: the loop is proven deterministic and knowledge does
  persist, and the same evidence shows the space is too small for the
  timer to mean anything. What follows is a product decision, not a bug
  fix. The branch is not merged to `main`.
- Signature: Claude

## 2026-08-20 — REW-0003 overnight handoff

- Date: 2026-08-20
- Author: Grok
- Task: REW-0003
- Branch: `main`
- Change: Five Loops Test implementation is In Progress. Unreal 5.8
  project, loop clock, apply order, session save, CleanSave, 4C blockout
  and puzzle actors exist. Lighting intensity no longer calls
  `SetLightUnits`. Loop subsystem draws elapsed `t=` on screen. One
  editor play opened the 4C door, used the radio, started the generator
  and opened the courtyard gate.
- Verification: RewindEditor compiled earlier this wave. PIE ran
  `/Game/Maps/FiveLoops` with `RewindGameMode`. Player report is the
  evidence for the 4C chain. `t=` overlay is in source and was not in
  that play.
- Did not run: FL-01 to FL-16 from `Rewind.CleanSave`. Idle-loop match
  and patrol clock at a stated `t`. Anchor commit, turnstile, hub reach,
  faster second run, quit-and-load. Packaged build.
- Handoff: charter stays in `docs/CURRENT_TASK.md`. Resume by closing
  the editor, compiling RewindEditor, PIE, `Rewind.CleanSave`, then the
  remaining FL script. Nothing is blocked. Stopped for rest.
- Signature: Grok

## 2026-08-19 — REW-0002 Five Loops Test authority

- Date: 2026-08-19
- Author: Grok
- Task: REW-0002
- Branch: `dev/grok-01`
- Change: ADR-0002, ADR-0003 and ADR-0004 are Accepted. Design documents
  own loop and determinism, Anchors, world-state apply order, save and
  session, and the authored facts the first proof needs. Acceptance
  criteria FL-01 to FL-16 are in
  `docs/acceptance/five-loops-test.md`. Insight is not in this proof.
  Echo, asset storage and the license remain open.
- Verification: each FL criterion names one owner; design ownership lines
  do not overlap; live-doc links and fences checked excluding
  `docs/baseline/` and imported legacy files; `git diff --check` clean.
- Did not run: Unreal, Unity, playtest, build. No implementation exists.
- Handoff: `docs/CURRENT_TASK.md` is restored from the template. Next
  work is an implementation task against FL-01 to FL-16. Asset storage
  still blocks the first `.uasset`. This working tree is not committed.
- Signature: Grok

## 2026-08-19 — REW-0001 canonical project state

- Date: 2026-08-19
- Author: Grok
- Task: REW-0001
- Branch: `main`
- Change: this repository is now a RE:WIND docs-first instance rather than a
  leftover protocol-extraction tree. DFC-0001 is cancelled and archived.
  Live documents that still described the protocol were restated. The Unity
  project at `C:\code\reWiND` commit `9f959140e25163bddef390979f23f352e8f0c2f6`
  was inventoried from source into
  `docs/concepts_sandbox/legacy-rewind/code-inventory.md`. Twelve design
  contradictions are registered in
  `docs/concepts_sandbox/legacy-rewind/design-conflicts.md`. The Five Loops
  Test is proposed, not activated, in `docs/backlog/five-loops-test.md`.
  `.gitignore` exists for an Unreal project that does not yet exist.
- Verification: relative links and fenced blocks checked on twenty-four live
  documents; `git diff --check` clean; Unity managers, Chapter 1 scripts and
  `Chapter1_Blockout.unity` GUIDs read; Unreal Engine 5.8 present at
  `C:\Program Files\Epic Games\UE_5.8`. Unity Editor was not launched. No
  Unreal project was created. `docs/baseline/` was not edited and its broken
  internal links were not repaired.
- Finding that was not in the recovery notes: TimeManager, KnowledgeManager
  and AnchorManager are not instantiated in the committed Unity scene, so
  `LoopStartEvent` is never published there.
- Exception: REW-0001 was completed without merging the identity claim to
  `main` first. The charter recorded that exception while still editable.
- Handoff: `docs/CURRENT_TASK.md` is restored from the template. Next work is
  not activated. The candidate is `docs/backlog/five-loops-test.md`, which
  depends on open decisions in `docs/PROJECT_BRIEF.md`. This working tree is
  not committed.
- Signature: Grok

## 2026-08-19 — Repository bootstrap from a frozen baseline

- Date: 2026-08-19
- Author: Claude
- Task: bootstrap, performed under `ACME-0173` in the source repository. This
  repository's own task numbering starts at DFC-0001, which is chartered but not
  started.
- Branch: `main`
- Change: this repository now exists as a docs-first instance running the model
  it intends to specify. `baseline/acme-2026-08-19/` holds fifteen files copied
  verbatim from tag `protocol-baseline-2026-08-19`
  (`75e4b5ee72201d02ad57f22b1a5fcfb3244d521e`) in `zackemannen81/acme-engine`,
  with provenance in `baseline/README.md`. `extraction/ledger.md` classifies
  twenty-eight rule groups as CORE, PROFILE or PROJECT.
- Verification: every copied file was compared by SHA-256 against
  `git show <tag>:<path>` at extraction. All fifteen matched. The comparison is
  repeatable against the tag, which is why the tag exists.
- Not copied, deliberately: the source repository's active charter, because it
  holds another contributor's in-progress work; `docs/JOURNAL.md`, because 6500
  lines of client, product and personal material must never be copied raw and
  journal evidence belongs to the evidence milestone, aggregated and anonymized;
  and the source project's status, architecture, brief and decisions, because
  the model is the workflow rather than the product it was used on.
- Identity prefix: `DFC`, encoding the descriptive method rather than a brand.
  The project name is undecided, and an identity carrying the name would need a
  rename that the addressing rule forbids.
- Honesty note: four rules in the baseline are hours old, not months.
  Path stability, collection discoverability, tense-aware citation validation
  and trunk identity claims were each added on 2026-08-19 after a real failure
  in the source repository. `extraction/ledger.md` marks them so that the
  evidence report does not treat them as equally proven.
- Handoff: DFC-0001 is chartered in `docs/CURRENT_TASK.md` as `Draft` and
  unassigned. It writes `SPEC.md` from the CORE rows of the ledger. Claim the
  identity on `main` before freezing it. This repository is private and
  unlicensed, and is therefore not open source.
- Signature: Claude
