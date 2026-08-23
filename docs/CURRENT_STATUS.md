# Current Status

Reality as of 2026-08-23. This document describes what exists, not what is
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
| `Rewind/Rewind.uproject` | Unreal Engine 5.8 C++ project. Loop clock, apply order, session save, CleanSave |
| `Rewind/Content/Maps/FiveLoops.umap` | Default map. C++ builds 4C on floor 4, its common hallway, cage-lift shaft, three-flight switchback stairs, entrance hallway, a 170 m service route folded around one large courtyard, and Transit Hub. Eleven playable regions declare complete cameras |
| `docs/design/` | Five accepted documents. Ownership in `docs/design/README.md` |
| `docs/acceptance/five-loops-test.md` | Accepted. Criteria FL-01 to FL-16. The earlier record evidences all sixteen against their pre-REW-0005 wording; REW-0007 re-verifies the rebuilt chain's touched criteria FL-01, FL-04, FL-06, FL-07, FL-09, FL-12, FL-13 and FL-14 |
| `docs/playtests/` | Evidence from named runs of named builds. The earlier complete Five Loops record remains; REW-0007 adds a Chapter 1 three-loop record for the rebuilt chain |
| `.codex/config.toml` and `.mcp.json` | Project-scoped clients for the running editor's MCP endpoint on localhost. They resolve only while this project's editor is open |
| `docs/EDITOR_AUTOMATION.md` | Canonical engine, editor, MCP, plugin, toolset, build and agent-playtest procedure |
| `docs/concepts_sandbox/legacy-rewind/` | Imported design, roadmaps and task files from the previous project, plus a verified code inventory and a conflict register. Non-authority |
| `docs/baseline/acme-2026-08-19/` | Frozen provenance for the working model itself. Never edited, never authority |
| `.gitignore` / `.gitattributes` | Ignore generated UE output. LFS tracks Unreal binaries, including the Tier 1 character assets |
| `docs/CURRENT_TASK.md` | Restored task template. REW-0007 is complete and archived; no successor task is approved |
| `docs/concept/` | Nine owner-produced target and construction-reference images: 4C, fuse box, stairwell, lift, three circulation/interaction sketches and the settled top- and ground-floor plans. Targets and blockout clarification, never game rules |
| `docs/finished/REW-0004_...md` | Superseded by REW-0006 after its frozen scope conflicted with the lift-or-stairs branch decided by REW-0005 |
| `Rewind/Source/Rewind/RewindCameraRig.cpp` | The authored camera of ADR-0007. Eleven enumerated regions declare rotation, travel axis, bounds, dead zone and player volume: 4C, fourth-floor hallway, lift shaft, three flights, two landings, entrance hallway, the courtyard and Transit Hub |
| `Rewind/Source/Rewind/RewindRadio.cpp` | Four channels on the loop clock. The accepted channel speaks `7312` across 20 seconds at phases 4, 9, 14 and 19 of a 50-second cycle. A full sequence grants the stored fact; individual digits remain player memory |
| `Rewind/Source/Rewind/RewindFuse.cpp` and `RewindFuseSocket.cpp` | One carried LoopWorld fuse with two exclusive sockets. The courtyard socket enables the generator; the building socket powers the lift. The lift refuses an empty building socket |
| `Rewind/Source/Rewind/RewindLift.cpp` and `RewindStairwell.cpp` | A physical six-second cage journey between floor 4 and entrance, plus three always-available 24-step flights. Geometry measures 50.40 s minimum; the formal PIE routes measured 55.67 s by stairs and 6.00 s by lift, a 49.67 s difference |
| `Rewind/Content/Characters/Tier1/` | CC0 Quaternius Tier 1: 21 in-place animations and both pack mannequins, 31 assets and 12.61 MiB. UAL1 idle/walk drive the visible blockout player |
| `Rewind/Source/RewindEditor/` | Editor-only PIE-input bridge: state, tap, held keys, exact game-time holds, queued measured sequences, restricted `Rewind.*` console calls and release-all through simulated Unreal input |
| `AutomationTestToolset` | Enabled and verified. All six discoverable `Rewind.*` tests passed together, 6/6 with no errors, in 0.0411 seconds |
| `docs/finished/REW-0007_...md` | Complete. The rebuilt Chapter 1 chain, timing construction, eleven cameras, Tier 1 import and three-loop evidence are archived |
| `docs/finished/REW-0008_...md` | Complete. Canonical editor/MCP context, project-scoped Codex config, PIE-input toolset and named automation-test execution |
| `docs/finished/REW-0006_...md` | Superseded after play established 20/50 as the radio rule while its frozen scope still required 45/60. Radio and fuse work retained for REW-0007 |
| `docs/finished/REW-0003_five-loops-test-implementation.md` | Complete. Five Loops Test implementation, archived 2026-08-23 |
| `docs/backlog/five-loops-test.md` | Authority resolved by REW-0002. Implementation activated as REW-0003 |

## What does not exist

- **Finished environment art.** The complete Chapter 1 gameplay chain now
  exists as measured C++ blockout geometry. The nine owner reference images are
  targets only; final models, materials, rain, neon treatment and prop dressing
  remain outside REW-0007.
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
