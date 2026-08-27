# Echo

Status: Proposed
Owns: what an Echo is, what it may and may not do, how it is staged, and the
order in which its meaning is revealed
Does not own: loop semantics, save and persistence, Anchor semantics, camera
implementation, chapter content, the macro-cycle frame, final copy
Source decisions: [`ADR-0012`](../adr/ADR-0012_echo-semantics.md) and its
2026-08-27 amendment

A rule that is not in this document is not an Echo rule.

Every other design document in this directory has carried "Echo" in its
`Does not own` line since REW-0002. This document is what those lines were
waiting for.

## What an Echo is

**An Echo is an authored silent figure performing an authored action.**

It is written, placed and animated by hand, like any other piece of staging.
Nothing about it is derived from the player: no transform is sampled, no path
is stored, no route is replayed. ADR-0012 forbids a ghost that follows a route
the player previously took, and an Echo is not one — it is a performance that
was composed before the player arrived.

That distinction is the whole reason Echo can carry the story without becoming
a system. It costs animation and staging. It costs no recording, no playback,
no per-loop state.

## The rules

**1. An Echo never changes world state.** It may appear to take, open, carry,
break or move something. The world is unaffected. The fuse the Echo pulls from
the panel is still in the panel.

That contradiction is not a defect and must never be corrected. It is the
clearest signal the game has that what the player is watching is not now. A
task that "fixes" it has removed the point.

**2. An Echo does not react to the player.** It does not turn, stop, wait, or
change what it does. There is exactly one authored exception, reserved in
["The reserved scene"](#the-reserved-scene) below, and that is a scripted beat
in a fixed place, not a behaviour an Echo has.

**3. An Echo is identical every time it plays.** It is keyed to loop time or to
a knowledge fact, never to loop count and never to what the player did.
[`ADR-0002`](../adr/ADR-0002_world-determinism.md) says the world does not
learn the player, and an Echo that adapted would not be an Echo.

**4. An Echo has no voice.** It never speaks and is never subtitled. Sound
belonging to what it touches may play. This keeps Echo inside the wordless
tradition the presentation is aiming at, and it is what lets Echo carry weight
that would otherwise cost dialogue.

**5. An Echo is not interactable.** It cannot be blocked, followed usefully,
touched or addressed. It does not collide with the player.

**6. An Echo is never a hint.** It may not demonstrate a solution the player is
currently stuck on, mark a route, or appear because the player has failed. The
moment an Echo becomes a hint system, every later Echo is read as guidance and
the ladder below collapses into a mechanic.

## Staging

An Echo reads as a silhouette: near-black, seen against a lit surface, at a
distance the authored camera chose.
[`camera-and-movement.md`](camera-and-movement.md) owns the framing; this
document owns only the requirement that an Echo must be *composed*, never
merely present. An Echo the player can walk past without noticing has cost its
animation budget for nothing.

Where the fiction wants the figure to be recognisable as the Returner, it uses
the Returner mesh. Until the reveal ladder reaches that point, silhouette and
lighting keep the identity ambiguous without ever lying about it.

## The reveal ladder

Echo's meaning is revealed in four steps. **Each step invalidates the
comfortable explanation the previous step allowed**, and each can be climbed
only once in the whole game.

| Rung | What the player sees | The model it produces |
| --- | --- | --- |
| 1 | An Echo does something the player has already done: listens to the radio, opens the apartment door, takes the fuse | "An Echo is an earlier me, in a local loop" |
| 2 | An Echo does something the player never did | "Then it is a loop I don't remember" |
| 3 | An Echo does something that requires knowledge the player has not been given | The comfortable explanation stops working |
| 4 | An Echo appears somewhere the player has never been | The local-loop model is dead |

Budget rules, because there are four rungs and five planned chapters:

- Chapter 1 spends rung 1 and nothing else.
- No chapter spends two rungs.
- At least one chapter spends none. That chapter is where the player *acts* on
  the model the previous rung forced, instead of receiving another revelation.
- Do not back-load. The strongest material should land where the game will
  certainly reach, not behind chapters that may not be built. See
  ["Scale discipline"](the-macro-loop.md#scale-discipline).

What the ladder is revealing is owned by
[`the-macro-loop.md`](the-macro-loop.md). This document owns only the order and
the pacing.

## The reserved scene

Rung 4's payoff is a single composed scene, held for the point in the game
where the macro frame becomes unavoidable. It is described here as a slot, not
a script, because the copy and the room are the writer's and the scenographer's
to fill.

The beat: the Returner enters a room for the first time. An Echo is already
there. It takes exactly the route the player is about to take. It avoids a
danger the player has not yet seen. It stops at a door, turns, and faces the
camera — the one authored exception to rule 2 — and for the first time the
figure is unambiguously the Returner. Then it does the thing that causes the
catastrophe that starts the next cycle.

No dialogue. No message line. The scene is finished when the player understands
that the question was never how to get out.

## Chapter 1's echoes

Three, all rung 1, all showing actions the player has performed in Apartment 4C:

- an Echo listening to the radio;
- an Echo opening the apartment door;
- an Echo taking the fuse from the panel — the fuse stays in the panel.

They exist to let the player build the wrong model confidently. The wrongness
is the setup for every rung after.

## Open questions for the owner

1. **Where do Chapter 1's echoes appear in the loop?** Recommendation: key them
   to loop time so they are reliably observable, not to knowledge facts, so a
   player who is not looking for them still sees at least one. Keying to
   knowledge makes them a reward for attention, which is attractive and risks
   the first rung never landing.
2. **Which chapter spends rung 4?** Recommendation: Chapter 2 or 3. The
   reserved scene is the strongest thing in the design and should not sit
   behind content that may not be built.
3. **Does Chapter 1 hint that echoes exist at all before the first one plays?**
   Recommendation: no. The first Echo should be the first evidence.
4. **Animation source.** Three Chapter 1 echoes need authored clips. The
   animation work is already queued in
   [`chapter-1-remaining-content.md`](../backlog/chapter-1-remaining-content.md)
   and this shares that dependency; it is not free, but it is not new either.

## What acceptance requires

The owner approves this document, or names what to change, and the ADR-0012
amendment it depends on is accepted in the same change. Until then this is a
proposal and no task may cite it.
