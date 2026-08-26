# System Document

The durable shape of this repository and of the game's systems once they
exist. Behaviour that is implemented is described here; intentions belong in
`docs/PROJECT_BRIEF.md`. Game rules belong in `docs/design/` when a task
writes them.

An Unreal Engine 5.8 C++ project exists at `Rewind/Rewind.uproject`. The loop
clock, ordered world-state apply, session knowledge, one Anchor, clean save,
authored camera, loop-clocked radio, contested fuse, lift, stairs, player
message channel and rebuilt Chapter 1 blockout are implemented. The
owner-shaped stairwell, Apartment 4C,
fourth-floor common hall, lift and courtyard form the bounded authored
presentation slice and the project's editor/game default. REW-0013 corrected REW-0012 against the
owner's existing stairs/lift/4C openings and removed its opposite-side duplicate.
An editor-only module exposes
project PIE input and clean game-viewport capture through the MCP toolset
registry, and the engine automation-test toolset is enabled. The sections below
distinguish the running proof from later art and product work.

ADR-0009 and the owning design documents require event-driven loop
termination. REW-0017 implements that rule in the running C++: a loop ends on
causal-contract failure at a named checkpoint, on death, or on a successful
first-time Anchor commit. Contract failure and commit latch a one-to-three-
second rewind prelude measured on elapsed loop time. Elapsed time alone does
not end a loop unless an authored whole-space deadline is explicitly enabled.

`/Game/Maps/FiveLoops_Stairwell_Blockout` is the first standalone authored
geometry map. It preserves the owner's switchback shape at human scale: five
floor surfaces at Z = 0, 300, 600, 900 and 1200 cm and four alternating
flights, each 170 cm wide with seventeen 28 cm treads and 17.65 cm risers.
Invisible alternating ramps provide continuous capsule collision over the
visible steps. Floors 1-3 have static closed doors on the same side as the 4C
and entrance openings. The map carries the WorldSettings tag
`Rewind.SkipProofLayout`, which makes `URewindWorldStateSubsystem` leave its
authored contents alone instead of spawning `ARewindProofLayout` over them.

Three half-open authored camera volumes cover that map without a vertical gap:
entrance Z `[-100, 180)`, stair travel Z `[180, 1180)`, and upper threshold Z
`[1180, 1500)`. All three use Z as their sole travel axis. The stair and upper
frames retain the owner camera's X = 1520 cm, yaw = 180 degrees and 37.5 degree
horizontal FOV derived from its 35 mm lens; the entrance uses X = 1400 cm,
pitch = -3 degrees and 42 degrees FOV. The runtime camera applies the region's
travel-axis framing offset after clamping the player coordinate and blends
explicit FOV with position and rotation.

Four additional half-open regions extend the same map through Apartment 4C,
the common hall, the lift shaft and the entrance hall. The apartment and hall
regions follow Y, while the cutaway shaft follows Z. Their fixed lenses are
37.497356, 58, 60 and 58 degrees respectively. REW-0020 attaches two more
Y-travel regions at `GroundHall_Region`'s positive-Y face:
`CourtyardApproach_Region` copies that hall's lens, and `Courtyard_Region`
cuts to a first-pass side-on 42 degree frame. The nine volumes abut without
a gap. The Apartment4C region derives
its initial frame from the preserved editor reference `4c_camera`: world
position `(750, 1330, 1330)`, rotation `(0, 180, 0)` and a 35 mm lens on its
23.76 mm-wide filmback. Standard PIE starts from the 4C
PlayerStart at `(0, 1580, 1296)`; the authored map is named by both
`EditorStartupMap` and `GameDefaultMap`.

The saved extension preserves the owner corridor wall at approximately X
`-260` with three openings centred near Y `328`, `630` and `1043`: stairs,
lift and 4C respectively. Apartment 4C occupies the existing 400 by 1050 cm
floor footprint behind the third opening and is enclosed to 300 cm height. The
vertical shaft is centred at `(-40, 630)` behind the middle opening across the
existing 1200 cm floor-four-to-entrance separation. The loop-clocked radio,
manual `7312` lock, one fuse, both sockets and bidirectional cage occupy those
corrected spaces. The cage has a blocking floor, centres its passenger during
travel and transforms its configurable local-space hall offset before
walking resumes.

