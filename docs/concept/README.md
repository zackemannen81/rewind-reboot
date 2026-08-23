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
| `stairwell-4c-landing.png` | The landing outside Apartment 4C, from the side. A magenta `4C` sign with a cyan underline beside a heavy door, a barred second opening next to it, a coat on a hook, a radiator, a railed walkway. A figure in a coat descending the stairs in silhouette. A `3 FLOOR` sign on the far wall, wall lamps making small pools, rain on the window and a distant `RE:WIND` sign in the city | This is the connective space REW-0005 requires: the stairwell and the shaft beside it |
| `apartment-4c-interior.png` | Apartment 4C, side-on and wide. A rain-streaked window wall with city neon, a sofa and low table, a desk with a cassette radio and a mug, a shelf, the `4C` sign, the front door under a lamp, a wall panel with a red indicator, and a kitchen counter at the right | The radio's placement, and the room the loop starts in |
| `fuse-box-detail.png` | An open fuse box in close-up. The door carries a printed label reading `RE:WIND / 4C / FUSE BOX`, a small floor plan, and a numbered list: `1. HALL LIGHT`, `2. LIVING AC`, `3. RECEPTACLES`, with `USE 250V FUSE ONLY`. Inside are three round sockets, one apparently empty and lit, and below them a strip reading `250V 15A SLOW BLOW` with two switches and a cartridge fuse in a holder | The removable cartridge is the carried fuse. See the discrepancy below |

## What these confirm

The composition in all three is the authored side-on camera of
[`ADR-0007`](../adr/ADR-0007_camera-and-perspective.md): a fixed angle, a
composed frame, large areas of near-black, and light arriving in small motivated
pools. The figure reads as a silhouette rather than as detail. Neon is a
restrained accent in magenta and cyan against a cold, almost monochrome ground.

The stairwell image is the strongest confirmation, because it shows the space
`chapter-1-authored.md` decided before the image was made: a stairwell and a
shaft beside it, on the floor the apartment is on.

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
