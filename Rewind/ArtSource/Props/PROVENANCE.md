# Chapter 1 props — provenance

Recorded per [`ADR-0011`](../../../docs/adr/ADR-0011_asset-source-and-provenance.md).

All props are generated on the owner's Meshy Pro account, whose output is
private and commercially usable. The source of truth for each is a Meshy task
id plus its remesh parameters, not a tracked FBX: the exports embed their
4K textures, so tracking them would cost ~40 MB per prop to store something
regenerable from a URL and two numbers.

## Remeshed for polycount, 2026-08-26

Meshy's raw output was around 1.9 million triangles per prop. Nanite was
carrying that, which is what Nanite is for, but it is not a reason to ship a
desk with more geometry than a hero character. Remeshed through the
`openapi/v1/remesh` endpoint, `topology=triangle`, `origin_at=bottom`.

| Prop | Source task | Target | Result |
| --- | --- | --- | --- |
| Desk | `acf2523f-8d53-4084-a60a-7a76ee5cc3ec` | 6,000 | 3,698 tris, from 1,932,126 |
| Pendant lamp | `f4d04bed-6150-43d3-ab95-f0998550bcf5` | 4,000 | 4,140 tris, from 1,860,510 |
| Neon 4C sign | `e0c7a020-8f52-4461-8322-46d9665cda8e` | 2,500 | 2,490 tris, from 1,823,126 |

## Generated at usable density already

Radio, shelf, coat, railing/plant, door, sofa, footstool and kitchen table came
back between 4,268 and 13,682 triangles and need no remesh.

## Texture policy

Props render through a near-black silhouette-weighted frame. Base colour and
normal maps are capped at 1024 and mask maps at 512 through `MaxTextureSize`,
which caps the built resolution while leaving source data intact, so the cap
can be raised later without re-importing.

Roughness and metallic maps arrive from Meshy flagged `sRGB` with default
compression. That is wrong rather than merely heavy: they carry linear mask
data, and a gamma curve applied to non-colour data makes the surface respond
to light incorrectly. They are corrected to linear with `TC_Masks`.
