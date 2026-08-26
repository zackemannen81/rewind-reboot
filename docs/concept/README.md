# Concept Art

Discoverability: index. Every image in this directory is listed below with what
it shows.
Member state: not applicable. Images carry no `Status:` line; this index carries
their status instead.

Target images produced for this project, by its owner. They show what the game
should look and feel like.

**They are targets, not specifications.** A task may cite an image as a goal it
is aiming at. No task may cite one as a rule. If an image and a design document
disagree, the design document is right and the image is a picture of an
intention. Where an image shows something the rules have not decided, that is a
question to route, not a permission.

This is not `docs/concepts_sandbox/`. That holds excluded and imported material
which may never be implemented. These are the project's own, and they are
current.

Image types are tracked by Git LFS in `.gitattributes` under
[`ADR-0005`](../adr/ADR-0005_asset-storage.md).

## Images

| Image | Shows | Notes |
| --- | --- | --- |
| `stairwell-4c-landing.png` | The landing outside Apartment 4C, from the side. A magenta `4C` sign with a cyan underline beside a heavy door, a barred second opening next to it, a coat on a hook, a radiator, a railed walkway. A figure in a coat descending the stairs in silhouette. A `3 FLOOR` sign on the far wall, wall lamps making small pools, rain on the window and a distant `RE:WIND` sign in the city | This is the connective space REW-0005 requires: the stairwell and the shaft beside it. The owner clarified that 4C is on floor 4, so the entrance route has three stair sections and two intermediate landings |
| `apartment-4c-interior.png` | Apartment 4C, side-on and wide. A rain-streaked window wall with city neon, a sofa and low table, a desk with a cassette radio and a mug, a shelf, the `4C` sign, the front door under a lamp, a wall panel with a red indicator, and a kitchen counter at the right | The radio's placement, and the room the loop starts in |
| `apartment-4c-interior-light-enhanced.png` | The same Apartment 4C target with lifted room exposure: rainy city/window, sofa and rug, central radio cabinet and shelf, 4C neon, technical pipes, door/keypad/coat and right-side kitchen remain readable while the framing stays near-black | Owner-supplied primary visual target for REW-0038/0039; exposure and material readability reference, not new game-rule authority |
| `fuse-box-detail.png` | An open fuse box in close-up. The door carries a printed label reading `RE:WIND / 4C / FUSE BOX`, a small floor plan, and a numbered list: `1. HALL LIGHT`, `2. LIVING AC`, `3. RECEPTACLES`, with `USE 250V FUSE ONLY`. Inside are three round sockets, one apparently empty and lit, and below them a strip reading `250V 15A SLOW BLOW` with two switches and a cartridge fuse in a holder | The removable cartridge is the carried fuse. See the discrepancy below |
| `lift-4c-shaft.png` | The cage lift beside 4C, framed through the vertical shaft. Floor 3 and floor 2 landings remain visible below, with the machinery above, narrow pools of warm light and the same magenta/cyan city accents | Target for the lift as a real vertical journey through the same three floor transitions as the stairs, not an abstract scene change |
| `chapter-1-4c-circulation-plan.png` | Owner-drawn relationship map connecting Apartment 4C, its common fourth-floor corridor, the separate stairwell and the cage lift | Topology clarification, not finished art: 4C is behind its own door; corridor, stairwell and lift shaft are distinct connected spaces |
| `chapter-1-circulation-blockout.png` | Four owner-drawn orthographic/blockout views exploring the relationship between 4C, hallway, switchback stair and vertical lift shaft | Early construction study for REW-0007. The later top- and ground-floor plans below settle the arrangement and camera sides more precisely |
| `chapter-1-4c-interaction-layout.png` | A sparse owner annotation for 4C's interaction landmarks: keypad, radio and fuse-box/fuse area | Placement reference: keypad at the apartment door, radio as the room's central focal interaction, and the fuse pickup reading as part of the technical wall/fuse box area |
| `chapter-1-top-floor-plan.png` | Owner-drawn top-floor plan with stairs, lift and Apartment 4C in the upper row, their common hallway below, the keypad/radio/fuse-box landmarks and three camera directions | Settled REW-0007 top-floor construction reference. Apartment and stair cameras look from the upper side; the shared hallway/lift camera looks from the opposite side |
| `chapter-1-ground-floor-plan.png` | Owner-drawn ground-floor plan joining the stair and lift landing to a long hallway, fuse-box threshold and large courtyard with gate, generator, patrol area and Transit exit | Settled REW-0007 ground-floor construction reference. The courtyard is a single composed room; its generator journey runs around the patrol yard rather than becoming a detached corridor |

