# Chapter 2 Conduit acceptance

Status: Accepted
Owns: what a later Conduit implementation must demonstrate before Chapter 2
may be called playable
Does not own: game rules. Every criterion below cites an owning design
document.

Run from the Chapter 2 entry state defined by
[`chapter-2-authored.md`](../design/chapter-2-authored.md): the Chapter 1 route
has reached Transit Hub, `radio_code_7312` is true,
`courtyard_gate_open` is active, and the next loop starts on the Conduit Intake
Platform. This is not a clean-save setup.

No Conduit implementation exists at the time these criteria are accepted. A
document review cannot pass them. A later playtest record must mark each
identifier pass or fail and name its evidence.

Identifiers are permanent. Do not renumber.

## Criteria

### C2-01 — Chapter-entry baseline

At `t = 0`, the Returner is at the Intake Platform start pose. The service
hatch is available; relay authentication and departure access are false; the
Conductor is at its phase-zero position; trolley, purge and passenger schedules
are at phase zero.

Owner: [`chapter-2-authored.md`](../design/chapter-2-authored.md)

### C2-02 — Connected authored circuit

The player can traverse one continuous circuit in this order: Intake Platform,
East Running Tunnel, all three Pressure Conduit chambers, Relay Room and
Inspection Aisle, Signal Control Room, West Platform, then the outbound train.
The one-way hatch prevents returning directly to Intake, and no closed gate,
hazard or platform door can be bypassed around its walkable boundary.

Owner: [`chapter-2-authored.md`](../design/chapter-2-authored.md)

### C2-03 — No global loop deadline

Idle beyond `t = 720` with no death, failed causal checkpoint or successful
Anchor commit does not rewind. The four schedules continue from elapsed loop
time.

Owner: [`chapter-2-authored.md`](../design/chapter-2-authored.md)

### C2-04 — Operationally truthful false information

The East indicator says `BLOCK OCCUPIED` only while the block is occupied and
traction is isolated, and says `LINE CLEAR` only while the line is clear for
rail traffic and traction is energized. The timetable board identifies its
East value as scheduled and switches to the actual platform at approach lock.
Neither display chooses a random or loop-indexed lie.

Owner: [`chapter-2-authored.md`](../design/chapter-2-authored.md)

### C2-05 — Maintenance schedule

Across two trolley cycles, the warning occupies phases `[0, 2)`, the lethal
trolley sweep `[2, 6)`, block occupation and traction isolation `[0, 18)`, and
energized `LINE CLEAR` state `[18, 72)`. The phase boundaries repeat 72 seconds
apart.

Owner: [`chapter-2-authored.md`](../design/chapter-2-authored.md)

### C2-06 — East-tunnel window

At the 200 cm/s authored walk speed, entry at trolley phase `6` crosses the
1,800 cm tunnel by phase `15`, before traction returns. Entry after phase `9`
cannot complete the nine-second crossing before phase `18`. Energized-rail
contact for 0.25 seconds and trolley contact each cause death and loop start.

Owner: [`chapter-2-authored.md`](../design/chapter-2-authored.md)

### C2-07 — Pressure schedule

For two 60-second cycles, Chamber A warns at `[10, 12)` and purges at
`[12, 20)`; B warns at `[20, 22)` and purges at `[22, 30)`; C warns at
`[30, 32)` and purges at `[32, 40)`. A player may cross each 600 cm chamber in
three seconds outside its lethal window. Jet exposure for 0.75 seconds causes
death and loop start.

Owner: [`chapter-2-authored.md`](../design/chapter-2-authored.md)

### C2-08 — Warning-lamp sequence

Relay lamps 2, 4, 1 and 3 illuminate for two seconds at pressure phases
`[10, 12)`, `[20, 22)`, `[30, 32)` and `[40, 42)` respectively. The same order
and phases recur after 60 seconds with no saved or printed solution.

Owner: [`chapter-2-authored.md`](../design/chapter-2-authored.md)

### C2-09 — Relay bank

Inputs `2, 4, 1, 3` with no gap above 3.0 seconds and no more than 10.0 seconds
from first to fourth set `relay_authenticated` and open the aisle gate for 15.0
seconds. A wrong input or exceeded limit clears the buffer, enforces a
four-second lockout and does not rewind.

Owner: [`chapter-2-authored.md`](../design/chapter-2-authored.md)

### C2-10 — Conductor baseline clock

Without a detection, Conductor position and scan zones match the six authored
48-second phase intervals for two complete cycles. Control scans only at
`[2, 6)`, East only at `[16, 20)`, and Platforms only at `[32, 36)`.

Owner: [`chapter-2-authored.md`](../design/chapter-2-authored.md)

### C2-11 — Same-loop response table

Matching first detections at time `d` in Control, East and Platforms produce
the exact door, inhibition, shutter and pulse intervals stated for that zone.
Each response lasts 12 seconds, ignores further detections during that time and
rejoins the baseline route at `d + 12`.

Owner: [`chapter-2-authored.md`](../design/chapter-2-authored.md)

### C2-12 — East-response exploit

With the player visible at the East bait mark when `t = 64`, first detection
occurs at Conductor phase `16`; the control door is open during `[68, 72)`; a
three-second bypass begun at `t = 65` reaches it at `t = 68`; and the East
pulse does not begin before `t = 74`.

