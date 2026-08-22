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
| `docs/adr/ADR-0005_asset-storage.md` | Accepted. Unreal binaries via Git LFS. No `.uasset` in history yet |
| `docs/adr/ADR-0006_cpp-and-blueprint.md` | Accepted. C++ owns FL systems; Blueprint may place and present |
| `Rewind/Rewind.uproject` | Unreal Engine 5.8 C++ project. Loop clock, apply order, session save, CleanSave |
| `Rewind/Content/Maps/FiveLoops.umap` | Default map. 4C, courtyard, street, hub blockout plus radio, lock, fuse, generator, gate, patrol, turnstile, Anchor board. Outdoor run bounded and continuous; gate, turnstile and patrol barrier each span the corridor |
| `docs/design/` | Five accepted documents. Ownership in `docs/design/README.md` |
| `docs/acceptance/five-loops-test.md` | Accepted. Criteria FL-01 to FL-16. **All sixteen have named evidence** |
| `docs/playtests/` | Evidence from named runs of named builds. One record, `five-loops-2026-08-22.md`, Complete, with four raw `LogRewind` captures beside it |
| `.mcp.json` | Points Claude Code at the running editor's MCP endpoint on localhost. Resolves only while the editor is open |
| `docs/concepts_sandbox/legacy-rewind/` | Imported design, roadmaps and task files from the previous project, plus a verified code inventory and a conflict register. Non-authority |
| `docs/baseline/acme-2026-08-19/` | Frozen provenance for the working model itself. Never edited, never authority |
| `.gitignore` / `.gitattributes` | Ignore generated UE output. LFS tracks Unreal binaries. No `.uasset` committed yet |
| `docs/CURRENT_TASK.md` | Restored from the template. No task is active |
| `docs/finished/REW-0003_five-loops-test-implementation.md` | Complete. Five Loops Test implementation, archived 2026-08-23 |
| `docs/backlog/five-loops-test.md` | Authority resolved by REW-0002. Implementation activated as REW-0003 |

## What does not exist

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

- **The central hypothesis is only half answered.** The loop is proven
  deterministic and knowledge does persist, so a player who knows the answer
  skips work. What the evidence also shows is that skipping the work buys
  almost nothing: every turnstile crossing landed within 0.7 s of the gate
  opening, because the space is small enough that even the full chain finishes
  early. Whether that is enjoyable is untested and is not an FL criterion.
- **Legacy material still contradicts the decided determinism rule.** ADR-0002
  chose a world that does not learn the player. The imported GDD still
  contains both that game and a game whose details shift. The sandbox is
  not authority; the contradiction stays there as C1.
- **The engine change discards the existing implementation.** ADR-0001 accepts
  this. The legacy C# is design reference from now on, and the cost is
  contained because the systems that mattered were not running in the
  committed scene anyway.
- **No verification tooling exists.** There is no link checker. Building is
  manual, and every task states which checks it could not run.
- **The instrumentation resolves to a tick, not an instant.** `LogRewind`
  writes a transition on the tick that first observes it, so two loops report
  the same boundary up to about 0.25 s apart. The offset is constant inside a
  loop, which is tick phase and not drift. Any criterion whose evidence needs
  exact equality at an arbitrary `t` is not closed by the log alone.
- **The editor cannot be driven and rebuilt at the same time.** The MCP
  endpoint lives inside the editor, and the editor holds the build lock.
  It also exposes no console-command and no input tool, so no criterion
  needing player action can be evidenced without a human at the keyboard.
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
