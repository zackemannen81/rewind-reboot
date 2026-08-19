# Code inventory of the previous Unity project

Status: Non-authority. Evidence of what the previous implementation did.
Source: `C:\code\reWiND`, commit `9f959140e25163bddef390979f23f352e8f0c2f6`,
2025-10-24, branch `main`, working tree clean at the time of this reading.
Read: 2026-08-19. Unity Editor was not launched. Nothing here is a rule for
this repository.

This file records observed behaviour in source. It does not decide how RE:WIND
is built now.

## How this was verified

- `git log -1` on `C:\code\reWiND` returned the commit above.
- `git status` on that repository was clean.
- Every `.cs` file under `Assets/Scripts/` was listed (thirty files).
- The managers, EventBus, Echo classes, Chapter 1 orchestrator, gate, fuse,
  generator, radio, turnstile, both patrols, echo visualizer, bootstrap and
  scene entry were read in full. `PlayerController.cs` was read through its
  field declarations and `Awake`/`Start`. The remaining player, camera, editor
  and ambient files were listed, not line-read.
- `Chapter1_Blockout.unity` was searched for the GUIDs of `TimeManager`,
  `KnowledgeManager`, `AnchorManager` and `Chapter1Bootstrap`.
- `ProjectSettings/EditorBuildSettings.asset` was read.
- `ProjectSettings/ProjectVersion.txt` was read.

What was not verified: play in the Unity Editor, a packaged build, runtime
PlayerPrefs on disk, or whether a developer had manager objects in an unsaved
editor session.

## Project facts

| Field | Observed |
| --- | --- |
| Engine | Unity 2022.3.62f2, from `ProjectVersion.txt` |
| Scenes in EditorBuildSettings | none |
| Committed scene | `Assets/Scenes/Chapter1/Chapter1_Blockout.unity` contains a Directional Light, Main Camera, SceneOrigin, and a `Chapter1Root` with `Chapter1Bootstrap` |
| C# gameplay and editor scripts under `Assets/Scripts/` | 30 |
| Additional C# under `Assets/Art/Scripts/` | art-palette helpers, not inventoried line by line |

## SYS-001 claim versus source

`tasks/task_core_systems.md` is marked `DONE`. Its acceptance criteria name a
TimeManager that can start, end and reset the loop; a KnowledgeManager that
persists knowledge across loops; an AnchorManager that locks world states; and
an EchoSystem that records and plays back movement.

The classes exist. The behaviours those criteria describe do not, as a running
system, in the committed project.

## Managers are not in the committed scene

`TimeManager`, `KnowledgeManager` and `AnchorManager` are never added by any
script under `Assets/Scripts/`. Their script GUIDs do not appear in
`Chapter1_Blockout.unity` or in any `.prefab` searched. The only scene
gameplay object is `Chapter1Bootstrap`.

`LoopStartEvent` is published only from `TimeManager.StartLoop`.
`KnowledgeManager.Instance` and `AnchorManager.Instance` are reached with
null-conditional calls from Chapter 1 scripts. If the managers are absent,
those calls do nothing.

Supported inference: with the committed scene as shipped, the 7-minute loop
does not start, knowledge is not stored, and the courtyard-gate anchor is not
applied, unless a manager object is added by hand in an uncommitted editor
session. That session is not in the repository.

## TimeManager

File: `Assets/Scripts/Managers/TimeManager.cs`

Observed:

- Default loop duration is `420f` seconds.
- `StartLoop` sets the timer, sets `_isLoopActive`, publishes `LoopStartEvent`.
- `Update` subtracts `Time.deltaTime` and publishes `MinutePassedEvent` when
  the remaining whole minute drops.
- At `<= 0` it calls `EndLoop("time_expired")`.
- `EndLoop` publishes `LoopEndEvent` and immediately calls `ResetLoop`.
- `ResetLoop` contains the comment that it would reset world state, then calls
  `StartLoop` only.

Not observed: a world-state object, a reset order, a spawn reset, or any call
into Chapter 1 objects.

`TimeManager` is not a singleton. Nothing in the tree constructs it.

## KnowledgeManager

File: `Assets/Scripts/Managers/KnowledgeManager.cs`

Observed:

- Singleton with `DontDestroyOnLoad`.
- Stores `Dictionary<string, string>`.
- Serialises to `PlayerPrefs` key `"Knowledge"` via MiniJSON on every
  `AddKnowledge`.
- Loads that key in `Awake` if present.
- `GetKnowledge` / `HasKnowledge` exist.

Not observed: a method that clears knowledge, a distinction between
loop-scoped and session-scoped data, or any write other than
`Chapter1RadioController` which would store key `chapter1_radio_code` value
`"7312"` if the instance existed.

## AnchorManager

File: `Assets/Scripts/Managers/AnchorManager.cs`

Observed:

- Singleton with `DontDestroyOnLoad`.
- Stores `HashSet<string>`.
- Serialises to `PlayerPrefs` key `"Anchors"`.
- `ActivateAnchor` and `IsAnchorActive` exist.

Not observed: applying an identifier to world objects, a cap, an Insight cost,
or more than one identifier in the whole codebase.

The only identifier is `Chapter1Constants.AnchorCourtyardGate`
(`"chapter1_courtyard_gate"`). The only reader is `Chapter1CourtyardGate.OnLoopStart`,
which calls `ForceOpen` if that string is active.

