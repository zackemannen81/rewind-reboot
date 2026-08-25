# Current Status

Reality as of 2026-08-26. This document describes what exists, not what is
planned. If it disagrees with the repository or the build, this document is
wrong and must be corrected.

## What exists in this repository

| Thing | State |
| --- | --- |
| The docs-first instance | Operating: entry point, workflow, brief, status, system document, journal, file map, identity register, collection indexes. REW-0001 archived |
| `docs/adr/ADR-0001_engine-unreal-engine-5.md` | Accepted. Unreal Engine 5.8 |
| `docs/adr/ADR-0002_world-determinism.md` | Accepted. World does not learn the player |
| `docs/adr/ADR-0003_anchor-semantics.md` | Accepted. One player-chosen gate override. No Insight in this proof |
| `docs/adr/ADR-0004_save-and-session.md` | Accepted. Knowledge and Anchors persist. Clean save is reachable |
| `docs/adr/ADR-0005_asset-storage.md` | Accepted. Unreal binaries via Git LFS. REW-0007 adds the first content import: 31 Tier 1 character `.uasset` files, 12.61 MiB measured |
| `docs/adr/ADR-0006_cpp-and-blueprint.md` | Accepted. C++ owns FL systems; Blueprint may place and present |
| `docs/adr/ADR-0007_camera-and-perspective.md` | Accepted and implemented. Camera is authored, follows inside a composition, player does not aim it |
| `docs/adr/ADR-0008_what-an-anchor-is-worth.md` | Accepted and implemented in the Chapter 1 chain. Holding `courtyard_gate_open` leaves the only fuse in the building socket, releasing both the six-second lift route and immediate access to the main route |
| `docs/adr/ADR-0009_event-driven-loop-termination.md` | Accepted and implemented by REW-0017. Causal-contract failure, death and successful Anchor commit are the default loop-end classes; contract failure and commit run a one-to-three-second loop-clocked prelude |
| `docs/adr/ADR-0011_asset-source-and-provenance.md` | Accepted, amended 2026-08-25. Art assets are project-owned or generated; third-party packs stay outside the tracked closure. The amendment corrects the clause that named `/Game/Art/Texture/`: those textures are the owner's own work, and their JPEG sources are tracked at `Rewind/ArtSource/Textures/`, 76 KB. REW-0022 unbound the map from them along with the Fab packs; REW-0025 re-binds them |
| `docs/adr/ADR-0012_echo-semantics.md` | Accepted. Echo is narrative only: nothing samples the player's transform, nothing persists a path, nothing replays a ghost |
| `docs/adr/ADR-0010_renderer-configuration.md` | Accepted. Lumen GI and Lumen reflections, virtual shadow maps, mesh distance fields and default bloom. Auto-exposure stays off. Hardware ray tracing stays off. Frame-time budget 16.67 ms at 1920×1080 on the named development machine |
| `Rewind/Rewind.uproject` | Unreal Engine 5.8 C++ project. Loop clock, apply order, session save, CleanSave. Loops end on causal-contract failure, death or first-time Anchor commit. The 240-second timeout is no longer the default end condition. `DefaultEngine.ini` matches ADR-0010 |
| `Rewind/Content/Maps/FiveLoops.umap` | Preserved procedural proof map, no longer the editor or game default. C++ builds 4C, common hallway, cage lift, proof stairs, courtyard/service route and Transit Hub under eleven camera regions |
| `Rewind/Content/Maps/FiveLoops_Stairwell_Blockout.umap` | Editor and game default. The accepted stairwell leads to one common corridor whose three preserved openings are stairs, lift and 4C. REW-0013 corrected the building side. REW-0014 applied the owner's placed 35 mm `4c_camera` as the exact Apartment4C runtime start frame: `(750, 1330, 1330)`, rotation `(0, 180, 0)`, horizontal FOV `37.497356`. REW-0015 widened that region's player volume to cover the room it frames, leaving the frame itself unchanged. REW-0022 unbound the saved map from untracked `/Game/Fab/` and `/Game/Art/Texture/` content: six empty-mesh dressing actors were removed, and surfaces that had lost those materials now use the tracked project-owned instances under `Rewind/Content/Art/Materials/`. Apartment 4C is undressed blockout. REW-0020 spawns `ARewindAuthoredCourtyard` on this SkipProofLayout map: approach hall, plaza, generator, gate, Transit exit and `GroundFuseGate` at the hall-to-courtyard seam `(-430, 2700, 150)`, with nine camera regions. Gameplay actors of the building slice were not moved |
| `Rewind/Content/Maps/Reference/FiveLoops_Handmade2_Reference.umap` | Stable untouched Git LFS reference copy. Its 26-actor inventory and stair camera transform match the owner source at REW-0010 completion |
| `Rewind/Content/FiveLoops_Handmade.umap` and `FiveLoops_Handmade2.umap` | Tracked owner-authored construction maps preserved through Git LFS. They are spatial source material, not runtime defaults or design-rule authority |
| `docs/design/` | Nine accepted documents. Ownership in `docs/design/README.md`; `player-messages.md` owns the player-facing text channel and states that the debug overlay is not UI; `tutorial-and-first-run.md` owns first-run copy, the naming rule, and first-time gating |
| `docs/acceptance/five-loops-test.md` | Accepted. Criteria FL-01 to FL-18. Existing playtests remain evidence for their timer-driven builds and pre-2026-08-24 wording; no build evidence exists for amended FL-01, FL-02, FL-03 or FL-07, or for FL-18. REW-0020 recorded PIE evidence for the three FL-17 checkpoint outcomes on the authored map |
| `docs/playtests/` | Evidence from named runs of named builds. The earlier complete Five Loops record remains; REW-0007 adds a Chapter 1 three-loop record; REW-0019 adds before/after 4C renderer frames and 1080p frame time; REW-0020 adds the authored-courtyard FL-17 record |
| `.codex/config.toml` and `.mcp.json` | Project-scoped clients for the running editor's MCP endpoint on localhost. They resolve only while this project's editor is open |
| `docs/EDITOR_AUTOMATION.md` | Canonical engine, editor, MCP, plugin, toolset, build and agent-playtest procedure |
| `docs/concepts_sandbox/legacy-rewind/` | Imported design, roadmaps and task files from the previous project, plus a verified code inventory and a conflict register. Non-authority |
| `docs/baseline/acme-2026-08-19/` | Frozen provenance for the working model itself. Never edited, never authority |
| `.gitignore` / `.gitattributes` | Ignore generated UE output. LFS tracks Unreal binaries, including the Tier 1 character assets |
| `docs/CURRENT_TASK.md` | Restored task template. REW-0016 to REW-0024 are complete and archived; no task is active on this branch |
| `docs/concept/` | Nine owner-produced target and construction-reference images: 4C, fuse box, stairwell, lift, three circulation/interaction sketches and the settled top- and ground-floor plans. Targets and blockout clarification, never game rules |
| `docs/finished/REW-0004_...md` | Superseded by REW-0006 after its frozen scope conflicted with the lift-or-stairs branch decided by REW-0005 |
| `Rewind/Source/Rewind/RewindCameraRig.cpp` | The authored camera of ADR-0007. Regions declare rotation, X/Y/Z travel axis, bounds, dead zone, player volume and explicit FOV. Half-open volumes give shared thresholds exactly one owner. The procedural proof has eleven regions; the authored building slice plus courtyard has nine |
| `Rewind/Content/Art/Materials/Stairwell/` | Two project-owned procedural masters and six material instances: upper and lower walls, floor, metal, door and near-black player silhouette. They are a first-pass presentation grammar and introduce no third-party dependency |
| `Rewind/Content/Art/Materials/BuildingSlice/` | Project-owned 4C floor instance with restrained reflection, plus `M_4CSignEmissive` for the 4C sign panel. Both depend only on project-owned material work; no third-party environment pack |
| `Rewind/ArtSource/Textures/EnvironmentTextureKit-v1/` | Twelve project-generated quality-94 JPEG import sources from RE:WIND Environment Texture Kit v1, with a dated provenance record. They are retained outside `Content/` so `/Game/Art/Textures/Surfaces` can be rebuilt without the owner's desktop staging directory |
| `Rewind/Content/Art/Textures/Surfaces/` | Twelve project-generated base-colour textures: eight tileable surfaces and four clamp-addressed backdrop/poster textures. REW-0027 read back sRGB enabled and `MaxTextureSize=1024` for every asset; tileable surfaces use Wrap and the backdrop/poster assets use Clamp |
| `Rewind/Content/Art/Materials/Surfaces/` | `M_REW_Surface` plus eight REW-0027 instances: plaster grey/green, dark brick, courtyard paving, interior floor tile, ivory ceramic tile, painted black metal and worn dark wood. Each instance has its base texture and world TileSize set; no map uses them yet |
| `Rewind/Source/Rewind/RewindRadio.cpp` | Four channels on the loop clock. The accepted channel speaks `7312` across 20 seconds at phases 4, 9, 14 and 19 of a 50-second cycle. A full sequence grants the stored fact; individual digits remain player memory |
| `Rewind/Source/Rewind/RewindFuse.cpp` and `RewindFuseSocket.cpp` | One carried LoopWorld fuse with two exclusive sockets. The courtyard socket enables the generator; the building socket powers the lift. The lift refuses an empty building socket |
| `Rewind/Source/Rewind/RewindLift.cpp` and `RewindStairwell.cpp` | A physical bidirectional six-second cage journey between floor 4 and entrance, plus the always-available stair routes. The cage has a blocking floor, centres its passenger and applies a configurable local-space hall exit offset, so the rotated authored instance hands the capsule to X `-280` on both landings while the procedural proof retains its default hand. Final REW-0013 PIE measured 6.00 s down and 6.00 s up |
| `Rewind/Content/Characters/Returner/` | The owner-generated 24-bone Returner mesh, skeleton and four animations. REW-0029 binds `/Game/Characters/Returner/Returner` to the runtime player, with `A_Returner_Alert` as idle and `A_Returner_Walk` as walk. Editor bounds and reference-pose data establish the Z −96 cm, yaw −90° presentation transform; no PIE visual frame was captured on this agent session |
| `Rewind/Content/Characters/Tier1/` | CC0 Quaternius Tier 1: 21 in-place animations and both pack mannequins, 31 assets and 12.61 MiB. The assets remain available but are no longer the runtime player binding |
| `Rewind/Source/RewindEditor/` | Editor-only PIE bridge: input state, tap, held keys, exact game-time holds, queued measured sequences, active camera region/axis/transform/FOV, clean game-viewport PNG capture, restricted `Rewind.*` console calls and release-all through simulated Unreal input |
| `AutomationTestToolset` | Enabled. Twenty-two `Rewind.*` tests exist after REW-0020: the nineteen inherited, plus `Rewind.AuthoredCourtyard.SpatialConstants`, `Rewind.AuthoredCourtyard.RegionAbutment` and `Rewind.AuthoredCourtyard.SkipProofLayoutSpawnsCheckpoint`. Run together headless on the REW-0029 branch on 2026-08-26: 22 passed, 0 failed, `TEST COMPLETE. EXIT CODE: 0` |
| `docs/finished/REW-0007_...md` | Complete. The rebuilt Chapter 1 chain, timing construction, eleven cameras, Tier 1 import and three-loop evidence are archived |
| `docs/finished/REW-0008_...md` | Complete. Canonical editor/MCP context, project-scoped Codex config, PIE-input toolset and named automation-test execution |
| `docs/finished/REW-0009_...md` | Complete. Event-driven loop termination accepted; product, design and FL authority amended; implementation and owner-level adoption routed separately |
| `docs/finished/REW-0012_...md` | Complete. The authored default map extends the accepted stairwell through 4C, common halls and a bidirectional powered lift under seven camera regions |
| `docs/finished/REW-0013_...md` | Complete. Corrected REW-0012 to the owner's three-opening corridor, removed primitive furniture and wrong-side duplicates, and verified 4C, hall, stairs and powered bidirectional lift handoffs |
| `docs/finished/REW-0014_...md` | Complete. The Apartment4C runtime region now starts from the owner's preserved 35 mm `4c_camera` transform and FOV; clean PIE matched all values exactly |
| `docs/finished/REW-0015_...md` | Complete. Movement no longer drops input outside a region, the player-volume clamp is consistent with region containment, and the Apartment4C volume covers its room |
| `docs/finished/REW-0016_...md` | Complete. Operator task: chartered, delegated, reviewed and merged REW-0017 to REW-0019 through the owner's `powershell-agent-mcp`, one clone per agent |
| `docs/finished/REW-0017_...md` | Complete. Event-driven rewind: causal-checkpoint latch, loop-clocked prelude, Anchor-commit end reason, interference signature; automatic 240-second timeout removed |
| `docs/finished/REW-0018_...md` | Complete. Player-facing text in the authored slice comes from the catalog and message subsystem, not `AddOnScreenDebugMessage` |
| `docs/finished/REW-0019_...md` | Complete. ADR-0010 renderer configuration applied; first look pass; before/after 4C frames and 1080p frame time |
| `docs/finished/REW-0023_...md` | Complete. First-run copy on the message system; naming rule and first-time gating owned by `tutorial-and-first-run.md` |
| `docs/finished/REW-0020_...md` | Complete. Authored courtyard blockout, GroundFuseGate at the hall-to-courtyard seam, three FL-17 PIE outcomes |
| `Rewind/Source/Rewind/RewindMessageSubsystem.cpp` | Player-facing text. Actors speak by catalog id; copy lives in `FRewindMessageCatalog`; one Slate line sits at the bottom of the viewport. First-run copy and first-time gating sit on that channel: a gated line is recorded as the knowledge fact `seen.<MessageId>` on `URewindSessionSubsystem` and is not shown again in the same session. The engine debug overlay is not this channel. `RewindLoopSubsystem` still draws `t=` |
| `docs/finished/REW-0010_...md` | Complete on its task branch. Human-scale four-floor stairwell, stable owner reference, traversal evidence and procedural-layout opt-out |
| `docs/finished/REW-0011_...md` | Complete. Isolated stairwell camera coverage, bidirectional route, first-pass material/light grammar and clean PIE visual evidence |
| `docs/finished/REW-0006_...md` | Superseded after play established 20/50 as the radio rule while its frozen scope still required 45/60. Radio and fuse work retained for REW-0007 |
| `docs/finished/REW-0003_five-loops-test-implementation.md` | Complete. Five Loops Test implementation, archived 2026-08-23 |
| `docs/backlog/five-loops-test.md` | Authority resolved by REW-0002. Implementation activated as REW-0003 |

