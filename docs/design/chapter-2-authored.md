# Chapter 2 authored content: Conduit

Status: Accepted
Owns: Chapter 2 fiction, spaces, authored clocks, puzzles, antagonist
behaviour, false-information rules, chapter-entry state and exit conditions
Does not own: general loop and determinism rules, Anchor semantics, save and
session semantics, player-message presentation, Echo, Chapter 1, final
narrative copy, final art, sound production or camera implementation

Chapter 2 begins where Chapter 1 points: inside Transit Hub, below the city,
with the transit system still running for purposes that do not include the
Returner.

The legacy seed supplied three phrases: sequence puzzles, false information,
and reactive AI that learns from player actions. They were prompts, not rules.
This document restates the first two and deliberately rejects the literal
third. Under [`ADR-0002`](../adr/ADR-0002_world-determinism.md), no world object
may remember earlier-loop play or read loop count.

Numbers below are authored for this chapter. They are not true because legacy
material proposed them. They are true because this document says so.

## What the chapter is about

Conduit is about learning whose truth a system serves.

The railway does not lie. `LINE CLEAR` means that a block is clear for a train,
which also means its traction rail is live. `TIMETABLE: EAST` names the planned
platform, not the platform the interlock will select when a maintenance trolley
occupies the route. The signs are accurate in railway terms and dangerous when
read as advice for a person walking the track.

The player first treats the station as a source of instructions. The chapter
becomes solvable when they treat it as a machine whose outputs have owners,
latencies and fixed relationships. That change is produced mechanically:

- the red `BLOCK OCCUPIED` interval contains the only safe walking window;
- the departure board corrects itself too late to cross platforms after the
  correction;
- vent warnings spell the relay order instead of explaining it;
- the Conductor's response opens the door it was meant to protect.

Nothing becomes easier because the world recognizes repetition. The same
signals, trolley, vents, train and Conductor are present on every loop. The
player becomes faster because they stop obeying those systems as a passenger
and start reading them as an operator.

The tracked transit-map poster and `TRANSIT` neon may provide visual continuity
from Chapter 1. Brick, ivory ceramic tile, paving stone and painted black metal
fit the spaces already established. They are presentation resources, not
puzzle authority. Every critical state also needs a word, shape, position,
motion or sound cue; colour alone may not carry it.

## The determinism decision

This chapter chooses route 1 for the legacy "reactive AI" tension:

> The Conductor appears to learn because a fixed schedule and fixed same-loop
> responses intersect with a player whose route changes as their knowledge
> changes.

The Conductor reads elapsed loop time, current pose, current line of sight and
detections made during this loop. At the first detection it takes one authored
response branch selected by the detection zone. That state is LoopWorld and is
discarded at rewind. It never reads loop count, a saved route, an earlier-loop
detection, a death location or a player tendency.

A player who reaches the relay aisle at `t = 64` after learning the tunnel and
purge timings meets an east scan that a slower player never saw. When the
player deliberately triggers that scan, the same response that previously
killed them opens the control-room door for four seconds. The apparent
escalation comes from a new arrival time and a new this-loop action, not from
world memory.

This is fully inside ADR-0002, so no ADR-0013 is proposed.

## Chapter entry and loop start

Chapter 2 is entered from a session that has reached Transit Hub through
Chapter 1. The chapter-entry state therefore carries the existing
`radio_code_7312` knowledge fact and active `courtyard_gate_open` Anchor. The
Anchor remains in the save but targets no object in Conduit's Baseline, so it
does not alter a Chapter 2 object.

Every Conduit loop starts at `t = 0` on the Intake Platform, two metres inside
the closed street-side turnstile. The Returner's body faces the service hatch.
The authored clocks below all start at their phase zero. Starting Chapter 2 is
not a clean save; acceptance uses this named chapter-entry state.

The chapter introduces no Anchor, no Insight and no second persistence route.

## Spaces and connections

The authored space has five connected areas with different jobs and readings.

1. **Intake and Departure Platforms.** Two platform faces, East and West,
   share a high crossover stair. The Intake spawn, block indicator, departure
   board and outbound train are visible here. The stair route between the two
   boarding marks is 2,000 cm and takes 10 seconds at 200 cm/s.
