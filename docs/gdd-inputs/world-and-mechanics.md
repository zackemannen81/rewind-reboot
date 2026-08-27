# World and mechanics

Derived brief. Compiled from `docs/design/chapter-1-authored.md`,
`loop-and-determinism.md`, `anchors.md`, `world-state-model.md`,
`save-and-session.md`, `camera-and-movement.md`, `player-messages.md`,
ADR-0007, ADR-0008, ADR-0009 and `docs/CURRENT_STATUS.md`. Do not hand-edit.

**This document is the authority on what the player can do. A mechanic that is
not here does not exist. Do not invent one.**

## Presentation

Third person. The camera is **authored and fixed per region**: the player never
aims, rotates or zooms it. Each region declares its own transform, rotation,
travel axis and field of view. Composition is the game's, not the player's.

Text is one line at the bottom of the viewport, from a catalog, about three
seconds. Sparse by design. There is no HUD, no inventory screen, no objective
marker, no quest log.

## The player's verbs

That is the complete list.

- **Move** at 200 cm/s. No run, no jump, no crouch, no climb.
- **Interact.** One key. It finds interactables within a 160 cm sphere and
  picks the **nearest** one.
- **Enter a digit**, 0-9, at a code lock.

The player carries at most one object: the fuse. There is no inventory.

## The loop

A loop starts with the player at the loop-start pose, the world at authored
baseline with active Anchors applied, and elapsed time zero.

A loop ends in exactly three ways, and no others:

1. an authored causal contract fails at a named checkpoint;
2. the player dies;
3. the player successfully commits an Anchor for the first time.

There is **no timer and no automatic timeout**. Cases 1 and 3 run a one-to-three
second loop-clocked prelude before the reset.

**The world does not learn the player.** Nothing reads loop count. Nothing
behaves differently because the player has done it before. Every schedule is
identical in every loop. What escalates is the player's knowledge, never the
world.

## What survives a loop

Across loops in one session: **knowledge facts and active Anchors. Nothing
else.** The player's body, carried objects, machine states and door states are
all loop-volatile and return to baseline.

Across sessions: the same knowledge facts and Anchors. A clean save is
knowledge empty, no Anchors, next loop from baseline, and it is reachable as a
first-class action.

A knowledge fact is stored. A remembered digit, a remembered timing and a
remembered route are **not** stored — they live in the player's head, and that
is the point.

## Chapter 1's chain

**Radio.** Four channels, cycling 1 → 2 → 3 → 4 → off → 1. Baseline is
**channel 2**. **Channel 3** carries the broadcast; the others carry static.
The sequence takes 20 seconds of loop time and repeats every 50 seconds; phase
is `t` modulo 50. The digits **7, 3, 1, 2** are spoken at phases **4, 9, 14 and
19**. A digit heard while in range is kept by the player. A whole sequence
stood through grants the stored fact `radio_code_7312`.

**The 4C door** is a code lock. It accepts **7312** and nothing else. It is the
way out of the apartment.

**The fuse.** There is exactly one, it is carried, and it fits **two exclusive
sockets, never both**:

| Socket | While the fuse is in it |
| --- | --- |
| Building, in 4C | The lift runs |
| Ground floor, at the building-to-courtyard threshold | The generator can be started |

At baseline it is in neither. It lies out on the desk in 4C.

**The lift** runs only with the fuse in the building socket: six seconds each
way between floor 4 and the entrance. **The stairs always work** and must cost
at least one turnstile period more. That trade is the whole point: the fuse's
position, not the player's skill, decides which route is open.

**The generator** opens the courtyard gate, and starts only with the fuse in
the ground socket.

**The Anchor** is `courtyard_gate_open`. Committing it the first time ends the
loop. Holding it lets the fuse stay in the building, which is what makes the
fuse a choice rather than a key.

## Spaces that exist

Apartment 4C. The fourth-floor hall outside it. The lift shaft. A four-floor
stairwell. The ground hall. A courtyard blockout: approach hall, plaza,
generator, gate and the Transit exit.

**Not built:** the Transit Hub interior, and everything after Chapter 1.

## What does not exist — do not write it

- **No hazards. Nothing in the game can kill the player.** Death is a valid
  loop-end class with no source in the world yet. No arcing outlets, no
  electrified hardware, no falling, no drowning. If a chapter needs death, that
  is a new mechanic and needs a decision before it may be written.
- **The fuse does not control lights, power or hazards in a room.** It is a
  contested resource between the lift and the generator. Nothing else.
- **No inventory, no crafting, no combat, no stealth meter, no dialogue tree,
  no skill or upgrade of any kind.**
- **No Echo may change world state**, block the player, react to the player,
  collide with the player, or speak.
- **Nothing carries across a loop except knowledge facts and Anchors.**

## Anchor admission test

If a chapter proposes a new Anchor: name the resource the loop must otherwise
commit, and name its two uses. If either cannot be named, it is a shortcut, not
an Anchor, and it does not go in.
