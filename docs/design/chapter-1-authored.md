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

A radio in Apartment 4C is present at `t = 0`. It presents the code
`7312` so that a player who listens can obtain it on the first loop.
It does not depend on loop count.

A lock between Apartment 4C and the courtyard accepts `7312` and does
not accept other four-digit codes. Without the correct code the player
cannot enter the courtyard.

## Fuse, generator, gate

At Baseline:

- The fuse can be routed to courtyard power once this loop.
- Courtyard power is off.
- The generator is offline.
- The courtyard gate is closed and blocks the way to the street.

This-loop play:

- Routing the fuse turns courtyard power on.
- The generator can be started only while courtyard power is on.
- Starting the generator opens the courtyard gate.

Those four objects are LoopWorld. They return to Baseline at the next
loop start unless `courtyard_gate_open` is active, which holds only the
gate.

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