2. **East Running Tunnel.** A 1,800 cm track-level crossing connects the
   service hatch to the conduit mouth. It takes 9 seconds at 200 cm/s. Its
   traction rail and maintenance trolley make the block indicator's meaning
   physical.
3. **Pressure Conduit.** Three 600 cm purge chambers, A, B and C, connect the
   tunnel to the relay room. Each chamber takes 3 seconds to cross at 200 cm/s.
   Warning vanes, pipe knocks and relay lamps share one pressure schedule.
4. **Relay Room and Inspection Aisle.** The four-relay bank authenticates the
   signal system. A 600 cm approach takes 3 seconds from the relay gate to the
   east-scan bait mark. A separate 600 cm blind bypass takes 3 seconds from the
   bait recess to the control door. The Conductor patrols the visible aisle.
5. **Signal Control Room.** The route console overlooks both platforms. A
   separate 1,400 cm stair returns from the console to the West boarding mark
   in 7 seconds. This route never enters the East tunnel or purge chambers in
   reverse.

The service hatch is a one-way 150 cm drop. The player may inspect it for any
duration before descending, but cannot return to the Intake Platform by that
route in the same loop. The control-room stair is the only return to the
platforms. The topology makes the learned route a circuit rather than repeated
backtracking:

```text
Intake Platform
  -> one-way service hatch
  -> East Running Tunnel
  -> Pressure Conduit A / B / C
  -> Relay Room
  -> Conductor-controlled door
  -> Signal Control Room
  -> West Platform
  -> East/West crossover when needed
  -> outbound train
```

Walls, shutters and live rail span their walkable boundaries. A player cannot
walk around a closed gate, a purge volume, the trolley sweep or platform-screen
doors.

## Movement and time

The Chapter 1 walk speed remains **200 cm per second**. The chapter has no
global duration. Elapsed time begins at zero and drives every schedule below.
Missing a routine cycle costs waiting, not an unexplained rewind.

The four independent periods are:

| Schedule | Period | Phase-zero event |
| --- | ---: | --- |
| Maintenance trolley and East traction | 72 seconds | block becomes occupied and traction isolates |
| Pressure purge and relay lamps | 60 seconds | all chambers are quiet |
| Conductor patrol and scan | 48 seconds | Conductor begins on the control balcony |
| Terminus passenger train | 180 seconds | board begins the next scheduled-East countdown |

Their joint state repeats every:

```text
lcm(72, 60, 48, 180)
= 2^4 * 3^2 * 5
= 720 seconds
```

No loop ends at 720 seconds. The least common multiple is an audit horizon:
matching Baseline, Anchors and this-loop input produce the same four-schedule
configuration at `t` and `t + 720`.

## Puzzles and events

The chapter contains nine discrete authored challenges. Solving one does not
count as a variation of another.

### 1. The block indicator

The East service hatch carries a two-state railway indicator:

| Trolley phase (`t modulo 72`) | Display | Traction | Meaning for the player |
| --- | --- | --- | --- |
| `[0, 18)` | `BLOCK OCCUPIED` | isolated | potentially walkable after the trolley passes |
| `[18, 72)` | `LINE CLEAR` | energized | lethal at track level |

Both statements are true. `LINE CLEAR` means clear for rail traffic, not safe
for a pedestrian. The display is diegetic signage, not a player message and
not the `t=` instrument.

Continuous contact with energized traction for **0.25 seconds** is death. The
rail state is a direct function of trolley phase and never of loop count.

### 2. The maintenance trolley

At each 72-second phase zero:

- a warning bell sounds during phases `[0, 2)`;
- the trolley's lethal swept volume crosses the walking line during `[2, 6)`;
- the trolley clears the walking line at phase `6`;
- block occupation and traction isolation end at phase `18`.

The usable East-tunnel window is therefore `[6, 18)`, exactly 12 seconds. The
tunnel takes 9 seconds to cross, leaving 3 seconds of margin. A player entering
at phase `9` exits at phase `18`; entering after phase `9` cannot finish before
traction energizes.

Contact with the trolley's swept volume is immediate death. Waiting for the
red state is insufficient; the player must also distinguish warning, passage
and clearance inside it.