The map uses project-owned procedural master materials and instances for upper
plaster, the separate green lower wall band, dark circulation surfaces,
guardrails, doors and the player silhouette, plus a project-owned unlit
emissive 4C sign material. Localized shadow-casting point lights with authored
source size, restrained cool fill, a dim stationary skylight and a
manual-exposure post process form the first-pass visual grammar. The 4C sign
panel has a real magenta light so it can bleed onto the wall behind it.

ADR-0010 sets the project renderer: Lumen global illumination
(`r.DynamicGlobalIlluminationMethod=1`), Lumen reflections
(`r.ReflectionMethod=1`), virtual shadow maps (`r.Shadow.Virtual.Enable=1`),
mesh distance fields (`r.GenerateMeshDistanceFields=True`) and default bloom
(`r.DefaultFeature.Bloom=True`). Auto-exposure stays off. Hardware ray tracing
stays off; Lumen uses software tracing through mesh distance fields. The
frame-time budget is 16.67 ms at 1920×1080 on the named development machine.
This is a presentation proof, not final art, and the added sign material
introduces no third-party environment asset dependency. Untracked `/Game/Fab/`
and `/Game/Art/Texture/` packages remain referenced by the saved map and remain
outside the tracked closure.

PIE exposed one REW-0010 collision defect in reverse traversal: a 180 cm-deep
intermediate landing overlapped the stair approach far enough for the capsule
to meet its underside before reaching the top. Floors 1-4 now keep their outer
edge but use a 130 cm landing depth, moving the inner edge outward by 50 cm.
The visible flights, elevations, doors and outer landing boundaries are
unchanged; the measured route now works in both directions.

The runtime player is The Returner. Its mesh bounds report an origin at Z 85 cm
and a Z extent of 85 cm, placing the imported feet at Z 0. The 42 cm radius,
96 cm half-height capsule therefore receives a Z −96 cm presentation offset.
The Returner reference pose's `headfront` endpoint is +Y from the head; yaw
−90 degrees maps that forward vector to the character root's +X movement
direction. This alignment is presentation only; the capsule remains
authoritative for collision.

## The working model in one page

```text
private actor context
  → explicit repository state
  → one bounded active task
  → only the authority that task names
  → work and verification
  → durable status, decision and handoff
  → next actor resumes without the previous actor's memory
```

The benefit is not documentation volume. It is work continuity with bounded
context loading: the reader loads the active task and the authority it names,
not the project's whole history.

## Semantic roles

| Role | This repository | Owns |
| --- | --- | --- |
| Entry and guardrails | `AGENTS.md` | Reading order, safety, operating rules |
| Active work | `docs/CURRENT_TASK.md` | One task's frozen charter, progress, verification, handoff |
| Task state machine | `docs/TASK_WORKFLOW.md` | States, freeze, routing, identity |
| Approved direction | `docs/PROJECT_BRIEF.md` | Product, first proof, non-goals |
| Current reality | `docs/CURRENT_STATUS.md` | What exists now, and the gaps |
| Durable system model | `docs/SYSTEMDOC.md` | This document |
| Editor automation | `docs/EDITOR_AUTOMATION.md` | Engine install, MCP lifecycle, plugins, toolsets, build and agent playtests |
| Work history | `docs/JOURNAL.md` | Dated, signed evidence of work waves |
| Repository map | `docs/FILESTRUCTURE.md` | Where things live and why |
| Identity allocation | `docs/TASK_IDS.md` | Which identities are taken |
| Durable decisions | `docs/adr/` | Decisions, alternatives, consequences |
| Game rules | `docs/design/` | One ownership area per document |
| Proof of done | `docs/acceptance/` | What a build must demonstrate; empty until a task writes it |
| Evidence | `docs/playtests/` | What a named run of a named build actually showed, and what it did not |
| Completed work | `docs/finished/` | Immutable archived tasks |
| Blocked work | `docs/paused/` | Frozen parents awaiting a condition |
| Future work | `docs/backlog/` | Non-activated proposals |
| Excluded ideas | `docs/concepts_sandbox/` | Undecided concepts and imported legacy, never authority |
| Model provenance | `docs/baseline/` | Frozen docs-first source, never edited, never authority |

## Why the sandbox exists

An idea has three possible destinations: an authoritative document, where it
contaminates the truth surface; a chat log, where it leaves the repository; or
a marked non-authority area. Only the third keeps both properties the project
depends on — nothing valuable leaves, nothing undecided gains authority.

The previous project's GDD, tasks and code inventory live there for that
reason. They may be read. They may not be implemented.

The only route out:

