# Planted details

Status: Proposed
Owns: the register of details authored early because they pay off later, and
the rule that protects them from every pass that happens in between
Does not own: chapter content, art direction, Echo behaviour, the macro-cycle
frame, final copy

A detail that is not in this register is not protected.

## Why this exists

Some of the strongest material in the design is a detail that means nothing the
first time it is seen and everything ten hours later. The radio already tuned
to a channel nobody set. Marks on a door from attempts nobody made. An object
lying exactly where it will later prove most useful.

All of them have to be authored in Chapter 1, **before the thing that explains
them exists**, and then survive every dressing pass, art pass, prop
regeneration, material rebind and cleanup that happens between now and the
payoff.

Nothing in this repository currently distinguishes a prop that is there because
it looks right from a prop that is there because it is load-bearing for a
reveal in a later chapter. Without that distinction, a dressing wave deletes
the game's own foreshadowing and no one notices until the payoff lands on
nothing.

This is not hypothetical. It already happened once, before this register
existed. See [PD-002](#pd-002).

## The rule

**A planted detail is registered here before it is authored. Once registered,
it may not be moved, replaced, retextured or removed by any task that does not
name it.**

Concretely:

- Any dressing, art, prop or cleanup task reads this register first and treats
  the listed actors as frozen.
- A task that needs to change a planted detail stops and asks the owner. It
  does not decide.
- A task that changes one anyway records it here, in the entry, with what
  changed. An unrecorded change is the failure this document exists to prevent.
- Removing an entry is a design decision and needs the owner, not a commit
  message.

The register does not protect a detail from being *dressed better*. A planted
detail may get a real mesh, a better material, correct lighting. What it may
not lose is its position, its state, and the thing about it that will be
noticed later.

## Register

Status values: `planned` — decided, not yet in the map. `authored` — in the map
and frozen. `paid` — the payoff has shipped, and the entry is history.

### PD-001

| | |
| --- | --- |
| Detail | The radio is already tuned to channel 2 at loop start — one step short of the channel that carries the code. Nobody in the fiction set it, and whoever did stopped one step early. |
| Where | Apartment 4C. Gameplay actor `4C_Radio`; visible prop `Meshy_AI_rad_o-3d-texture`, on the desk at `(-240, 1530, 1319)` |
| First seen | Chapter 1, first loop |
| Paid off in | Unassigned. Requires the rung-3 or rung-4 chapter — see [`echo.md`](echo.md#the-reveal-ladder) |
| What it means then | Someone prepared Chapter 1. Later: the Returner prepared Chapter 1, in an earlier cycle. |
| Status | `authored` and frozen. The owner delegated the choice on 2026-08-27; channel 2 is now the rule in [`chapter-1-authored.md`](chapter-1-authored.md), which owns the radio, and `ARewindRadio::RestoreFromBaseline` restores it. The channel number is what must not drift. |

### PD-002

| | |
| --- | --- |
| Detail | The fuse sits where it sits, unexplained, as though left ready. |
| Where | Apartment 4C. Actor `4C_Fuse` |
| First seen | Chapter 1, first loop |
| Paid off in | Unassigned |
| What it means then | The room was set up for someone who would need it. |
| Status | `authored` and frozen at the desk. On 2026-08-26 the fuse was moved from `(-250, 1280, 1350)`, inside the `electrical_panel` prop, to `(-215, 1435, 1316)` on the desk. The move was made for a mechanical reason — at the old position it sat 6 cm outside the Apartment4C player volume and 19 cm from the building socket, so interaction was unreliable — and it was made without knowing the detail was load-bearing, because this register did not exist. The owner ruled on 2026-08-27 that the desk is correct: a fuse left out reads as prepared, a fuse already seated in its own box does not. The desk position is now the planted detail. It still needs a mesh; it is currently an engine cube. |

### PD-003

| | |
| --- | --- |
| Detail | A door carries marks from repeated attempts nobody has made. |
| Where | Apartment 4C. Candidate: `4C_CodeDoor_7312` at `(-275, 1055, 1310)` — the keypad door is the door the player will attempt most, which is what makes the marks read |
| First seen | Chapter 1, first loop |
| Paid off in | Unassigned |
| What it means then | The attempts happened. They were not the player's, and they were not in this loop. |
| Status | `planned`. Needs a decal or material treatment; the grime decal instances from REW-0028 are the existing vehicle. |

### PD-004

| | |
| --- | --- |
| Detail | An object lies exactly where it later proves most useful. |
| Where | Unassigned |
| First seen | Chapter 1 |
| Paid off in | Unassigned |
| What it means then | The placement was not chance. |
| Status | `planned`. This entry cannot be authored until the chapter that pays it off names the use. Registering it now reserves the intent so it is not invented late and placed badly. |

## How a task uses this register

A dressing, art or prop task adds one line to its charter's verification
gates:

> Every actor named in `docs/design/planted-details.md` is unchanged, or the
> change is named in the charter and recorded in the register.

That is the whole mechanism. It is cheap because the register is short, and it
stays cheap only if the register stays short: a detail that is merely nice is
not planted, and putting it here to be safe makes the rule stop meaning
anything.

## Open questions for the owner

1. ~~**PD-002: panel or desk?**~~ Answered by the owner on 2026-08-27: the
   desk. A fuse lying out where someone left it reads as prepared; one already
   seated in its own box does not. Recorded in the entry, and the position is
   frozen.
2. **Should the register carry a payoff chapter before that chapter is
   designed?** Recommendation: no. Leave `Unassigned` and let the chapter claim
   it. A detail assigned early tends to be written to fit the assignment.
3. **How many planted details does Chapter 1 carry?** Recommendation: these
   four and no more. Four unexplained things in one apartment is a texture. Ten
   is a puzzle the player will try to solve immediately, which is the opposite
   of what they are for.

## What acceptance requires

The owner approves this document, or names what to change. PD-002 is already
answered. Until then it is a proposal, and the register protects nothing.