### 3. The pressure conduit

Pressure phase is `t modulo 60`. The three chambers purge in sequence:

| Chamber | Warning | Lethal jet |
| --- | --- | --- |
| A | `[10, 12)` | `[12, 20)` |
| B | `[20, 22)` | `[22, 30)` |
| C | `[30, 32)` | `[32, 40)` |

All three are quiet during `[40, 60)`. Continuous presence in a lethal jet
for **0.75 seconds** is death. Each 600 cm chamber takes 3 seconds to cross.

A player who leaves the first trolley window at `t = 15` can traverse without
guessing:

```text
wait for A to stop at t = 20; cross A, t = 20..23
wait for B to stop at t = 30; cross B, t = 30..33
wait for C to stop at t = 40; cross C, t = 40..43
```

The waits are caused by the observed pressure sequence. They are not a global
tax, and a later 60-second phase offers the same route.

### 4. The warning-lamp sequence

Each pressure warning lights the relay number assigned to that valve:

| Pressure phase | Lit relay | Duration |
| --- | ---: | ---: |
| `[10, 12)` | 2 | 2 seconds |
| `[20, 22)` | 4 | 2 seconds |
| `[30, 32)` | 1 | 2 seconds |
| `[40, 42)` | 3 | 2 seconds |

The fourth light is the equalization pulse after Chamber C. The complete order
is **2, 4, 1, 3**. It repeats every 60 seconds and is visible from the pressure
conduit. It is not written to the save or printed as a message.

### 5. The relay bank

The relay-room bank accepts `2, 4, 1, 3` and no other order. A press registers
at the interaction instant. The maximum allowed gap between consecutive
presses is **3.0 seconds**, and the fourth press must land no more than **10.0
seconds** after the first.

A wrong press, an overlong gap or an overlong sequence clears the buffer and
locks the bank for **4.0 seconds**. This is recoverable and does not rewind.

A correct sequence sets the LoopWorld flag `relay_authenticated` and opens the
inspection-aisle gate for **15.0 seconds**. Authentication stays true until
loop end even after the gate closes. The player must use the open gate, but
does not have to repeat the sequence at the signal console.

### 6. The Conductor's baseline patrol

Conductor phase is `t modulo 48`:

| Phase | Position or movement | Scan window |
| --- | --- | --- |
| `[0, 8)` | control balcony | Control, `[2, 6)` |
| `[8, 12)` | north transfer | none |
| `[12, 24)` | east relay aisle | East, `[16, 20)` |
| `[24, 28)` | south transfer | none |
| `[28, 40)` | platform bridge | Platforms, `[32, 36)` |
| `[40, 48)` | return gallery | none |

A scan detects the player only inside its named zone with unobstructed line of
sight during that four-second window. Baseline motion and scans are functions
of elapsed loop time.

### 7. The Conductor's same-loop response

Let `d` be the first detection time in the current loop. The detection zone
selects one fixed 12-second response:

| Detection zone | Deterministic response from `d` |
| --- | --- |
| Control | console inhibited `[d+1, d+9)`; one-second control pulse `[d+6, d+7)` |
| East | control door unlocked `[d+4, d+8)`; east pulse `[d+10, d+12)` |
| Platforms | platform shutters closed `[d+2, d+12)`; one-second platform pulse `[d+8, d+9)` |

Contact with a pulse for **0.25 seconds** is death. New detections are ignored
while the 12-second response runs. Each branch ends on the corresponding
baseline route segment at `d + 12`, so the Conductor resumes the baseline
position for the current 48-second phase without a teleport or accumulated
delay.

The required exploit is the East response. Standing at the bait mark when the
East scan begins at `t = 64` gives `64 modulo 48 = 16`, so `d = 64`. The door
opens during `[68, 72)`. The blind bypass takes 3 seconds; leaving the bait
recess at `t = 65` reaches the door at `t = 68`, and the player is through
before it closes at `t = 72`. The east pulse does not begin until `t = 74`.

The Conductor did not remember where the player went on another loop. The
player deliberately caused a known response in this one.

### 8. The signal sequence and commit

The control console accepts three route inputs in this order:

```text
ISOLATE -> CROSS -> CALL
```