```text
concept or legacy material
  → activated task or discovery
  → explicit decision
  → restatement in an owning document under docs/
  → implementation
```

## Addressing

A record's path is its identity. Status lives in the record's content and in
its collection index.

Journal entries are append-only and archived tasks are immutable, and they
cite other records by path. Renaming a cited record leaves two exits, and
both are forbidden: repairing append-only history, or leaving citations
broken. Restore the original path.

Validation of cited paths must distinguish tense. A document describing the
present must name paths that exist. The archive names files after they are
gone and the active charter names its deliverables before they exist.

## Identity allocation

Identities are claimed on the trunk before a charter freezes, in a strictly
ascending append-only register. The prefix in force is `REW`. `DFC-0001`
remains a taken address from the extraction this repository was bootstrapped
from.

The register records identity, never activity.

## Game systems

`Rewind/` has C++ types for the loop clock, loop-start apply order, session
knowledge, the one legal Anchor identifier, a USaveGame slot, and a
reachable `Rewind.CleanSave` command. World clocks are required to read
`URewindLoopSubsystem::GetElapsedLoopTime`, not engine time.

`URewindLoopSubsystem` advances the loop clock without a default terminal
duration. It exposes end reasons CausalContract, Death and AnchorCommit. Timer
remains only for an authored whole-space deadline, which is off by default
(`bUseWholeSpaceDeadline`). A failed crossing of `ARewindCausalCheckpoint`
(`GroundFuseGate`: ground-floor fuse seated or `courtyard_gate_open` active)
or a first-time successful Anchor commit latches a rewind that cannot be
cancelled by stepping back. The latched prelude lasts the configured duration
clamped to [1.0, 3.0] seconds of elapsed loop time, then loop-start apply
runs. Death ends the loop immediately. `ARewindLoopBreakSignature` presents
rising interference during the prelude as a function of elapsed loop time,
not engine time or frame delta. A rejected or redundant Anchor commit does
not latch. `GroundFuseGate` is spawned in C++ at the proof-layout hall-to-
courtyard seam (X = 4110) on maps that do not carry `Rewind.SkipProofLayout`.
On SkipProofLayout maps `ARewindAuthoredCourtyard` places the same named
checkpoint at its own hall-to-courtyard seam. PIE on the authored default
measured that seam at `(-430, 2700, 150)`.

`/Game/Maps/FiveLoops` is the preserved procedural proof map. It loads
`ARewindProofLayout`, which spawns 4C on floor 4,
its common hallway, a cage-lift shaft, three switchback stair flights, the
entrance hallway, one large courtyard and Transit Hub. The 170 m service route
folds around the courtyard patrol yard and rejoins the main route at the
generator instead of becoming a detached corridor. The layout also spawns the
radio, code lock, one fuse, two sockets, generator, gate, patrol, turnstile and
an Anchor board. LoopWorld
participants restore from baseline on loop start; the gate honors
`courtyard_gate_open`. Patrol, turnstile and radio phase read elapsed loop
time. Interact is E. Digits type a code at the lock. The board commits the
gate Anchor only if this loop opened the gate. Elapsed loop time is drawn on
screen as `t=` for stated-time checks.

Player-facing text is owned by `URewindMessageSubsystem`. Actors call `Show`,
`ShowTimed`, `ShowSequence` or `Clear` by catalog id. Copy and default
duration live in `FRewindMessageCatalog`, so rewriting a line does not require
editing the actor that triggers it. One line is visible at a time. Further
lines wait in enqueue order and expire into the next. Presentation is a
project Slate overlay at the bottom of the game viewport: light text on a
dark backing, hit-test invisible. It does not use
`AddOnScreenDebugMessage`. The `t=` overlay remains instrumentation inside
`URewindLoopSubsystem` and is not this channel. Messages tick on frame delta,
are not world state, and are not saved. The subsystem does not clear them at
loop start.

First-run copy is owned by `docs/design/tutorial-and-first-run.md`. It names
what a thing is and does not name what to do with it. Gated first-run lines
are shown at most once per session. The seen-state is the knowledge fact
`seen.<MessageId>` on `URewindSessionSubsystem`, not a second store. A
first-time player is told that the room starts over and that they do not,
before they leave 4C. Radio spoken digits remain the puzzle speaking;
first-run copy does not print `7312`. No catalog line describes a ghost.

