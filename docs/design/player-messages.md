# Player messages

Status: Accepted
Owns: when the game speaks to the player in text, how long a line stays, what
may not become a message, and that the debug overlay is not UI
Does not own: puzzle facts the lines refer to, voice, audio, localisation,
interaction close-up framing, renderer look, world clocks, or the `t=`
elapsed-time overlay

A rule that is not in this document is not a player-message rule.

## The debug overlay is not UI

`GEngine->AddOnScreenDebugMessage` is an engine debug instrument. It is not
the game's interface. Player-facing text does not go there.

The `t=` elapsed-time overlay remains an instrument for stated-time checks.
It is not a message. It is not UI. This document does not own it.

`LogRewind` is a log. It is not a message.

## When the game speaks

The game speaks in text when an authored interaction needs to tell the player
a fact the world does not yet make visible by itself.

In the current authored slice that is:

- a refused action, and why it refused
- a successful state change that is not otherwise readable from the object
- a radio fragment heard while standing in range on the code channel
- the digits currently typed at the lock
- the one-time control reminder at spawn

The game does not speak a learnable world clock as a caption. Patrol phase,
turnstile phase, radio phase and elapsed loop time are read from the world,
not from this channel.

Adding a new spoken line is a catalog entry plus a call. Inventing a new
world fact still belongs to the document that owns that fact.

## How long a line stays

Each line has a duration in the catalog. `Show` uses that duration.
`ShowTimed` may override it for a single display. When the duration elapses,
the line is gone.

One line is visible at a time. Further lines wait in the order they were
asked for. `ShowSequence` is that same queue, given several ids at once.
`Clear` removes the current line and every waiting line immediately.

A duration change is a catalog edit, not an actor edit.

## Copy is data

Actors speak by identifier. The catalog holds the line. Rewriting a line does
not require editing the actor that triggers it.

Arguments exist only where the line must include this-loop input, such as the
digits typed at the lock or the radio channel just selected. The template
still lives in the catalog.

## Presentation

The line sits at the bottom of the viewport, off the composed subject, as
light text on a dark backing. It does not use the engine debug overlay. It
does not steal input. Colour is not a second information channel.

Messages are presentation. They may tick on frame delta. They are not world
state, they are not saved, and no world object reads them.

## What may not become a message

- Engine debug overlay output
- `LogRewind` lines
- The `t=` elapsed-time overlay
- World clocks, phases, and other learnable facts the player is meant to read
  from the world
- Voice, audio, and localisation, which this document does not own
- Interaction close-up framing, which
  [`camera-and-movement.md`](camera-and-movement.md) already owns

## Testable statements

These are the statements later acceptance may cite. They are not themselves
acceptance criteria.

1. No player-facing line in the authored slice is drawn by
   `AddOnScreenDebugMessage`.
2. Every player-facing line is stored as catalog data keyed by id, not as a
   literal in the actor that triggers it.
3. A queued line becomes current only after the lines in front of it expire
   or are cleared, and in the order they were enqueued.
4. A line is gone once its duration has elapsed.
5. `Clear` leaves no current line and no waiting line.
