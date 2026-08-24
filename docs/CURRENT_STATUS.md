# Current Status

Reality as of 2026-08-24. This document describes what exists, not what is
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
| `docs/adr/ADR-0009_event-driven-loop-termination.md` | Accepted, not implemented. Causal-contract failure, death and successful Anchor commit replace the global timer as default loop-end classes; contract failure and commit require a one-to-three-second prelude |
| `Rewind/Rewind.uproject` | Unreal Engine 5.8 C++ project. Loop clock, apply order, session save, CleanSave. The running loop still ends automatically at 240 seconds or on death and therefore does not yet comply with ADR-0009 |
| `Rewind/Content/Maps/FiveLoops.umap` | Preserved procedural proof map, no longer the editor or game default. C++ builds 4C, common hallway, cage lift, proof stairs, courtyard/service route and Transit Hub under eleven camera regions |
| `Rewind/Content/Maps/FiveLoops_Stairwell_Blockout.umap` | Editor and game default. The accepted stairwell leads to one common corridor whose three preserved openings are stairs, lift and 4C. REW-0013 removed REW-0012's wrong-side duplicate, enclosed the existing 4C footprint at 300 cm height, aligned the shaft to the middle opening and relocated gameplay, lighting and four extension camera regions to those authored spaces |
| `Rewind/Content/Maps/Reference/FiveLoops_Handmade2_Reference.umap` | Stable untouched Git LFS reference copy. Its 26-actor inventory and stair camera transform match the owner source at REW-0010 completion |
| `Rewind/Content/FiveLoops_Handmade.umap` and `FiveLoops_Handmade2.umap` | Tracked owner-authored construction maps preserved through Git LFS. They are spatial source material, not runtime defaults or design-rule authority |
| `docs/design/` | Seven accepted documents. Ownership in `docs/design/README.md`; the added stairwell owner states the bounded first-pass visual grammar without claiming final art |
| `docs/acceptance/five-loops-test.md` | Accepted. Criteria FL-01 to FL-18. Existing playtests remain evidence for their timer-driven builds and pre-2026-08-24 wording; no build evidence exists for amended FL-01, FL-02, FL-03 or FL-07, or for new FL-17 and FL-18 |
| `docs/playtests/` | Evidence from named runs of named builds. The earlier complete Five Loops record remains; REW-0007 adds a Chapter 1 three-loop record for the rebuilt chain |
| `.codex/config.toml` and `.mcp.json` | Project-scoped clients for the running editor's MCP endpoint on localhost. They resolve only while this project's editor is open |
| `docs/EDITOR_AUTOMATION.md` | Canonical engine, editor, MCP, plugin, toolset, build and agent-playtest procedure |
| `docs/concepts_sandbox/legacy-rewind/` | Imported design, roadmaps and task files from the previous project, plus a verified code inventory and a conflict register. Non-authority |
| `docs/baseline/acme-2026-08-19/` | Frozen provenance for the working model itself. Never edited, never authority |
| `.gitignore` / `.gitattributes` | Ignore generated UE output. LFS tracks Unreal binaries, including the Tier 1 character assets |
| `docs/CURRENT_TASK.md` | REW-0014 is In Progress: adopt the owner's placed `4c_camera` as the exact initial runtime frame for Apartment 4C while preserving the region camera system |
| `docs/concept/` | Nine owner-produced target and construction-reference images: 4C, fuse box, stairwell, lift, three circulation/interaction sketches and the settled top- and ground-floor plans. Targets and blockout clarification, never game rules |
| `docs/finished/REW-0004_...md` | Superseded by REW-0006 after its frozen scope conflicted with the lift-or-stairs branch decided by REW-0005 |
| `Rewind/Source/Rewind/RewindCameraRig.cpp` | The authored camera of ADR-0007. Regions declare rotation, X/Y/Z travel axis, bounds, dead zone, player volume and explicit FOV. Half-open volumes give shared thresholds exactly one owner. The procedural proof has eleven regions; the authored building slice has seven |
| `Rewind/Content/Art/Materials/Stairwell/` | Two project-owned procedural masters and six material instances: upper and lower walls, floor, metal, door and near-black player silhouette. They are a first-pass presentation grammar and introduce no third-party dependency |
| `Rewind/Content/Art/Materials/BuildingSlice/MI_4CFloor.uasset` | Project-owned 4C floor instance with restrained reflection; it depends only on the tracked stairwell master/instance family |
| `Rewind/Source/Rewind/RewindRadio.cpp` | Four channels on the loop clock. The accepted channel speaks `7312` across 20 seconds at phases 4, 9, 14 and 19 of a 50-second cycle. A full sequence grants the stored fact; individual digits remain player memory |
| `Rewind/Source/Rewind/RewindFuse.cpp` and `RewindFuseSocket.cpp` | One carried LoopWorld fuse with two exclusive sockets. The courtyard socket enables the generator; the building socket powers the lift. The lift refuses an empty building socket |
| `Rewind/Source/Rewind/RewindLift.cpp` and `RewindStairwell.cpp` | A physical bidirectional six-second cage journey between floor 4 and entrance, plus the always-available stair routes. The cage has a blocking floor, centres its passenger and applies a configurable local-space hall exit offset, so the rotated authored instance hands the capsule to X `-280` on both landings while the procedural proof retains its default hand. Final REW-0013 PIE measured 6.00 s down and 6.00 s up |
| `Rewind/Content/Characters/Tier1/` | CC0 Quaternius Tier 1: 21 in-place animations and both pack mannequins, 31 assets and 12.61 MiB. UAL1 idle/walk drive the visible blockout player |
| `Rewind/Source/RewindEditor/` | Editor-only PIE bridge: input state, tap, held keys, exact game-time holds, queued measured sequences, active camera region/axis/transform/FOV, clean game-viewport PNG capture, restricted `Rewind.*` console calls and release-all through simulated Unreal input |
| `AutomationTestToolset` | Enabled and verified. All eight discoverable `Rewind.*` tests pass together; `Rewind.Camera.Region.VerticalTravel` covers Z follow and exact-one shared thresholds, and `Rewind.Project.DefaultAuthoredMap` locks both startup settings to the authored map |
| `docs/finished/REW-0007_...md` | Complete. The rebuilt Chapter 1 chain, timing construction, eleven cameras, Tier 1 import and three-loop evidence are archived |
| `docs/finished/REW-0008_...md` | Complete. Canonical editor/MCP context, project-scoped Codex config, PIE-input toolset and named automation-test execution |
| `docs/finished/REW-0009_...md` | Complete. Event-driven loop termination accepted; product, design and FL authority amended; implementation and owner-level adoption routed separately |
| `docs/finished/REW-0012_...md` | Complete. The authored default map extends the accepted stairwell through 4C, common halls and a bidirectional powered lift under seven camera regions |
| `docs/finished/REW-0013_...md` | Complete. Corrected REW-0012 to the owner's three-opening corridor, removed primitive furniture and wrong-side duplicates, and verified 4C, hall, stairs and powered bidirectional lift handoffs |
| `docs/finished/REW-0010_...md` | Complete on its task branch. Human-scale four-floor stairwell, stable owner reference, traversal evidence and procedural-layout opt-out |
| `docs/finished/REW-0011_...md` | Complete. Isolated stairwell camera coverage, bidirectional route, first-pass material/light grammar and clean PIE visual evidence |
| `docs/finished/REW-0006_...md` | Superseded after play established 20/50 as the radio rule while its frozen scope still required 45/60. Radio and fuse work retained for REW-0007 |
| `docs/finished/REW-0003_five-loops-test-implementation.md` | Complete. Five Loops Test implementation, archived 2026-08-23 |
| `docs/backlog/five-loops-test.md` | Authority resolved by REW-0002. Implementation activated as REW-0003 |

