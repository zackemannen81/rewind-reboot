# Tutorial and first run

Status: Accepted
Owns: first-run copy in the authored slice, what that copy may and may not
say, and first-time gating of those lines
Does not own: the player-message channel, queue, durations or presentation
([`player-messages.md`](player-messages.md)); puzzle facts
([`chapter-1-authored.md`](chapter-1-authored.md)); how knowledge persists
([`save-and-session.md`](save-and-session.md)); Echo
([`ADR-0012`](../adr/ADR-0012_echo-semantics.md)); voice, audio,
localisation, the courtyard, or anything beyond the entrance hall

A rule that is not in this document is not a first-run rule.

## Why this exists

The game must be legible to a player who has never seen it, and must reward
one who has seen it five times. That is a product requirement in
[`PROJECT_BRIEF.md`](../PROJECT_BRIEF.md). It is not a licence to print the
answers.

The player becomes more powerful by understanding the world, not by being
told what to do. First-run copy teaches the player how to look.

## The naming rule

The game names what a thing is. It never names what to do with it.

The radio may be described as carrying a voice. The digits are never printed
in first-run copy. A socket may be described as empty. The fuse's destination
is never named. The stairs may be described as remaining. The route is never
named.

A line that states a puzzle solution is not first-run copy. It is a defect.

## What the first-run set must say

A first-time player who reaches the entrance hall has been told, by the
game, two things:

1. The world loops.
2. Their knowledge persists.

They have not been told the code, the fuse's destination, or the route.

Those two facts are the product. They are the one thing first-run copy is
allowed to teach in words. Everything else is named, not instructed.

## The first-run set

These lines are first-run copy. They travel on the player-message channel.
Gated lines are shown at most once per session.

| Id | Line | When |
| --- | --- | --- |
| `Apartment.Waking` | Apartment 4C. The room starts over. You do not. | First spawn in 4C |
| `Character.Controls` | WASD move. E interact. | First spawn in 4C, after waking |
| `Radio.Present` | A radio. | First interaction with the radio |
| `Lock.Prompt` | A lock. Four digits. | First interaction with the locked panel |
| `Fuse.Carried` | A fuse. | First pickup of the fuse |
| `Socket.EmptyAtRest` | An empty socket. | First interaction with an empty socket whose fuse is still at rest |
| `Lift.NoPower` | The cage is still. The socket is empty. | The lift refuses an empty building socket. This line is a refused action, so it may repeat; the words still obey the naming rule |
| `Stairs.Remain` | The stairs remain. | First entry into a stair camera region |
| `Loop.Break` | The world will not hold. | First visible rewind prelude |
| `Apartment.Returned` | The room is as it was. You remember. | First return from a rewind prelude |

`Lift.NoPower` is listed because the first-run path includes the lift
refusing. Refused actions remain speakable on later attempts, as
[`player-messages.md`](player-messages.md) already requires. First-run
gating does not silence a refusal.

## What is not first-run copy

The radio's spoken digits are the puzzle speaking. They are owned as facts
by [`chapter-1-authored.md`](chapter-1-authored.md) and presented as
messages under [`player-messages.md`](player-messages.md). They are not
first-run copy, and first-run copy does not reprint them.

Typed digits at the lock are this-loop input. The catalog template does
not contain the code.

Operational confirmations — a lock that opens, a fuse that seats, a
channel that is static — name a this-loop state change. They do not teach
the first-run path, and they do not print a solution.

No first-run line describes a ghost, a replayed body, or any Echo the
player will see. ADR-0012 decided Echo is narrative only. There will not
be one.

## First-time gating

A first-run line shown once in a session is not shown again in that
session.

The seen-state is a knowledge fact. Knowledge already has an owner in
`URewindSessionSubsystem`. This document does not invent a second store.

The fact for a gated line is `seen.<MessageId>`. It becomes true when the
line is shown. It stays true across loops and across sessions until a
clean save, which is the same rule [`save-and-session.md`](save-and-session.md)
already states for knowledge.

`Lift.NoPower` is not gated. Radio digits, typed lock digits, and other
operational lines are not gated.

Loop count is not an input. A world object that changed its copy because
it was loop two would violate
[`loop-and-determinism.md`](loop-and-determinism.md). Gating reads
knowledge, which is not world.

## The loop boundary

The rewind prelude is the loop boundary the player can see. The first time
it becomes visible, the game says the world will not hold. The first time
it ends, the game says the room is as it was, and that the player
remembers.

A death ends a loop without that prelude. This document does not add a
death-specific line. The waking line already stated the product rule
before the player left 4C.

## Testable statements

These are the statements later acceptance may cite. They are not
themselves acceptance criteria.

1. First-run copy names what a thing is and does not name what to do with
   it.
2. No first-run line, and no catalog template in the authored slice,
   contains the digits `7312`.
3. A first-time player who has seen `Apartment.Waking` has been told that
   the world loops and that their knowledge persists.
4. A gated first-run line shown once for a session is not shown again for
   that session.
5. The seen-state of a gated line is a knowledge fact on
   `URewindSessionSubsystem`, not a second store.
6. No line in the first-run set describes a ghost or a replayed Echo.
