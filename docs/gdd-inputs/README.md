# GDD Creator inputs

Discoverability: index. Every member is listed below.

**These files are derived, not authority.** Each one is compiled from the
documents that own its rules, for pasting into the external GDD Creator as
source documents. Nothing here decides anything. If a file here disagrees with
its sources, the file is wrong.

**Do not hand-edit.** The update path is to recompile from the sources named at
the top of each file. A hand edit here becomes a rule nothing owns, which is
the exact failure `AGENTS.md` exists to prevent.

## Members

| File | Paste as | Compiled from |
| --- | --- | --- |
| [`world-and-mechanics.md`](world-and-mechanics.md) | `World and mechanics` | `design/chapter-1-authored.md`, `loop-and-determinism.md`, `anchors.md`, `world-state-model.md`, `save-and-session.md`, `camera-and-movement.md`, `player-messages.md`, ADR-0007/0008/0009, `CURRENT_STATUS.md` |
| [`story-spine.md`](story-spine.md) | `Story spine` | `design/the-macro-loop.md` |
| [`story-timeline.md`](story-timeline.md) | `Story timeline` | `design/the-macro-loop.md`, `design/echo.md` |
| [`chapter-purposes.md`](chapter-purposes.md) | one section, as `Current chapter` | `design/echo.md`, `design/chapter-1-authored.md`, `design/chapter-2-authored.md` |

Send alongside the three you already have: **Narrative directive**, **Echo**
(from [`../design/echo.md`](../design/echo.md)) and **Planted details** (from
[`../design/planted-details.md`](../design/planted-details.md)). Seven
documents, inside the tool's limit of ten.

## Theme and premise fields

**Tema:**

```text
RE:WIND — Memory, Echoes and the Architecture of Foreshadowing
```

**Antal kapitel:** `1`. Run one chapter per job and paste that chapter's
section as `Current chapter`. Asking for five at once is what let the last run
spend the whole Echo ladder inside Chapter 1.

**Premiss:**

```text
A third-person psychological sci-fi puzzle game. The world resets in
deterministic loops; the player's knowledge does not. The player grows stronger
by understanding the world, never by unlocking or upgrading.

The Returner wakes in Apartment 4C and learns, loop by loop, that a route that
looked impossible is only unrehearsed. Later evidence points somewhere worse:
the journey itself has already happened. Echoes are the instrument of that
reveal, and they are authored performances, never recordings of the player.

Presentation is visual: fixed authored cameras, sparse text, technical
interiors, and planted details that gain meaning when the player returns
knowing more. Every reveal must be fairly planted before it pays off.
```

## Per-run continuity

Chapter 1 runs with no continuity bootstrap. Every later chapter injects one,
and it must keep three tiers apart, because a game does not continue from what
happened but from what the player knows:

1. **Fiction history** — what the macro cycle claims occurred. Not world state.
2. **World state at chapter entry** — the baseline. It resets every loop.
3. **Player-guaranteed knowledge** — the only tier a chapter's writer may
   assume. It maps to knowledge-fact ids in `URewindSessionSubsystem`, not to
   prose.
