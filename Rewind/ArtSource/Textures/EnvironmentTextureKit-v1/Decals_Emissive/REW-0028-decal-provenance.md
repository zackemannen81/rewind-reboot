# REW-0028 decal sources

Status: imported source record

The six PNGs in `Decals_Emissive/` are verbatim source copies from the owner's
external `rewind-environment-texture-kit-v1` supplied at
`C:\Users\zakri\Desktop\rewind-environment-texture-kit-v1`. Its `README.md`
states that the kit was generated from the supplied RE:WIND environment concepts
and that its decal masks are intentionally white-on-black instead of alpha.
The supplied README does not name a generator, model, license, or generation
date; this record does not infer any of them. The task's direct provenance is
therefore the owner-supplied kit and its README, inspected on 2026-08-26.

These sources are tracked under Git LFS through the repository-wide `*.png`
rule. `import_rew_grime_decals.py` imports them to `/Game/Art/Textures/Decals`.
It configures the three `D_` mask textures as non-sRGB, `TC_Masks`, and Clamp;
it configures the three `T_` neon textures as sRGB, Default compression with
alpha retained, and Clamp.

## Atlas cell layouts

All three mask PNGs are square 1254 × 1254 images. The kit README specifies
four peeling-paint masks and six masks each for leaks/rust and wall cracks.
Direct visual inspection on 2026-08-26 found evenly divided rows and columns:

| Source | Cells | Grid | Instance numbering and UV offsets |
| --- | ---: | --- | --- |
| `D_REW_PeelingPlaster_Atlas_Mask.png` | 4 | 2 × 2 | row-major: 1=(0,0), 2=(0.5,0), 3=(0,0.5), 4=(0.5,0.5); scale=(0.5,0.5) |
| `D_REW_LeaksRust_Atlas_Mask.png` | 6 | 3 × 2 | row-major: 1–3 top, 4–6 bottom; scale=(1/3,0.5) |
| `D_REW_WallCracks_Atlas_Mask.png` | 6 | 3 × 2 | row-major: 1–3 top, 4–6 bottom; scale=(1/3,0.5) |

The grids come from the visible cell boundaries and the repeated mask islands,
not from count alone. The script gives each cell its own `UVScale` and
`UVOffset` override on `M_REW_GrimeDecal` and uses a dark tint (with rusty brown
only for leaks) so a mask reads as grime on RE:WIND's near-black walls.
