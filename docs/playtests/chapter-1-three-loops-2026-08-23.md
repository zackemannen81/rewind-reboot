# Chapter 1 Three Loops — 2026-08-23

Status: Complete

## Build and method

- Branch: `rew-0007/chapter-1-spatial-chain`
- Base commit: `2056d9d2e29a631b8f1417a704f35bd141880002`
- Build: Unreal Editor Win64 Development, UE 5.8, successful after the final
  blockout seam correction.
- Map: `/Game/Maps/FiveLoops`
- Input: project-owned MCP PIE input. Exact game-time holds and one queued
  sequence per formal route removed MCP response latency from route timing.
- Starting state: `Rewind.CleanSave`, followed by the three formal loops below.
- Evidence source: named `LogRewind` transitions and possessed-player state.
  Calibration and diagnostic PIE loops between formal B and C are excluded;
  they tested route input and ended without being used as acceptance evidence.

## Formal Loop A — learn the radio code

The clean-save start logged knowledge `0` and Anchor `None`. The player stayed
at the accepted radio channel through a complete 20-second sequence:

| Event | Elapsed loop time |
| --- | ---: |
| Sequence start | 50.13 s |
| Digit `7` | 54.13 s |
| Digit `3` | 59.13 s |
| Digit `1` | 64.13 s |
| Digit `2` | 69.13 s |
| Complete; `radio_code_7312` obtained | 70.13 s |

The loop ended with reason `Death` at 99.13 s. The session write contained one
knowledge fact and no Anchor. This establishes that information survives the
loop while the world starts from Baseline.

## Formal Loop B — spend the fuse on the gate

Loop B began with knowledge `1` and Anchor `None`. A single uninterrupted
266.9-second queued route produced these named events:

| Event | Elapsed loop time |
| --- | ---: |
| Fuse picked up | 9.33 s |
| 4C lock auto-submitted from learned code | 17.00 s |
| Stair route entered | 29.33 s |
| Entrance reached | 85.00 s |
| Courtyard socket reached | 121.34 s |
| Generator online; gate opened | 210.00 s |
| `courtyard_gate_open` commit accepted | 211.34 s |
| Loop ended, reason `Timer` | 240.00 s |

No Transit Hub entry occurred. The session write contained knowledge `1` and
Anchor `courtyard_gate_open`. The failure was the intended resource-and-route
consequence: the only fuse was carried to the courtyard, so the building lift
was unavailable and the measured route consumed the loop. It was not an
unrelated obstacle or a slow manual input sequence.

## Formal Loop C — use the held change

The next acceptance loop began with knowledge `1` and the held Anchor active.
World apply logged the courtyard gate open before play. The same one fuse was
therefore free for its second use:

| Event | Elapsed loop time |
| --- | ---: |
| Fuse picked up | 9.33 s |
| 4C lock auto-submitted | 17.00 s |
| Fuse seated in building socket | 23.67 s |
| Lift descent started | 26.00 s |
| Lift reached entrance | 32.00 s |
| Patrol phase clear | 60.00 s |
| Patrol phase covered | 80.00 s |
| Turnstile opened | 90.00 s |
| Transit Hub entered | 90.67 s |

The final possessed-player position was
`(13847.656, -144.939, 98.15)`. All held input was released and PIE stopped
cleanly. The held Anchor did not teleport the player or grant a new action: it
removed the generator trip, left the fuse available for the lift and thereby
released enough time to reach Transit Hub.

## Measurements

| Measurement | Result | Required | Outcome |
| --- | ---: | ---: | --- |
| Three-flight stairs, fourth floor to entrance | 55.67 s | — | Observed |
| Powered lift, fourth floor to entrance | 6.00 s | — | Observed |
| Stairs minus lift | 49.67 s | at least 30.00 s | Pass |
| Loop B versus Loop C hub result | no entry by 240.00 s vs entry at 90.67 s | B cannot, C can | Pass |
| Naive minus learned route | at least 149.33 s | at least 30.00 s | Pass |

The construction model independently reports a 50.40-second minimum stair
route, a 6.00-second lift, a 260.40-second Loop B and a 46.00-second learned
route. The played times are the acceptance evidence; the model explains why
the result is stable by construction.

## Camera and baseline evidence

Enumeration logged eleven complete authored regions: Apartment4C,
FourthFloorHallway, LiftShaft, Stairs4To3, Floor3Landing, Stairs3To2,
Floor2Landing, Stairs2ToEntrance, EntranceHallway, Courtyard and TransitHub.
Baseline inspection logged the fuse at rest, lift at 4C and unpowered,
generator offline and gate closed.

All six discoverable `Rewind.*` automation tests passed together, 6/6 with no
errors, in 0.0411 seconds. This includes the Chapter 1 route-budget and
vertical-route tests, design identifier consistency, both loop-clock tests and
session-bucket save coverage.

## Acceptance mapping

| Criterion | Evidence in this record | Result |
| --- | --- | --- |
| FL-01 | Clean save began with no knowledge and no Anchor | Pass |
| FL-04 | Loop A retained `radio_code_7312` across the boundary | Pass |
| FL-06 | Loop B world began from Baseline while knowledge persisted | Pass |
| FL-07 | Loop B committed the gate Anchor and Loop C applied it | Pass |
| FL-09 | One fuse had two exclusive observed uses across B and C | Pass |
| FL-12 | Loop B could not enter the hub; held-state Loop C could | Pass |
| FL-13 | The authored chain required separate information, commit and held-state loops | Pass |
| FL-14 | Learned/held route beat the naive route by at least 149.33 s | Pass |

## Not verified

No packaged build was made. Frame rate was not varied. This record does not
claim finished art, visual fidelity to the concept targets, or that repeating
the route is enjoyable. The diagnostic loops excluded above are not evidence.