## What does not exist

- **PIE evidence for the remaining amended Five Loops criteria.** REW-0020
  recorded PIE evidence for the three FL-17 checkpoint outcomes on the
  authored map. REW-0017 implements event-driven rewind in C++ and named
  automation tests; it did not run a first-time commit. Existing playtests
  remain evidence for their timer-driven builds. No build evidence exists
  yet for amended FL-01, FL-02, FL-03 or FL-07, or for FL-18.
- **Finished environment art.** The complete Chapter 1 gameplay chain remains
  measured C++ blockout geometry. The authored slice now renders under Lumen
  GI, Lumen reflections, virtual shadow maps, mesh distance fields and bloom,
  with a first light-shaping pass so the accepted grammar can be judged. That
  is a renderer and look pass, not final art. Procedural plaster, basic
  fixtures and primitive geometry remain. Authored decals, final models,
  props, rain, neon treatment and wider-map dressing are not integrated.
  Locally imported Fab/environment packs and `Rewind/Content/Art/Texture/`
  remain untracked and local under ADR-0011. REW-0022 removed the saved map's
  references to them. A fresh clone of the default map therefore opens without
  those packages. Apartment 4C is undressed until generated or authored props
  exist; that is accepted by ADR-0011, not a defect to work around.
- **A packaged game build.** Editor Win64 Development compiled and run in PIE;
  a cooked package has not been made, so nothing here is evidence about a
  shipped build.
