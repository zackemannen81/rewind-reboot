# REW-0038 Apartment 4C concept-target wave

Status: Frozen
Owner: Codex (process supervisor)
Parent task: REW-0038
Implementation child: REW-0039
Base: `90d3d62`
Frozen: 2026-08-26

## Outcome

One Codex worker brings the exact-camera Apartment 4C frame materially closer
to `docs/concept/apartment-4c-interior-light-enhanced.png` using tracked
project-owned assets and the accepted renderer, without changing gameplay or
camera contracts.

## Allocation

| Role | Identity | Clone | Branch | Authority |
| --- | --- | --- | --- | --- |
| Operator | REW-0038 / Codex supervisor | `C:\code\rewind-reboot` | `codex/rew-0038-4c-concept-supervisor` | Target, scope, decisions, task IDs, process control, review, merge, canonical gates |
| Worker | REW-0039 / Codex | clean clone under `C:\code\rewind-multiagents` | `codex/rew-0039-4c-concept-art` | Frozen child charter, owned implementation paths, worker build/captures/tests, PR |

## Exclusive worker paths

- `Rewind/Content/Maps/FiveLoops_Stairwell_Blockout.umap`
- `Rewind/Content/Props/Chapter1/`
- `Rewind/Content/Art/Materials/BuildingSlice/`
- New task-specific content under
  `Rewind/Content/Art/Apartment4C/REW0039/`
- Task-specific sources under `Rewind/ArtSource/Apartment4C/REW-0039/`
- Task-specific automation/import scripts under
  `Rewind/ArtSource/Scripts/REW-0039_*.py`
- Task-specific captures under `Rewind/ArtSource/Screenshots/REW-0039_*`
- Child-owned updates to `docs/CURRENT_STATUS.md`, `docs/SYSTEMDOC.md`,
  `docs/JOURNAL.md` and its archive while the worker is isolated

The worker must not edit `docs/TASK_IDS.md`, parent `docs/CURRENT_TASK.md`,
this wave plan, unrelated maps/gameplay source, or the owner's untracked
canonical `Rewind/ArtSource/Props/Chapter1/` and `dev/`.

## Global locks

- One Unreal Editor/build/test lease for this wave.
- The operator closes the canonical editor before worker launch.
- The worker owns its isolated Unreal process until every editor and commandlet
  it started has stopped.
- The operator does not open the canonical editor until the worker releases the
  lease and the branch is reviewed.

## Asset-generation decision

Existing tracked props and surfaces are mandatory first inventory. A Meshy call
requires a written worker gap report naming the absent silhouette, intended
placement, expected credit cost and why existing/simple geometry cannot serve.
Only the operator approves the call. Secrets remain local and are never printed
or committed. New generated sources use the REW-0039 task-specific source path.

## Checkpoints

1. Frozen child charter, baseline inventory and exact-camera baseline.
2. Shell/material/reflection correction.
3. Window/city and major prop-zone composition.
4. Kitchen/technical-wall/secondary dressing.
5. Lighting hierarchy and exact-camera after frame.
6. Collision/traversal/interaction clean-up.
7. Clean-save, build, full tests, provenance, archive and PR.

Every checkpoint is committed and pushed before the next risky editor mutation.

## Review and merge

The operator reviews the complete diff, LFS objects, asset inventory,
before/after frame and every named gate. Visual insufficiency returns to the
same child; no second worker edits the map. After merge, the operator runs the
canonical build, full suite, standard PIE traversal and final exact-camera
capture before REW-0038 can complete.

## Recovery

Git commits and the frozen child charter are authority. MCP status is only a
process signal. After a crash, verify editor/process ownership, inspect the
worker branch and resume from the last pushed checkpoint without replaying
already committed map mutations.

