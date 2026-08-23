# Chapter 1 authored content for the first product proof

Status: Accepted
Owns: the authored space, timings and puzzle facts the Five Loops Test
needs
Does not own: loop and determinism rules, Anchors, save, apply order, Echo,
anything in Chapters 2 to 5

This is not Chapter 1. It is the subset of Apartment 4C, courtyard, street
and Transit Hub that the first product proof requires. Other GDD detail
stays in the sandbox until a later task restates it.

Numbers below are restated for this proof. They are not true because the
Unity scripts used them, and they are not true because the GDD wrote them.
They are true because this document says so.

## Space and spawn

The space is four connected areas, in access order: Apartment 4C,
courtyard, street, Transit Hub.

The loop-start pose is in Apartment 4C.

## Movement and time

Revised 2026-08-23 under REW-0004. The previous values were 420 seconds and
an unauthored walk speed inherited from the Unreal third-person template.
They are recorded here because
[`five-loops-2026-08-22.md`](../playtests/five-loops-2026-08-22.md) is
evidence taken under them and remains true of that build.

The player's walk speed is **200 cm per second**. It was 500, which is a
sprint and was never a decision. Travel time is a design resource in a game
about repeating a route, and at 500 it rounded to nothing.

The 240-second loop duration was the accepted rule implemented and played by
REW-0007. It remains historical evidence for that build and is no longer a
Chapter 1 rule. Under
[`ADR-0009`](../adr/ADR-0009_event-driven-loop-termination.md), elapsed time
continues to drive authored local clocks but does not end this space globally.

## The knowledge saving

**Knowledge advances the causal frontier.** A player who has not satisfied the
ground-floor power contract rewinds at the courtyard threshold. A player who
knows how to satisfy it reaches the generator and the Anchor boundary. A player
with the held gate reaches Transit Hub.

Elapsed time still measures route mastery and decides which patrol and
turnstile windows a successful route meets. It no longer decides whether the
space eventually gives the player another attempt.

The layout consequence from ADR-0008 still holds: **an Anchor must release a
journey, not a button press.** The ground-floor fuse box and generator belong
off the learned lift route. The held gate removes that obligation and leaves
the one fuse available for the building socket.

## Radio and code

A radio in Apartment 4C is present at `t = 0`. It broadcasts on the loop
clock, like the patrol and the turnstile, and **it does not depend on loop
count**. The imported GDD makes it clearer on later loops; that version is
not restated here, because a world object that reads loop count is
forbidden by [`ADR-0002`](../adr/ADR-0002_world-determinism.md).

The escalation is in the player instead. What changes between loops is that
the player knows the code, knows which channel carries it, and knows when
in the loop it is spoken. None of that is stored; it is learned, which is
what this game says progression is.

**Channels.** The radio has channels and exactly one carries the code
sequence. The others carry static. Selecting a channel is a single
interaction. Which channel is the right one is not a stored fact and is
never written to a save; the player remembers it, exactly as they remember
the patrol's timing.

**The sequence speaks four digits.** The code sequence takes **20 seconds**
of elapsed loop time and repeats every **50 seconds**. Phase is `t` modulo
50. The four digits of `7312` are spoken at phases **4, 9, 14 and 19**, in
order.

**A digit heard is a digit kept.** A player within range when a digit is
spoken has that digit. It is not stored anywhere and never reaches a save:
the player remembers it, exactly as they remember the patrol's timing. Two
digits from one cycle and two from the next is a legitimate way to learn the
code, and typing a half-known code at the lock is a legitimate thing to try.

**The stored fact still requires a whole sequence.** `radio_code_7312`
becomes true at the end of a sequence the player was within range of
throughout. That fact is what lets the lock open without typing, so the
convenience is earned by patience while the knowledge is not.

