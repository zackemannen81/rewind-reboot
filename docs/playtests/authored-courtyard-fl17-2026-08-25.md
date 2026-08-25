# Authored courtyard FL-17 — 2026-08-25

Status: Complete

## Build and method

- Branch: `grok/rew-0020-authored-courtyard`
- Build: Unreal Editor Win64 Development, UE 5.8. Editor-closed `RewindEditor`
  Win64 Development succeeded (final confirm 1.78 s, target up to date after
  the last C++ change). Headless `Rewind.*` afterwards: 22/22 Success,
  `TEST COMPLETE. EXIT CODE: 0`.
- Map: `/Game/Maps/FiveLoops_Stairwell_Blockout`
- Input: project-owned MCP PIE-input toolset. Every evidence run ended with
  `ReleaseAllKeys` before `StopPIE`.
- Developer verification commands used: `Rewind.PlacePlayerForVerification`
  and, for the power outcome only, `Rewind.SeatFuseForVerification courtyard`.
  Those are not player actions.

## Spatial attach

Standard PIE spawned in `Apartment4C_Region` at `(0, 1580, 1298.15)`, camera
`(750, 1330, 1330)`, FOV `37.497356`. `LogRewind` wrote:

```text
Camera regions: 9 authored after courtyard attach
Checkpoint: GroundFuseGate at authored hall-to-courtyard seam
AuthoredCourtyard: attached at GroundHall +Y seam Y=1500  threshold=(-430, 2700, 150)
```

`CourtyardApproach_Region` abuts `GroundHall_Region` at Y `1500`. The
courtyard region begins at Y `2700`. Named automation
`Rewind.AuthoredCourtyard.RegionAbutment` and
`Rewind.AuthoredCourtyard.SkipProofLayoutSpawnsCheckpoint` assert that
relationship and that SkipProofLayout maps do not force X = 4110.

## FL-17 fail — no power, no Anchor

Starting pose: `Rewind.PlacePlayerForVerification threshold` at
`(-430, 2540, 96)` in `CourtyardApproach_Region`. Walked screen-right (`D`)
along +Y.

```text
t=142.76  Checkpoint GroundFuseGate: FAIL  latching rewind
t=142.76  REWIND LATCH  reason=CausalContract  checkpoint=GroundFuseGate  prelude=2.00s
t=143.09  LOOP BREAK  interference=0.17 (elapsed loop time)
t=144.77  LOOP END  reason=CausalContract
Player: placed at loop-start pose (0, 1580, 1296)
t=0.00  LOOP START
```

Prelude duration 2.00 s, inside [1.0, 3.0]. The next loop started in 4C.
Later repeats of the same crossing produced the same latch (t=104.43, 108.73,
157.11, 129.33, 67.12, 75.00, 118.96, 81.33) with the same 2.00 s prelude.

Capture during a later prelude, player at Y `2801` in `Courtyard_Region`,
camera yaw 180 FOV 42:

- [`authored-courtyard-fl17-2026-08-25/interference-prelude.png`](authored-courtyard-fl17-2026-08-25/interference-prelude.png)

The frame shows the courtyard blockout, the player silhouette, and the
loop-break fringe/desaturation. The plaza floor is over-bright under the
first-pass moon key; that is a look note, not a missing latch.

Held keys were released after the run.

## FL-17 Anchor pass

`Rewind.SetActiveAnchor courtyard_gate_open` wrote the held gate and restarted
the loop. Place at the threshold, walk +Y.

```text
t=92.33  SetActiveAnchor: courtyard_gate_open
Gate: OPEN, held by anchor courtyard_gate_open
t=17.00  PlacePlayerBeforeGroundFuseGate: (-430, 2540, 96)
t=33.00  Checkpoint GroundFuseGate: pass
```

The player reached `Courtyard_Region` at Y `2868` with no rewind latch on that
crossing. Held keys released.

## FL-17 power pass

This-loop 4C fuse pickup was not landed in PIE: the fuse sits at `(-250, 1180,
1350)`, 5 cm outside `Apartment4C_Region`'s X `[-245, 370)`, beside the
building socket. Interact from the nearest contained pose hit the empty
building socket instead.

The power outcome was therefore seated with
`Rewind.SeatFuseForVerification courtyard`, then the same threshold walk:

```text
t=16.33  Fuse: seated in the courtyard socket
t=16.33  SeatFuseForVerification: courtyard
t=48.33  Checkpoint GroundFuseGate: pass
```

The player reached `Courtyard_Region` at Y `2868` with no rewind latch. Held
keys released before stop.

## What this does not show

- FL-18 (Anchor commit as a loop boundary) was not this charter's PIE gate.
- The player did not carry the fuse from 4C to the socket by walking.
- Street and Transit Hub interiors past the Transit exit.
- Final art. The moon key blows the plaza floor under the authored interior
  exposure; that is first-pass, not a lighting decision.
- Packaged build, frame-rate variation.

## FL-17

The three checkpoint outcomes FL-17 names were observed in PIE on this
authored map: fail latches a 2.00 s prelude with visible interference and
restarts in 4C; courtyard-socket power passes; an active `courtyard_gate_open`
Anchor passes. The power seating used a developer command because 4C pickup
did not land. That is this-loop ground-floor power as the predicate reads it,
not a claim that Loop B is a played route from 4C.