- **Frame-rate independence of the loop clock.** Every run was at one frame
  rate. The zero drift measured across 420 seconds is evidence at that rate.
- **Any answer to whether the loop is enjoyable.** The Five Loops Test excludes
  enjoyment by design. What the evidence does show is that the loop has no time
  pressure: every turnstile crossing in every run landed within 0.7 s of the
  gate opening at `t = 30`, because the player always arrived early and waited.
  FL-14 passes on its wording, and its number is not evidence that knowledge
  makes a player faster. See
  [`five-loops-2026-08-22.md`](playtests/five-loops-2026-08-22.md) and
  [`loop-pressure-and-interaction.md`](backlog/loop-pressure-and-interaction.md).
- **Echo as a replayed ghost.** ADR-0012 decided Echo is narrative only, so
  this will not be built. Nothing samples the player's transform and nothing
  persists a path. Insight as a later system and Chapters 2 to 5 remain
  unwritten and are not required by the first product proof.
- **A license decision.** A `LICENSE` file with Apache-2.0 text remains from
  the docs-first extraction. That is not a decision that RE:WIND is open
  source. The intended safe default for the game is all rights reserved, and
  that default is itself not yet a recorded decision.

## What exists outside this repository

`C:\code\reWiND` holds the previous Unity project at commit
`9f959140e25163bddef390979f23f352e8f0c2f6` (2025-10-24), clean, untouched by
this reboot. It is source material. It is not a dependency, and this
repository does not read from it at runtime or at build time.