## What does not exist

- **Event-driven rewind in the build.** There is no causal-checkpoint actor,
  latched rewind prelude or Anchor-commit end reason. `URewindLoopSubsystem`
  still exposes only Timer and Death and automatically resets at 240 seconds.
  The accepted rule is ahead of implementation.
- **Finished environment art.** The complete Chapter 1 gameplay chain remains
  measured C++ blockout geometry. The isolated stairwell now has a bounded
  project-owned presentation pass, but its procedural plaster, basic fixtures
  and primitive geometry are not final art. Authored decals, final models,
  props, rain, neon treatment and wider-map dressing are not integrated.
  Locally imported Fab/environment packs remain outside the tracked dependency
  closure. The owner has asked that the corrected 4C use no primitive furniture
  props; imported candidates require a selected provenance/license/size review
  before any dependency is committed.
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
- **Echo, Insight as a later system, and Chapters 2 to 5.** Not written, and
  not required by the first product proof.
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

- **Accepted loop authority and the build disagree.** ADR-0009 and the owning
  design documents require event-driven rewind. The current C++ build and its
  six route/clock tests still require a 240-second automatic timeout, call the
  ground-floor fuse box a courtyard socket, and do not end on Anchor commit.
  Existing playtests are historical evidence, not evidence for FL-17 or FL-18.
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