## Echo

File: `Assets/Scripts/Core/EchoSystem.cs`

`EchoRecorder` observed:

- Subscribes to loop start and end.
- Samples `transform.position` and `transform.rotation` every `0.1f` seconds
  while recording.
- Timestamps with `Time.time`.
- Clears the list on `StartRecording`.
- On loop end, stops recording. The comment says a recording would typically
  be saved. Nothing is saved.

`EchoPlayback` observed:

- Can lerp along a supplied list of `PlayerAction`.
- Uses `Time.time` against the first sample's timestamp.
- No other file calls `StartPlayback`.

`Chapter1Bootstrap.EnsureEchoSystem` adds `EchoRecorder`, `EchoPlayback` and
`Chapter1EchoVisualizer` to a child of the bootstrap. The recorder therefore
samples the echo object's transform, not the player's, unless that object is
later parented to the player — which this bootstrap does not do.

`Chapter1EchoVisualizer` observed:

- On loop end, spawns up to twenty primitive spheres from the recorder's
  samples, spaced `0.8f` seconds apart.
- On the next loop start, destroys them.
- This is a point trail, not action replay.

## EventBus

File: `Assets/Scripts/Core/EventBus.cs`

Observed:

- Static dictionary of subscribers.
- `Publish` invokes them.
- `Unsubscribe` returns after a presence check and does not remove the
  callback. The comment calls this simplified.

Supported inference: objects that disable and re-enable accumulate duplicate
handlers.

## Chapter 1 reset is per object, not a world model

`Chapter1LoopOrchestrator` subscribes to `LoopStartEvent` and calls
`OnLoopStart` on radio, fuse box, generator, gate and turnstile, if those
references were set by `Configure`. It does not reset the player, either
patrol, the echo trail, or a baseline world snapshot.

There is no `WorldState` type in `Assets/Scripts/`.

## Authored Chapter 1 objects

These exist as scripts and are wired by `Chapter1Bootstrap.BuildBlockout`.

| Object | Observed behaviour | Loop start |
| --- | --- | --- |
| Radio | Clarity clamped to loop iteration 1–4. At iteration `>= 3` writes knowledge `7312` once per component lifetime (`_codeRecorded` is not reset) | Called from orchestrator |
| Fuse box | `hasFuse = true`, power to courtyard false | Called from orchestrator |
| Fuse interact | E routes power to courtyard and consumes the fuse | No loop hook |
| Generator | Goes offline and stops audio. `HasCompletedGoldenPath` is not cleared | Called from orchestrator |
| Gate | Opens if the courtyard-gate anchor string is active, else closes | Called from orchestrator |
| Turnstile | 30 s cycle, window `cyclePosition <= 3f` seconds, clocked from `Time.time` at loop start | Called from orchestrator |
| Guard patrol | Waypoint loop at `1.6f`, investigates noise for `4f` seconds, movement uses `Time.deltaTime` | None |
| Drone patrol | Waypoint loop at `3.2f`, hover uses `Mathf.Sin(Time.time * hoverFrequency)`, investigates `3.5f` seconds, movement uses `Time.deltaTime` | None |

The GDD's 20-second guard window to the hub and the 2.5-second turnstile
opening are not the numbers in these scripts. The turnstile window in code is
3 seconds. The guard script has no 20-second window.

`HasCompletedGoldenPath` staying true across loops means that, if the
orchestrator and `AnchorManager` ran, a single successful generator activation
would make every later `LoopEndEvent` re-activate the same anchor identifier.

## Patrols are not reproducible from the source as written

Both patrols:

- do not subscribe to `LoopStartEvent`;
- keep waypoint index and investigate state across whatever a loop boundary
  would be;
- move with `Time.deltaTime`.

The drone also hovers on global `Time.time`, so its vertical motion is not a
function of time-in-loop.

This was read from the two patrol files. It was not playtested.

## Player

`PlayerController.cs` implements walk, run, crouch, sneak, jump, vault, climb,
lean and noise broadcast onto `PlayerNoiseEvent`. No loop-start reset of
transform or pose was present in the portion read. Whether the player is
spawned by bootstrap was not fully traced; `Chapter1Bootstrap` does not spawn
a player in the `BuildBlockout` method that was read.

## Loop events that exist on paper

`GameEvents.cs` defines `LoopStartEvent`, `LoopEndEvent`, `MinutePassedEvent`
and `PlayerNoiseEvent`. There is no paradox event and no knowledge-discovered
event.

## What the previous project actually had, as source

Observed:

- A third-person controller with stealth movement and noise.
- A Chapter 1 bootstrap that builds a blockout of Apartment 4C, courtyard,
  street and Transit Hub, and wires radio, fuse, generator, gate, turnstile
  and two patrols.
- Manager *classes* for time, knowledge and anchors.
- Transform sampling and a sphere trail labelled Echo.
- Task files that call the core systems `DONE`.

Not observed in this source:

- A running loop in the committed scene.
- Canonical world reset.
- Canonical anchor application beyond one gate checking one string.
- Echo action replay.
- A way to obtain a clean save.
- Reproducible patrols across loop boundaries.
- Insight, WorldState, ParadoxValidator, or any Chapter 2–5 implementation.