The order is drawn on the fixed relay schematic. Each interaction registers at
its instant. Consecutive inputs may be no more than **3.0 seconds** apart;
`CALL` must land no more than **8.0 seconds** after `ISOLATE`; and the player
must pull `COMMIT` within **2.0 seconds** after `CALL`.

`COMMIT` is the named `SignalInterlockCommit` causal checkpoint. Its predicate
is:

```text
relay_authenticated
&& INPUT_ORDER_IS_ISOLATE_CROSS_CALL
&& EVERY_INPUT_AND_COMMIT_DURING_TROLLEY_PHASE_[0,18)
&& PASSENGER_PHASE_NOT_[144,162)
```

Pulling `COMMIT` is labelled as a no-return breaker close. If the predicate is
false, the interlock burns out for this loop, rewind latches, and an exact
**2.0-second** perceptible prelude runs before loop-start apply. Editing the
buffer before pulling `COMMIT` is safe; the commit action is the irreversible
choice.

A valid commit sets one-shot LoopWorld `departure_access` for the next
passenger train. It is not an Anchor and does not survive rewind. One concrete
valid sequence is:

```text
t = 72: ISOLATE
t = 74: CROSS
t = 76: CALL
t = 78: COMMIT
```

All four actions are inside trolley occupation `[72, 90)`. Their gaps are 2,
2 and 2 seconds, `CALL - ISOLATE = 4`, and the Conductor is on the platform
bridge at `t = 78` because `78 modulo 48 = 30`.

### 9. The departure board and outbound train

Passenger phase is `t modulo 180`.

- The public board displays `TIMETABLE: EAST` and the time to the next train.
  It is explicitly a timetable, not a guarantee.
- At phase `144`, the interlock locks the actual platform.
- If the East block is occupied at phase `144`, the train diverts to West.
  Otherwise it uses East.
- The public board changes to the actual platform at phase `144`.
- The train enters at phase `150`, dwells during `[150, 162)`, opens its doors
  during `[152, 160)`, and departs at phase `162`.

The board countdown changes at integer-second boundaries. Before phase `150`
it is `ceiling(150 - passenger phase)`. It reports the current train during
`[150, 162)`. After phase `162` it counts to the next cycle's arrival as
`ceiling(330 - passenger phase)`, where 330 is 180 plus the next phase-150
arrival.

The timetable is sometimes operationally misleading and sometimes not. Train
approach locks occur at:

```text
t = 144 + 180n
```

Because `180 modulo 72 = 36`, trolley phase at successive locks alternates:

```text
n = 0: 144 modulo 72 = 0   -> East occupied -> West
n = 1: 324 modulo 72 = 36  -> East clear    -> East
n = 2: 504 modulo 72 = 0   -> East occupied -> West
n = 3: 684 modulo 72 = 36  -> East clear    -> East
```

The 720-second joint horizon therefore contains four passenger trains: two
divert West and two remain East. The board is not randomly wrong and is not
always wrong. A player who knows both clocks predicts which kind is next.

For the first train, the board corrects to West at `t = 144` and the doors open
at `t = 152`, eight seconds later. The platform crossover takes 10 seconds.
Since `10 > 8`, a player waiting at East cannot respond to the correction in
time. A player who predicts the trolley occupation can stand at West before
`t = 144`.

If `departure_access` is true, the platform-screen doors open with the next
train. The authorization is consumed by that train. Boarding during
`[152, 160)` completes Conduit and continues onward without a rewind.

If the authorized train's doors close at phase `160` without the player
aboard, the named `AuthorizedDepartureMissed` causal contract fails. The
one-shot authorization is spent and the route is impossible in that loop, so
an exact **2.0-second** prelude runs and the next loop starts. If no train was
authorized, an ordinary departure does not rewind the player.

## The learned route and its arithmetic

The authored fast route is possible from the first loop. The game never reads
loop count to unlock it. A player with all knowledge can perform this sequence:

```text
t = 0..6    wait for the trolley's swept volume to clear
t = 6..15   cross the 1,800 cm East tunnel
t = 15..20  wait for Chamber A
t = 20..23  cross A
t = 23..30  wait for Chamber B
t = 30..33  cross B
t = 33..40  wait for Chamber C
t = 40..43  cross C
t = 43..45  enter 2, 4, 1, 3 within the relay limits
t = 45..48  take the 600 cm approach to the bait recess
t = 48..64  wait for the East scan
t = 64      allow the first East detection
t = 65..68  take the blind bypass to the control door
t = 68..70  cross while the door is open [68, 72)
t = 72,74,76,78
             enter ISOLATE, CROSS, CALL, COMMIT in the isolation window
t = 78..85  take the 1,400 cm control stair to West
t = 85..152 wait at West while the board still predicts East until t = 144
t = 152..160 board the first authorized train
```

The route uses all four independent schedules. It reaches West 59 seconds
before the actual-platform correction and 67 seconds before door opening. The
waiting time is the cost of knowing which departure to call, not a hidden
whole-space deadline.

## The eight learning loops

These are shapes the space must support, not a script the player must obey. A
player with prior knowledge may combine them, and the game must not inspect
loop count to prevent that. Each has one new observation or application; none
exists merely to make the chapter longer.

**Loop A — railway truth is not pedestrian truth.** The player waits for the
red `BLOCK OCCUPIED` indication to become green `LINE CLEAR`, drops to track
level and dies after 0.25 seconds on live traction. They learn what `clear`
means to this system.

**Loop B — red still contains a moving hazard.** The player enters during red
but before phase `6`, meets the trolley swept volume and dies. They separate
block occupation from the safe sub-window `[6, 18)`.

**Loop C — one schedule is not the whole route.** The player crosses the
tunnel in nine seconds, reaches the pressure conduit and dies in a purge. They
learn that the vent clock is independent of the trolley clock.

**Loop D — warnings are also an instruction.** The player follows the three
quiet windows, observes `2, 4, 1, 3`, authenticates the relay or learns its
four-second rejection, then lingers into a Conductor scan and dies. The new
fact is the relay order and the existence of a separate watcher clock.

**Loop E — pursuit is a door cycle.** The player authenticates quickly,
triggers the East response, and is killed by the pulse because they treat the
opening door as escape rather than timing. They learn the four-second door
window and the ten-second lead before the pulse begins.

**Loop F — the signal breaker is a causal boundary.** The player reaches the
control room but pulls `COMMIT` without the correct isolation-window sequence.
`SignalInterlockCommit` fails, the two-second prelude makes the breaker failure
legible, and the loop restarts. They learn that the trolley's red state is also
the console's permission state.

**Loop G — the timetable is not the interlock.** The player makes a valid call
for the first train, returns to East because the board still says
`TIMETABLE: EAST`, sees the board correct at `t = 144`, and cannot finish the
10-second crossover before doors open at `t = 152`. At `t = 160`, the spent
one-shot departure latches the two-second rewind. They learn to predict the
diversion rather than react to the display.

**Loop H — knowledge reaches the exit.** The player executes the same world
from the same Baseline, deliberately causes the East response at `t = 64`,
commits the signal sequence at `t = 78`, prepositions at West before `t = 144`
and boards during `[152, 160)`. No stored upgrade, loop-count beat or
earlier-loop Conductor memory helped.

## State, persistence and reset

The buckets and apply order are exactly those in
[`world-state-model.md`](world-state-model.md).

**Baseline** contains the five-space topology, signs, default closed gates,
unpressed relays, empty input buffers, Conductor route, train timetable and the
four phase-zero states. Play never rewrites it.

**LoopWorld** contains trolley pose, traction power, purge jets, relay lamps,
relay input and authentication, gates and doors, Conductor pose, detection time
and response branch, signal input, `departure_access`, actual train platform,
train pose and platform-screen-door state. All are discarded at loop start.

**PlayerBody** contains the Returner's pose and traversal state. It is
discarded and replaced by the Intake Platform loop-start pose.

**Anchors** retain the existing `courtyard_gate_open` identifier in the save.
Conduit creates, commits and applies no new override.

**Knowledge** retains `radio_code_7312` and any already-seen message ids through
the existing save store. This chapter adds no stored knowledge fact. The
trolley window, vent order, relay order, scans, signal sequence and diversion
rule are knowledge held by the human player. They do not change world objects
and are not a hidden journal system.