The radio has four channels and exactly one speaks the code. Its 20-second
sequence repeats every 50 seconds and speaks `7`, `3`, `1`, `2` at phases 4,
9, 14 and 19. Hearing an individual digit changes no repository state; it is
player knowledge. Staying in range for a complete sequence grants the stored
`radio_code_7312` fact used by the lock's convenience path. Channel and
broadcast phase reset from Baseline and never read loop count. REW-0035 makes
the existing digit events play their matching SoundWaves while retaining the
message copy. The radio's LoopWorld channel drives an attached spatial station
or static loop; channel `0` is off. Its digit and bed attenuation ends at 320
cm, the same as the code-credit `RadioRange`, while on/off/tune clicks are dry
2D sounds. Player footsteps select among six interior sounds by travelled
distance and never choose the immediately preceding variation.

The authored camera remains player zero's visual view target. It also owns the
spatial listener correction: every tick, and immediately from `SnapToPlayer`
after loop/player placement, it attaches Unreal's listener override to the
possessed pawn's root with zero location offset. UE 5.8 composes an attached
override as component rotation plus rotation offset and component location plus
the rotated location offset, so the rig supplies camera-world rotation minus
root-world rotation. The resulting listener world position is exactly the root
position while its front/right orientation is exactly the camera component's
current world orientation. A possession change replaces the attachment on the
next tick; ending the rig clears the override. The visual camera does not move,
and the radio's 320 cm attenuation and player-relative hearing-credit rule are
unchanged.

The fuse is one LoopWorld actor whose state is at rest, carried, seated in the
building socket or seated in the courtyard socket. It owns that state, so two
sockets cannot both be occupied. Carrying disables its collision and follows
the player; loop start restores it to the authored 4C position. The generator
starts only while the fuse is in the courtyard socket. The cage lift runs only
while that same fuse is in the building socket. It carries the player through
1200 cm in six seconds; otherwise it refuses the interaction. The stairs always
work: three flights of 24 physical steps span the same floor transitions. Their
10,080 cm run is 50.40 seconds at the authored 200 cm/s walk speed, 44.40
seconds more than the lift and therefore more than one 30-second turnstile
period.

The courtyard power branch is 17,000 cm one way. A Loop B route that carries
the fuse there, opens and commits the gate, then returns by stairs models at
260.40 seconds against the 240-second loop. With the held gate, the fuse stays
in the building socket and the lift-plus-main learned route models at 46.00
seconds. These construction values live together in
`RewindChapter1Metrics.h` and are asserted by the named Chapter 1 automation
tests. Formal PIE measured 55.67 seconds from the fourth floor to the entrance
by stairs, 6.00 by lift, no Loop B hub entry by the 240-second timeout, and a
Loop C hub entry at 90.67 seconds.

The gate, the turnstile and the patrol barrier each span the walkable
corridor, so a closed one cannot be walked around. The outdoor run is
bounded and continuous. Exposure is authored in a post-process volume,
because the sun is a physical 50000 lux and auto exposure is off.

`LogRewind` writes the run: loop start and end with reason, the
apply-order baseline, session writes, interactions, and patrol and
turnstile transitions. Play events carry elapsed loop time. Apply-order
events do not, because they run before the clock is zeroed at step 5 and
a stamp there would name the previous loop's final time. Transitions are
written on the tick that first observes them, so the log resolves state
changes to a tick rather than to an instant.

All sixteen FL criteria carried named evidence as of 2026-08-23, against the
criteria as they read then. REW-0007's three-loop record separately
re-verifies the rebuilt chain's touched criteria: FL-01, FL-04, FL-06, FL-07,
FL-09, FL-12, FL-13 and FL-14. `docs/playtests/` owns evidence; this document
owns what the systems do.

The rebuilt chain makes `courtyard_gate_open` pass ADR-0008's
resource-and-two-uses test. The Anchor does not grant a new action: it removes
the 170 m generator branch from the next loop. That releases the one fuse for
the building socket, which in turn releases the lift and enough loop time to
reach Transit Hub.

Eleven authored camera regions cover the procedural proof's 4C, common hallway, the
lift shaft, all three stair flights, both switchback landings, the entrance
hallway, the complete courtyard and Transit Hub. Each declares its player
volume, travel axis, offset, rotation, padding, dead zone and transition mode.
Separate switchback landing volumes allow the player to cross between the
flights' alternating Y lanes without violating the one-region rule. The large
courtyard composition holds the gate, generator, patrol yard and Transit route
in the same spatial field, matching the settled ground-floor plan.