Revised 2026-08-23 after the first played test. The previous rule was a
45-second sequence in a 60-second cycle with nothing but the full sequence
counting for anything. It left a 15-second window per minute to tune in and
punished mistiming with up to 105 seconds of a 240-second loop spent
waiting with nothing to show. A loop game cannot charge for a mistake in the
one currency the player is already paying, which is loops.

The numbers now cost between 20 and 50 seconds, and the floor is no longer
zero: a player who arrives mid-sequence leaves with the digits that were
spoken while they stood there. Knowing **when** each digit falls is the
knowledge that makes a later loop cheap, which is a better thing to learn
than how long to stand still.

The cycle is 50 rather than 40 so that it does not lock in step with the
patrol's 40 or the turnstile's 30.

A lock between Apartment 4C and the courtyard accepts `7312` and does
not accept other four-digit codes. Without the correct code the player
cannot enter the courtyard.

## The fuse, and what it is contested by

Supersedes the previous rule that the fuse is "routed to courtyard power".
It is not routed. It is carried, and it is the resource
[`ADR-0008`](../adr/ADR-0008_what-an-anchor-is-worth.md) requires an Anchor
to release.

**There is one fuse.** At Baseline it sits in Apartment 4C, and the player
can pick it up and carry it. A carried fuse is LoopWorld, per
[`world-state-model.md`](world-state-model.md), so it returns to its
authored position at every loop start.

**There are two sockets and the fuse fits either, never both.**

| Socket | While the fuse is in it |
| --- | --- |
| Building | The lift runs |
| Ground floor | The generator can be started |

At Baseline the fuse is in neither. The lift is dead and the generator
cannot start until the player chooses.

The ground-floor socket is the fuse box at the building-to-courtyard
threshold. It is the same exclusive second use that the timer-driven build
called the courtyard socket.

**The generator opens the courtyard gate**, and can be started only while
the fuse is in the ground-floor socket.

The fuse, the generator and the gate are LoopWorld and return to Baseline
at the next loop start unless `courtyard_gate_open` is active, which holds
only the gate.

## The lift and the stairs

Apartment 4C is above ground and reaching the courtyard means descending.
There are two ways down and they cost differently.

**The lift** runs only while the fuse is in the building socket.

**The stairs** always work.

**The stairs must cost at least one turnstile period more than the lift.**
Stated as a relationship for the same reason the knowledge saving is: a
number here would drift from the cycle it is measured against. With a
30-second turnstile cycle, taking the stairs must cost at least 30 seconds
more than riding the lift, so that the fuse's position alone decides which
open window the player reaches.

That is what makes the fuse a choice rather than a key. Carrying it to the
ground-floor fuse box buys the gate and costs the lift.

## Event-driven rewind checkpoints

Chapter 1 has two authored loop boundaries.

### GroundFuseGate

`GroundFuseGate` is the no-return threshold from the building route into the
courtyard-gate sequence. Its contract is:

```text
GROUND_FUSE_POWERED || courtyard_gate_open
```

If neither side is true when the player crosses, rewind is latched. Static,
light or sound instability makes the failure perceptible for at least one and
at most three seconds, then the next loop starts in 4C. The player cannot
cancel it by stepping backward.

The predicate is not checked while the player explores 4C, the hallway or the
stairs. Carrying the fuse is not failure, and leaving it on the table is not
failure. The no-return threshold is what turns the missing prerequisite into a
causal consequence.

Ground-floor power passes the checkpoint in the loop that carries the fuse
down. An active `courtyard_gate_open` Anchor passes it on the learned loop
without ground-floor power, leaving the fuse available for the building
socket.

### CourtyardGateCommit

Opening the courtyard gate through this-loop generator play makes
`courtyard_gate_open` eligible for the existing explicit commit. At the gate
threshold the player may perform that commit. A successful first-time commit
writes the Anchor, latches the same one-to-three-second prelude and ends the
loop before the player crosses into the onward Transit route.

