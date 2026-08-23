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

## Movement and loop duration

Revised 2026-08-23 under REW-0004. The previous values were 420 seconds and
an unauthored walk speed inherited from the Unreal third-person template.
They are recorded here because
[`five-loops-2026-08-22.md`](../playtests/five-loops-2026-08-22.md) is
evidence taken under them and remains true of that build.

The player's walk speed is **200 cm per second**. It was 500, which is a
sprint and was never a decision. Travel time is a design resource in a game
about repeating a route, and at 500 it rounded to nothing.

The loop timer duration is **240 seconds**. It was 420.

## The knowledge saving

This is the rule the previous values failed, and it is stated as a
relationship rather than a distance so that it survives a change of speed.

**A naive run must take at least one turnstile period longer to reach the
turnstile than a learned run.**

Stated in time, not in distance. The first draft of this rule said sixty
metres of path, which measured the wrong thing: a saving can come from a
loop spent listening to a radio, or from a slow route taken because a
resource is somewhere else, and neither is a distance. Any lever that
produces the difference satisfies the rule.

The naive run does whatever the puzzle costs a player who knows nothing.
The learned run has `radio_code_7312` and `courtyard_gate_open`. The
difference is everything knowledge buys.

With a 30-second turnstile cycle, that difference must exceed 30 seconds,
so the two runs reach different open windows. Measured on the build this
rule was written against, the difference was 8.3 metres of walking, about
3 seconds, so both runs always reached the same window and the crossing
time measured the gate rather than the player.

Distance is one way to produce the difference and the most expensive one.
See [`fuse-radio-and-elevator.md`](../backlog/fuse-radio-and-elevator.md)
for a structure that produces it from mechanics instead. That proposal is
not authority and nothing here depends on it.

The consequence for layout: **an Anchor must save a journey, not a button
press.** The fuse and generator belong on a branch off the route, not as
stops along it. A generator the player passes on the way is worth no time to
hold open.

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
| Courtyard | The generator can be started |

At Baseline the fuse is in neither. The lift is dead and the generator
cannot start until the player chooses.

**The generator opens the courtyard gate**, and can be started only while
the fuse is in the courtyard socket.

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
generator buys the gate and costs the lift.

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

**Loop A — information costs time.** The player knows nothing. They find the
radio, find the channel, and wait through a full 45-second sequence to
obtain `7312`. They may also find the fuse. They do not reach Transit Hub,
because the sequence has taken a fifth of the loop and they did not know
where anything was.

**Loop B — information saves time, and matter costs it.** `radio_code_7312`
is true, so the radio is skipped entirely and the lock opens on arrival.
The player takes the fuse to the courtyard socket, starts the generator and
opens the gate. The building socket is now empty, so the lift is dead and
the stairs are the only way down. **Loop B cannot reach Transit Hub**: the
stairs plus the chain exceed the loop duration. That is a constraint on the
authored space, not an accident of pacing.

**Loop C — a held change makes a route possible.** `courtyard_gate_open` is
active, so the gate is open at `t = 0` and the generator is not needed. The
fuse never leaves the building socket, the lift runs, and Transit Hub is
reachable.

Loop C is the first loop in which the hub can be reached, and it is
reachable only because a previous loop's change refused to rewind. That is
the sentence the whole chain exists to make true.

## Testable statements

1. At `t = 0` from a clean save, the player is in Apartment 4C, the
   gate is closed, courtyard power is off, the generator is offline.
2. The radio offers `7312` on loop one.
3. The 4C-to-courtyard lock opens for `7312` and not for `0000`.
4. The generator does not start without courtyard power.
5. Starting the generator opens the gate.
6. At the same `t`, the patrol window and the turnstile phase match
   across loops that share Anchors and have no this-loop combat with
   the patrol.
7. The turnstile is open when `(t modulo 30) <= 2.5`. It is open at
   `t = 0` and at `t = 30`, and closed at `t = 2.6` and at `t = 29`.
8. A naive run reaches the turnstile at least one turnstile period later
   than a learned run. With a 30 second cycle that is 30 seconds.
9. A run that knows nothing and a run that knows everything reach the
   turnstile in different open windows.
10. The radio's code sequence takes 20 seconds and repeats every 50, and
    the four digits fall at phases 4, 9, 14 and 19. It is the same at the
    same `t` on every loop.
10a. A player in range at a digit's phase hears that digit, whether or not
    they heard any other. Nothing about that is stored.
10b. `radio_code_7312` becomes true only at the end of a sequence heard
    from its start.
11. Exactly one fuse exists. It fits the building socket or the courtyard
    socket, never both, and is in neither at Baseline.
12. The lift runs only while the fuse is in the building socket.
13. Descending by the stairs costs at least one turnstile period more than
    descending by the lift.
14. With the fuse in the courtyard socket, Transit Hub cannot be reached
    within one loop duration.
15. With `courtyard_gate_open` active, Transit Hub can be reached within
    one loop duration, and the fuse never leaves the building socket.