The authored building slice separately uses nine regions: the three accepted
stairwell regions plus Apartment4C, TopHall, LiftShaft, GroundHall,
CourtyardApproach and Courtyard.
Apartment4C is centred at `(62.5, 1368, 1340)` with player-volume extent
`(307.5, 515, 160)`, so it covers `X [-245, 370)` from the corridor wall to the
cutaway edge and abuts `TopHall` without a gap. Its `CameraOffset` of
`(687.5, -250, -10)` reproduces the owner's adopted frame at `(750, 1330, 1330)`.

Movement input is expressed against the region containing the player. Where no
region contains it, the character holds the last contained region's screen axes
rather than discarding the input, matching what `ARewindCameraRig` already does
with the frame, and writes one log line on entering and on leaving that state.
`ClampToPlayerVolume` insets one millimetre inside its half-open positive faces
so a clamped location is still contained by the region that clamped it. A named
automation test asserts that invariant.

The authored camera type supports X, Y or Z as the one legal travel axis and
stores an explicit horizontal FOV per region. Region player volumes are
half-open on their positive edge, so two adjacent regions cannot both own the
same exact threshold. A named automation test asserts vertical travel,
travel-axis framing offset, clamp/padding behavior, fixed non-travel axes,
explicit FOV and exactly-one ownership immediately before, on and after a
shared threshold.

The runtime player uses `/Game/Characters/Returner/Returner` on its own
24-bone skeleton. `A_Returner_Alert` and `A_Returner_Walk` switch from
CharacterMovement velocity in `ARewindCharacter`; movement remains
CharacterMovement-owned. `MI_CharacterSilhouette` is assigned across every
Returner material slot. The run and second walk assets are unbound. The 31
CC0 Tier 1 assets remain under Git LFS but are no longer the runtime binding.

The previous Unity project, inventoried in
`docs/concepts_sandbox/legacy-rewind/code-inventory.md`, contained manager
classes, a Chapter 1 bootstrap and a player controller. It did not contain a
canonical world reset, a running loop in the committed scene, Echo replay, or
reproducible patrols. That inventory is evidence, not a system description of
this project.

When a task implements a system, this document is updated in the same change
to describe what the implementation does, not what is intended.

## Editor automation

`Rewind/Source/RewindEditor/` is an editor-only module. At `PostEngineInit` it
registers `RewindEditor.RewindPIEInputToolset` with the engine's
`ToolsetRegistry`. Its calls locate player zero in the in-process PIE world and
deliver simulated `FInputKeyEventArgs` through `APlayerController::InputKey`,
so the game's normal input mappings receive them without desktop focus or
Windows input emulation. It tracks keys pressed through the toolset, exposes
their state with the possessed pawn transform, releases them explicitly, and
clears its bookkeeping when PIE ends. Exact game-time holds and queued input
sequences keep measured routes independent of MCP response latency. The state
result includes the active region, X/Y/Z axis, view target, camera transform
and FOV. `CapturePIEScreenshot` requests a clean PNG from the active game
viewport on its next rendered frame, independent of editor-window overlays.
Console execution is restricted to project-owned `Rewind.*` commands.

`AutomationTestToolset` is enabled beside `EditorToolset`. The running MCP
server therefore owns the complete agent loop: editor and log inspection,
standard PIE start/stop, project player input, viewport capture, and named
automation-test discovery/execution/results. The server itself runs inside the
editor on localhost port 8000 and does not exist while the editor is closed.
`.codex/config.toml` is the Codex project client; `.mcp.json` files retain the
same endpoint for clients that use that format. `docs/EDITOR_AUTOMATION.md`
owns the exact operator sequence and troubleshooting boundary.

## This repository's own structure

```text
rewind-reboot/
├── AGENTS.md
├── README.md
├── .codex/                project-scoped Codex MCP configuration
├── Rewind/                UE 5.8 runtime proof plus editor-only automation module
├── docs/                  live working state
│   ├── design/            game rules, one ownership area per document
│   ├── acceptance/        proof criteria
│   ├── baseline/          frozen working-model provenance
│   └── concepts_sandbox/  legacy import and excluded ideas
```

Three levels coexist and must not be confused:

| Level | Meaning |
| --- | --- |
| `docs/` excluding sandbox and baseline | This project's live working state |
| `docs/baseline/` | Provenance of the docs-first working model. Frozen, unedited, not authority |
| `docs/concepts_sandbox/` | Non-authority. Legacy RE:WIND and other excluded ideas |