At every loop start the apply order is Baseline, active Anchors, loop-start
PlayerBody, elapsed time zero, then simulation. An object-local reset that
retains `relay_authenticated`, a Conductor detection, an actual platform or
`departure_access` from the previous loop is a defect.

## Failure, rewind and completion

Conduit has no global timeout.

**Death ends the loop immediately.** Authored deaths are energized-traction
exposure for 0.25 seconds, trolley contact, purge exposure for 0.75 seconds and
Conductor-pulse exposure for 0.25 seconds. Ordinary player death remains in
the accepted death class.

**`SignalInterlockCommit` is a causal failure.** Invalid no-return commit runs
an exact two-second prelude and then applies loop start.

**`AuthorizedDepartureMissed` is a local-deadline causal failure.** It is
evaluated only at the authorized train's phase-160 door close. The one-shot
call and visible departure make the deadline authored and legible. It runs an
exact two-second prelude and then applies loop start.

Relay rejection, a closed door, a missed uncalled train, an ordinary scan that
does not hit the player, waiting and exploration do not end the loop. Their
situations remain solvable.

Boarding the authorized train completes the chapter and continues to the next
authored space without a rewind. Save/checkpoint treatment of that campaign
transition remains an owner decision below.

## Testable statements

1. Every Conduit loop begins on the Intake Platform at `t = 0`, and all four
   clocks begin at their stated phase zero.
2. No elapsed duration ends Conduit globally.
3. `BLOCK OCCUPIED` is displayed and traction is isolated at trolley phases
   `[0, 18)`; `LINE CLEAR` is displayed and traction is energized at
   `[18, 72)`.
4. The trolley sweep occupies phases `[2, 6)`, leaving the 12-second walking
   window `[6, 18)` for a nine-second tunnel crossing.
5. The purge warnings, lethal windows and relay flashes repeat at the stated
   60-second phases.
6. The relay accepts `2, 4, 1, 3` inside the stated gap and total limits and
   rejects other inputs with a four-second recoverable lockout.
7. The Conductor baseline repeats every 48 seconds. Matching detections at
   matching `d` produce the same 12-second response.
8. No Conductor state survives loop start and no Conductor decision reads loop
   count or earlier-loop input.
9. An East detection at `t = 64` opens the control door during `[68, 72)` and
   does not pulse the East aisle until `[74, 76)`.
10. A signal sequence at `t = 72, 74, 76, 78` passes
    `SignalInterlockCommit`; an invalid commit latches a two-second rewind
    prelude.
11. Passenger approach locks repeat every 180 seconds at phase `144`; doors
    open during `[152, 160)`.
12. Approach locks at `t = 144` and `504` divert West, while `t = 324` and
    `684` remain East.
13. The board continues to state scheduled East until the phase-144 approach
    lock, when it changes to the actual platform.
14. The eight seconds from board correction to door opening are less than the
    10-second platform crossover, so the first West diversion must be
    predicted to board it from the correct side.
15. Missing an authorized departure at phase `160` runs a two-second prelude;
    an uncalled departure does not rewind.
16. Relay, Conductor, signal, train and player-body state all return to
    Baseline on rewind. Existing knowledge and Anchors persist through their
    accepted buckets, and Conduit creates neither a new fact nor a new Anchor.
17. A fully informed player can complete the chapter in one loop and board the
    first train during `[152, 160)`.

## Open questions for the owner

1. **What is the Conductor in the fiction?** Recommendation: keep it an
   autonomous station-security body with no voiced personality. That makes its
   fixed response table readable as infrastructure and avoids implying an
   off-screen intelligence that might remember the player. The mechanics do
   not depend on this answer.
2. **What persists when the train is boarded?** Recommendation: add a campaign
   progression fact only when a campaign-save owner defines chapter
   completion and resume semantics. This document intentionally does not
   invent `chapter_2_complete` inside the existing knowledge store.
3. **When may implementation begin?** Recommendation: keep Chapter 2 code,
   maps and assets blocked until the owner explicitly moves implementation out
   of `PROJECT_BRIEF.md`'s phase non-goals and charters a bounded build task.
   Accepted design authority is not implementation evidence or permission.