Owner: [`chapter-2-authored.md`](../design/chapter-2-authored.md)

### C2-13 — The world does not learn the player

Two loops sharing Baseline and Anchors and receiving the same inputs through a
chosen `t` have matching Conductor pose, detection state and response. A loop
with a previous detection followed by a loop with no detection has no retained
alert, route choice, death location or scan change. No Conduit world object
reads loop count.

Owner: [`loop-and-determinism.md`](../design/loop-and-determinism.md)

### C2-14 — Valid signal commit

After relay authentication, interactions `ISOLATE`, `CROSS`, `CALL`, `COMMIT`
at `t = 72, 74, 76, 78` satisfy all input gaps, the total sequence limit, the
commit limit, trolley occupation and passenger-route availability. The commit
sets one-shot `departure_access` and does not rewind.

Owner: [`chapter-2-authored.md`](../design/chapter-2-authored.md)

### C2-15 — Invalid signal commit

At `SignalInterlockCommit`, each independently invalid case — no relay
authentication, wrong order, an exceeded timing limit, an action outside
trolley phase `[0, 18)`, or passenger phase `[144, 162)` — latches rewind. The
prelude lasts exactly 2.0 seconds and cannot be canceled by another input.

Owner: [`chapter-2-authored.md`](../design/chapter-2-authored.md)

### C2-16 — Passenger train clock

For two 180-second cycles, approach locks occur at phase `144`, train entry at
`150`, dwell at `[150, 162)`, open doors at `[152, 160)` and departure at
`162`. The board countdown changes at integer-second boundaries and changes
from timetable to actual platform at phase `144`.

Owner: [`chapter-2-authored.md`](../design/chapter-2-authored.md)

### C2-17 — Predictable diversions

Within one 720-second observation, approach locks at `t = 144` and `504` select
West because trolley phase is zero, while locks at `t = 324` and `684` select
East because trolley phase is 36. Repeating the observation from matching
state produces the same four selections.

Owner: [`chapter-2-authored.md`](../design/chapter-2-authored.md)

### C2-18 — Correction is too late to chase

On the first diverted train, the board corrects to West at `t = 144` and doors
open at `t = 152`. A player starting at the East boarding mark at correction
cannot finish the 10-second, 2,000 cm crossover by door opening. A player who
prepositions at West can board during `[152, 160)`.

Owner: [`chapter-2-authored.md`](../design/chapter-2-authored.md)

### C2-19 — Authorized departure miss

With `departure_access` true, failing to board the called train by phase `160`
consumes the authorization, fails `AuthorizedDepartureMissed`, runs an exact
two-second perceptible prelude and starts the next loop. A phase-160 door close
with no called departure does not rewind.

Owner: [`chapter-2-authored.md`](../design/chapter-2-authored.md)

### C2-20 — Learned one-loop completion

From chapter-entry state, a player who already knows every sequence can cross
the first trolley window, pass the purge chambers, authenticate the relay,
cause the East response at `t = 64`, enter the valid signal sequence by
`t = 78`, reach West by `t = 85`, and board during `[152, 160)`. Boarding
continues onward without a rewind.

Owner: [`chapter-2-authored.md`](../design/chapter-2-authored.md)

### C2-21 — LoopWorld resets

After a loop with changed relay, gate, Conductor, console, authorization and
train state, the next loop at `t = 0` restores every one to Baseline and places
the player at Intake. No object-local reset overwrites the accepted Baseline,
Anchor, PlayerBody and time apply order.

Owner: [`world-state-model.md`](../design/world-state-model.md)

### C2-22 — Persistence boundary

Across rewind and session load, `radio_code_7312` and
`courtyard_gate_open` remain as accepted knowledge and Anchor state. No
Conduit relay order, detection, route input, departure authorization or actual
platform is stored. Conduit commits no second Anchor and adds no stored
knowledge fact.

Owner: [`chapter-2-authored.md`](../design/chapter-2-authored.md)

### C2-23 — Eight distinct learning runs

A chapter-entry playtest can record the eight authored run shapes A through H:
live-rail semantics, trolley sub-window, independent purge, warning order,
East-response door timing, invalid signal commit, late timetable correction,
then informed completion. Each run's new observation is named, and no observed
world change is attributed to loop count.

Owner: [`chapter-2-authored.md`](../design/chapter-2-authored.md)

### C2-24 — Joint-clock repeat

With matching Baseline, Anchors and no this-loop input, trolley, purge,
Conductor and passenger configuration at `t` matches configuration at
`t + 720` for at least one chosen `t` that is not a phase boundary.

Owner: [`chapter-2-authored.md`](../design/chapter-2-authored.md)

### C2-25 — Critical states are not colour-only

Block occupation, traction danger, purge warning, relay identity, Conductor
scan, actual platform and open boarding state are each readable through at
least one non-colour channel such as text, symbol, position, motion or sound.
The test records which channel carries each state.

Owner: [`chapter-2-authored.md`](../design/chapter-2-authored.md)

## What this does not claim

Passing these criteria would demonstrate the authored Chapter 2 chain. It
would not demonstrate final art, final audio, localisation, campaign save
semantics, Chapter 3 continuity, enjoyment or a packaged build unless a later
task adds those gates explicitly.
