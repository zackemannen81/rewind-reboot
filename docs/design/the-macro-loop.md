# The macro loop

Status: Proposed
Owns: the macro-cycle frame, what part of it is fiction and what part is
mechanism, the position on determinism and freedom, and how chapters relate to
the cycle
Does not own: local loop semantics, persistence mechanics, Anchor semantics,
Echo behaviour, chapter content, final copy
Source decisions: [`ADR-0002`](../adr/ADR-0002_world-determinism.md),
[`ADR-0004`](../adr/ADR-0004_save-and-session.md),
[`ADR-0008`](../adr/ADR-0008_what-an-anchor-is-worth.md)

A rule that is not in this document is not a macro-loop rule.

## The frame

The local loops are not the shape of the game. They are small reset domains
inside one larger cycle.

```text
MACRO CYCLE
│
├─ Chapter 1
│   ├─ local loop
│   ├─ local loop
│   └─ local loop → exit
│
├─ Chapter 2
│   ├─ local loop
│   └─ local loop → exit
│
├─ Chapter 3 …
│
└─ Final event
        ↓
   MACRO RESET
        ↓
   Chapter 1
```

**Chapter 1 is not the beginning.** It is the point at which the Returner
begins to remember. Everything before it may already have happened many times.

The title carries both readings. The player first takes `RE:WIND` as a
description of the mechanic: the world rewinds a few minutes. Later it is a
description of the story: the whole thing is a rewind.

## What is mechanism and what is fiction

**The macro loop is fiction. Nothing in the build simulates it.**

This is the load-bearing decision in this document, and it is what makes the
entire structure affordable.

A new game is a new cycle. [`ADR-0004`](../adr/ADR-0004_save-and-session.md)
has three states — loop-volatile world, knowledge and Anchors across loops
inside a session, the same restored across sessions — and it requires that a
clean save be reachable as a first-class action. The macro loop adds no fourth
state, so none of that changes. "The previous cycle" is something the first
playthrough already assumes, the way a novel's first page assumes a past.

Every beat that sells the macro loop is authored content: Echo performances,
planted details, staging, and what the world implies. None of it needs
persistence machinery, which is the same reason
[`echo.md`](echo.md) can carry the story without recording anything.

**The alternative is named and rejected for now.** Macro state that survives a
clean save — an artefact, a physical change on an exceptional persistence
surface, a mark left for the next cycle — would require a fourth persistence
tier and would break ADR-0004's clean-save property. Revisit only when a
specific scene cannot be authored any other way, and revisit it as an ADR, not
as a task's convenience.

## Determinism and freedom

The uncomfortable question the frame invites is whether the player has any
freedom if everything has already happened. Answering "no" is flat, and it is
also not what this game believes.

**The sequence of events may be determined without the knowledge being
determined.**

The Returner makes the same large journey again and again. What differs is what
survives: each cycle preserves something the last one could not — an Echo, an
observation, a change in who is watching the system.

This is not a new rule. It is [`ADR-0002`](../adr/ADR-0002_world-determinism.md)
at a larger radius. The local rule is that the world resets and the player's
knowledge persists; the world does not learn, the one who observes does. The
macro rule is the same sentence with a bigger circle drawn around it. That is
why the frame does not feel bolted on, and it is why the small loop puzzles and
the long story are the same activity at two scales: **observe a deterministic
system until you find where the real degree of freedom is.**

## The macro Anchor

The player learns early that an Anchor carries one thing across a local loop
boundary. The question the ending exists to ask is what an Anchor is for the
whole cycle.

[`ADR-0008`](../adr/ADR-0008_what-an-anchor-is-worth.md) already supplies the
test, and it applies unchanged at this scale: **name the resource the cycle
must otherwise commit, and name its two uses.** If either cannot be named, the
macro Anchor is a shortcut wearing a costume, exactly as a local one would be.

One prerequisite, recorded because it is easy to forget: ADR-0008 states that
`courtyard_gate_open` **does not currently pass that test** — holding the gate
open only makes existing routes faster. The tutorial's Anchor has to
demonstrate the mechanic before the finale can escalate it. The macro Anchor
cannot be designed around a local one that does not yet work.

Structurally, then, the ending is: understand why the cycle exists, and find
the single intervention that lies outside its reset boundary.

## Scale discipline

The presentation references — Another World, Flashback, Beneath a Steel Sky,
Little Nightmares — are two to ten hour games. Their retention comes from
composed scene following composed scene, not from a system that compounds over
twenty hours. The design should match them.

Two rules follow:

- **Chapters are reset domains, so they are genuinely modular.** Use that. The
  game must be able to end earlier than planned and still have said what it
  means.
- **Do not back-load the strongest material.** The reserved scene in
  [`echo.md`](echo.md#the-reserved-scene) should land where the game will
  certainly reach.

## Open questions for the owner

1. **Is the macro loop fiction, or does something survive a clean save?** This
   document decides fiction. Recommendation: keep it there. It costs nothing,
   supports every beat currently described, and leaves ADR-0004 intact. If a
   scene later demands real macro persistence, amend ADR-0004 first.
2. **How many chapters?** Recommendation: build for the length the references
   imply and treat chapters three to five as an extension rather than a
   commitment. See ["Scale discipline"](#scale-discipline).
3. **What is the resource the macro Anchor contests?** Unanswered, and it
   cannot be answered before the local Anchor passes ADR-0008's test.
4. **How much does the game say out loud?** The reference set contains a
   contradiction: the point-and-click tradition retains through density of
   authored response, and the cinematic tradition says almost nothing.
   [`player-messages.md`](player-messages.md) owns the text channel; this is a
   note that the choice sets the content cost of every room in the game.

## What acceptance requires

The owner approves this document or names what to change. Until then it is a
proposal, no task may cite it, and it decides nothing about implementation.
Nothing here authorises Chapter 2 code, maps or assets.