| `chapter-1-courtyard-target.png` | The courtyard at night in rain, side-on. The 4C building's barred gate at the left under a wall lamp with a magenta `4C` sign and cyan underline; a generator outbuilding centre-right with a lit door and a magenta `GENERATOR` sign; a `TO TRANSIT` gate at the right; a distant `RE:WIND` sign on the city skyline; wet reflective paving, a dumpster, and a chain-link fence across the foreground | The target for the space REW-0020 builds. It shows the three things the ground-floor chain needs in one frame: the way back into the building, the generator, and the Transit exit. The foreground fence is the black-mass occluder the art direction calls for, made of geometry rather than vignette |
| `chapter-1-ground-hallway-target.png` | The ground-floor hallway, side-on and letterboxed. Stairs at the left under a `1 FLOOR` sign, the lift behind a scissor gate with a red indicator, a wall-mounted electrical cabinet centre-right, and the courtyard threshold at the right opening onto rain and a vertical `RE:WIND` sign | The hallway between the stairwell and the courtyard, and the ground fuse box that the ground-floor power contract turns on. Confirms the two-tone wall grammar already accepted in `docs/design/stairwell-visual-direction.md`: worn upper plaster over a muted green lower band |
| `chapter-1-street-and-transit-target.png` | The street outside, side-on in rain. An arched Transit entrance under a magenta `TRANSIT` sign with cyan underline, a `4C LINE / ALL STATIONS` board and descending stairs, a transit map panel at the right, a `4C COURTYARD` sign pointing back the way the player came, chain-link with `DANGER KEEP OUT` at the left, and the city beyond | The space past the courtyard. Not chartered yet; it is the task after REW-0020. The `4C COURTYARD` sign matters more than it looks: the street is where the player learns the route has a direction |
| `apartment-4c-walkable-and-interactions.png` | Apartment 4C, side-on and letterboxed, with annotation. A green band along the floor marks the walkable area; yellow boxes mark the radio on the desk, the front door, and the small panel beside it; a yellow arrow at the door marks the exit. Behind them the window wall with city neon, a sofa and low table, the `4C` sign, a coat on a hook and a kitchen counter at the right | The most directly usable of these. The green band is the player volume that `docs/design/camera-and-movement.md` requires each region to declare, drawn rather than described, and the yellow boxes are the interaction landmarks. It also states the intended 4C framing: the whole room, wide and letterboxed |
| `apartment-4c-window-city-view.jpg` | The view out of 4C's window in rain: a balcony railing, a plant, two magenta `RE:WIND` signs and a cyan vertical neon strip on the buildings opposite | What the window wall carries. The window is 4C's largest light source in every target frame of that room, and the current build has nothing in it |

## What these confirm

The composition in the four rendered targets is the authored side-on camera of
[`ADR-0007`](../adr/ADR-0007_camera-and-perspective.md): a fixed angle, a
composed frame, large areas of near-black, and light arriving in small motivated
pools. The figure reads as a silhouette rather than as detail. Neon is a
restrained accent in magenta and cyan against a cold, almost monochrome ground.

The stairwell and lift images are the strongest confirmation, because they show
the space `chapter-1-authored.md` decided before the images were made: a
stairwell and a shaft beside it, with three floor transitions between 4C and
the entrance.

## One discrepancy, settled

`fuse-box-detail.png` labels its circuits `HALL LIGHT`, `LIVING AC` and
`RECEPTACLES`. Those are an apartment's own circuits.
[`chapter-1-authored.md`](../design/chapter-1-authored.md) decided one fuse and
two sockets, one running the lift and one letting the courtyard generator start.

**Settled 2026-08-23: the design's flow wins, the image's style stands.** The
labelling is content rather than look, and the look is what this image is a
target for. Whatever is modelled carries the design's two sockets; the panel's
material, its lit socket, the printed door label as an object, and the way the
whole thing sits in near-black with the city behind it are all correct and are
what the image is here to specify.

This is the rule at the top of this file working as intended rather than an
exception to it: the document decided, the image aimed.