Its implementation was inventoried against the source, file by file. The
verified result is in
[`code-inventory.md`](concepts_sandbox/legacy-rewind/code-inventory.md).
Unity Editor was not launched. The summary that matters:

```text
TimeManager, KnowledgeManager, AnchorManager
- classes exist
- none of them is referenced from Chapter1_Blockout.unity or from bootstrap
- EditorBuildSettings has no scenes
- LoopStartEvent is published only by TimeManager, which is not in the scene

TimeManager (as written)
- a 7-minute countdown exists in the class
- ResetLoop only calls StartLoop
- the comment in ResetLoop says world state would be reset and is not

KnowledgeManager (as written)
- a string dictionary serialised to PlayerPrefs
- no clear, so a clean save is not achievable from inside the class

AnchorManager (as written)
- a string set serialised to PlayerPrefs
- exactly one identifier exists in the codebase
- exactly one object reads it
- canonical world-state override is not implemented

EchoSystem (as written)
- transform sampling exists, at 0.1 s
- EchoPlayback is never called
- a recording is never saved; the code says so in a comment
- Chapter1EchoVisualizer builds a sphere trail at loop end

Chapter 1
- bootstrap builds a blockout and wires radio, fuse, generator, gate, turnstile, two patrols
- orchestrator resets those five objects on LoopStartEvent
- patrols have no loop-start hook
```

