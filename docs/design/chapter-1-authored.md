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

## Loop duration

The loop timer duration is 420 seconds.

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