Declining the commit leaves the player in the courtyard and does not end the
loop. A rejected commit does not end it. When the Anchor is already active at
loop start, the gate threshold is traversable and committing again is a no-op.

## Patrol

A street patrol is present at `t = 0`. Its phase is a function of
elapsed loop time.

Its route leaves a window of 20 seconds during which the path from the
courtyard toward the Transit Hub entrance is not covered. That window
repeats as a function of `t`. It does not depend on loop count or on
the previous loop's alert state.

## Turnstile

The Transit Hub entrance is a turnstile. Its cycle is 30 seconds of
elapsed loop time. It is open for the first 2.5 seconds of each cycle
and closed for the rest. Phase is `t` modulo 30 seconds.

## The three loops

The chain is a tutorial in the whole game's language, so each loop teaches
one thing and each solution creates the next problem. These are shapes the
space must support, not a script the player must follow.

**Loop A — information reveals the first contract.** The player knows nothing.
They find the radio, find the channel, and hear the four digits across the
20-second sequence to obtain `7312`. If they cross `GroundFuseGate` without
ground power, the failure prelude teaches that the route required the fuse
below. Prior knowledge may let a player combine this discovery with Loop B;
the game does not inspect loop count to forbid mastery.

**Loop B — information saves time, and matter costs it.** `radio_code_7312`
is true, so the radio is skipped entirely and the lock opens on arrival.
The player takes the fuse to the ground-floor socket, starts the generator and
opens the gate. The building socket is now empty, so the lift is dead and the
stairs are the only way down. At `CourtyardGateCommit`, the player explicitly
commits the open gate. The successful commit ends the loop after the perceptible
prelude; the global timer does not.

**Loop C — a held change makes a route possible.** `courtyard_gate_open` is
active, so the gate is open at `t = 0` and the generator is not needed. The
fuse never leaves the building socket, the lift runs, and Transit Hub is
reachable.

Loop C is the first loop after the required gate commit in which the hub can be
reached, and it is reachable because a previous loop's change refused to
rewind. That is the sentence the whole chain exists to make true.

## Testable statements

1. At `t = 0` from a clean save, the player is in Apartment 4C, the
   gate is closed, ground-floor power is off, the generator is offline.
2. The radio offers `7312` on loop one.
3. The 4C-to-courtyard lock opens for `7312` and not for `0000`.
4. The generator does not start without ground-floor power.
5. Starting the generator opens the gate.
6. At the same `t`, the patrol window and the turnstile phase match
   across loops that share Anchors and have no this-loop combat with
   the patrol.
7. The turnstile is open when `(t modulo 30) <= 2.5`. It is open at
   `t = 0` and at `t = 30`, and closed at `t = 2.6` and at `t = 29`.
8. Crossing `GroundFuseGate` with neither ground-floor power nor
   `courtyard_gate_open` active latches rewind.
9. Ground-floor power or an active `courtyard_gate_open` Anchor passes
   `GroundFuseGate` without rewind.
10. The radio's code sequence takes 20 seconds and repeats every 50, and
    the four digits fall at phases 4, 9, 14 and 19. It is the same at the
    same `t` on every loop.
10a. A player in range at a digit's phase hears that digit, whether or not
    they heard any other. Nothing about that is stored.
10b. `radio_code_7312` becomes true only at the end of a sequence heard
    from its start.
11. Exactly one fuse exists. It fits the building socket or the ground-floor
   socket, never both, and is in neither at Baseline.
12. The lift runs only while the fuse is in the building socket.
13. Descending by the stairs costs at least one turnstile period more than
    descending by the lift.
14. A successful first-time `courtyard_gate_open` commit ends the current
    loop after a one-to-three-second perceptible prelude and applies the held
    gate at the next loop start.
15. With `courtyard_gate_open` active, Transit Hub can be reached, and the
    fuse never leaves the building socket.
16. Elapsed time alone does not rewind Chapter 1. Its radio, patrol and
    turnstile still read the deterministic loop clock.