Two findings go beyond class-level reading and matter for the reboot:

- **World reset was never centralised.** Each listed Chapter 1 object resets
  itself if the orchestrator calls it. There is no world-state model, no reset
  order, and no way to know whether a reset was complete.
- **The patrol was not reproducible in source.** `Chapter1DronePatrol` moves
  with `Time.deltaTime`, hovers on global `Time.time`, and has no loop-start
  hook. Its waypoint index and alert state carry across whatever a loop
  boundary would be. A player could not have learned a stable pattern from
  that script.

A third finding is stronger than the earlier recovery notes:

- **The committed scene does not instantiate the managers.** Null-conditional
  calls on `KnowledgeManager.Instance` and `AnchorManager.Instance` therefore
  no-op, and `LoopStartEvent` is never published by anything in that scene.

That second finding is still the important one for the product. Reproducible
patrol behaviour is required by the first product proof as currently proposed,
and the legacy implementation did not provide it in any form. It is new work
in any engine.

## Legacy status claims that are false

The previous project's task files record the following. None of it may be
carried forward as a starting position.

| Legacy claim | Reality |
| --- | --- |
| `SYS-001` core systems: `DONE` | Three manager classes exist and are not in the committed scene. World reset, anchor application and Echo replay do not exist |
| Acceptance: "a `TimeManager` that can start, end and reset the loop" | The class restarts a timer. Nothing in the world resets with it. The class is not placed in the committed scene |
| Acceptance: "an `AnchorManager` that allows specific world states to be locked" | The class stores strings. One gate checks one hardcoded string, if the instance exists |
| Acceptance: "a basic `EchoSystem` that can record and playback player movements" | Both classes exist. Nothing connects them, and nothing persists a recording |
| `PC-001`: `DONE` | A substantial third-person controller class exists. It was not playtested for this inventory |
| `ART-001`: `Ready for Review`, then reopened | 140 MB of assets exist in the Unity project. They are Unity assets, and the engine decision has changed |

