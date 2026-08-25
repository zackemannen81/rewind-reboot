# The Returner — source assets

Provenance, as [`ADR-0011`](../../../../docs/adr/ADR-0011_asset-source-and-provenance.md)
requires. These are the sources the tracked Unreal assets are built from; the
`.uasset` form can always be rebuilt from what is here.

| File | What it is |
| --- | --- |
| `Returner_Character.fbx` | The rigged character. 15,439 vertices, ~31,123 triangles, ~24 bones |
| `Returner_Walking.fbx` | Walk clip on the same rig, from the rigging pass |
| `Returner_Running.fbx` | Run clip on the same rig, from the rigging pass |
| `Returner_WalkingWoman.fbx` | Library action 1, a second walk variant |
| `Returner_Alert.fbx` | Library action 2, candidate for a standing idle |

## Generation

- Generator: Meshy, `image-to-3d` then `rigging`, through the openapi v1 REST
  API on 2026-08-25.
- Source image: the owner's own T-pose reference for The Returner, a hooded
  figure in a long tattered coat over dark trousers and heavy boots.
- Texturing was disabled deliberately. The player renders through
  `MI_CharacterSilhouette` at near-black, so albedo, roughness and metallic
  maps are never visible. An earlier export carried 10 MB of maps that the
  silhouette material makes invisible.
- Meshy account: the owner's Pro subscription. Output under that plan is
  private and commercially usable, unlike the free tier's CC BY 4.0.

## What is not here

No idle clip. Meshy's rigging pass returns walking and running only, and its
action library is not ordered or searchable through the API, so locating an
idle by probing numeric action ids does not converge. The idle comes either
from a library action chosen in the Meshy web UI, or from retargeting
`Tier1_UAL1Idle_Loop`, which is already in place and already tuned to the
authored 200 cm/s walk speed.