The failure was not that the work was bad. It was that the status surface
could not be trusted, so nobody could tell which parts were real. That is the
specific thing `AGENTS.md` "Evidence Discipline" exists to prevent.

## Known gaps and risks

- **FL-18 still has no PIE evidence.** REW-0017 implements Anchor-commit as a
  loop-end reason in C++ and named tests. REW-0020 made `GroundFuseGate`
  reachable on the authored map and recorded FL-17. A first-time
  `courtyard_gate_open` commit was not played. The ground-floor socket is
  still named Courtyard in C++. The 4C fuse at X `-250` still sits 5 cm
  outside `Apartment4C_Region`, which blocked this-loop pickup in the
  REW-0020 power PIE.
- **The rebuilt chain answers the timing defect, not enjoyment.** The formal
  REW-0007 run measured the naive Loop B at the 240-second timeout without hub
  entry and the held-Anchor Loop C at hub entry by 90.67 seconds, at least
  149.33 seconds apart. The earlier tiny-space finding is therefore superseded
  for this blockout. Whether traversing and repeating this space is enjoyable
  remains untested and is not an FL criterion.
- **Legacy material still contradicts the decided determinism rule.** ADR-0002
  chose a world that does not learn the player. The imported GDD still
  contains both that game and a game whose details shift. The sandbox is
  not authority; the contradiction stays there as C1.
- **The engine change discards the existing implementation.** ADR-0001 accepts
  this. The legacy C# is design reference from now on, and the cost is
  contained because the systems that mattered were not running in the
  committed scene anyway.
- **Documentation verification is still manual.** There is no link checker or
  CI gate. Unreal Editor Win64 Development builds locally, and MCP can drive
  PIE input and named automation tests, but every task still names exactly
  what it ran and what it did not.
- **The instrumentation resolves to a tick, not an instant.** `LogRewind`
  writes a transition on the tick that first observes it, so two loops report
  the same boundary up to about 0.25 s apart. The offset is constant inside a
  loop, which is tick phase and not drift. Any criterion whose evidence needs
  exact equality at an arbitrary `t` is not closed by the log alone.
- **The editor still cannot be driven and rebuilt at the same time.** The MCP
  endpoint lives inside the editor, and the editor holds the module build
  lock. REW-0008 added project-owned PIE input and enabled automation-test
  discovery/execution, so player-action evidence no longer requires a human
  at the keyboard. Agents still need separate build and live-editor phases and
  a session started from the repo root so the MCP config is loaded.
- **Git LFS quota.** ADR-0005 put binaries in LFS. The Five Loops Test must
  stay a blockout. A large art import is a new decision.
- **The `LICENSE` file disagrees with the intended default.** Apache-2.0 text
  is in the tree from the model extraction. The product license is an open
  decision. No task may treat either text as the decision.
- **Links inside `docs/baseline/` do not resolve.** The baseline is a partial
  verbatim copy whose links point into its own source repository. This is
  correct and must not be repaired. Any future link checker excludes it.

## Boundaries that hold

- `docs/baseline/` and imported files under
  `docs/concepts_sandbox/legacy-rewind/` are never edited.
- Legacy material is never authority. It becomes true only by being restated
  in an owning document through an activated task or a decision.
- `C:\code\reWiND` is read-only source material. The relation is one way, and
  this repository never writes to it.
- This repository is private, and no license has been granted for RE:WIND.
