# File Structure

The repository map. Update it in the same change as the structure it describes.

```text
rewind-reboot/
├── AGENTS.md                       entry point and operating rules
├── README.md                       what this is, for a first-time reader
├── LICENSE                         Apache-2.0 text leftover from model extraction; not a product license decision
├── .gitignore                      Unreal generated and local-only paths
├── .gitattributes                  Git LFS types for Unreal binaries, ADR-0005
├── .mcp.json                       the running editor's MCP endpoint; works only while the editor is open
├── .codex/config.toml              project-scoped Codex client for the same editor endpoint
├── Rewind/                         Unreal Engine 5.8 blank C++ project
│   ├── Rewind.uproject             engine 5.8; runtime and editor modules; MCP/toolset plugins
│   ├── Source/Rewind/              loop, session, Chapter 1, authored camera, courtyard, player messages and named runtime tests
│   ├── Source/RewindEditor/        editor-only MCP PIE-input and clean screenshot toolset
│   ├── ArtSource/Textures/         owner-authored 4c_* JPEG sources, plus the generated kit:
│   │   ├── EnvironmentTextureKit-v1/ REW-0027 JPEG surface/backdrop import sources
│   │       └── Decals_Emissive/  REW-0028 decal atlas and neon PNG sources
│   │                               both outside Content/ so the editor never scans them
│   ├── ArtSource/Audio/           REW-0033 generated WAV sources: radio digits,
│   │                               station and static beds, clicks, footsteps
│   ├── ArtSource/Characters/Returner/
│   │                               owner-generated Returner source FBX and provenance
│   ├── ArtSource/Screenshots/      rendered evidence kept with the task that produced it
│   ├── ArtSource/Scripts/          reproducible editor Python imports, rebuilds and
│   │                               inspections. capture_region.py renders a camera
│   │                               region's authored shot headlessly; verify_audio.py
│   │                               and verify_surface_bindings.py read results back
│   │                               rather than trusting a generator's own report
│   ├── Config/                     default engine, game, input, editor
│   └── Content/
│       ├── FiveLoops_Handmade.umap owner-authored original construction map
│       ├── FiveLoops_Handmade2.umap
│       │                            owner-authored revised stair construction
│       ├── Audio/Chapter1/         fifteen generated SoundWaves, wired by REW-0035
│       ├── Maps/FiveLoops.umap     empty level; Chapter 1 blockout spawned in C++
│       ├── Maps/FiveLoops_Stairwell_Blockout.umap
│       │                            default authored stairwell, 4C, halls, lift and courtyard slice
│       ├── Maps/Reference/FiveLoops_Handmade2_Reference.umap
│       │                            untouched owner-layout reference
│       ├── Art/Materials/Stairwell/
│       │                            project-owned masters and instances for the presentation slice
│       ├── Art/Materials/BuildingSlice/
│       │                            project-owned 4C floor instance and 4C sign emissive
│       ├── Art/Materials/Decals/   deferred-decal master and REW-0028's 16 cell instances
│       ├── Art/Materials/Surfaces/  M_REW_Surface and eight REW-0027 tileable-surface instances
│       ├── Art/Textures/Decals/    REW-0028's three opacity-mask atlases and three neon textures
│       ├── Art/Textures/Surfaces/   twelve REW-0027 textures: eight tileable surfaces and
│       │                            four backdrop/poster assets
│       ├── Characters/Returner/    runtime Returner mesh, skeleton and four own-skeleton clips
│       └── Characters/Tier1/       retained 21 CC0 animations and two pack mannequins
├── docs/
│   ├── CURRENT_TASK.md             active task when one is approved; otherwise the restored template
│   ├── template_CURRENT_TASK.md    the charter form
│   ├── TASK_WORKFLOW.md            states, freeze, routing, identity
│   ├── PROJECT_BRIEF.md            approved product direction and non-goals
│   ├── CONTRIBUTING.md             how work is done here
│   ├── CURRENT_STATUS.md           what exists now, and the gaps
│   ├── SYSTEMDOC.md                durable shape of repo and, later, game systems
│   ├── EDITOR_AUTOMATION.md        editor, MCP, plugins, toolsets, build and agent playtests
│   ├── JOURNAL.md                  dated, signed work waves, append-only
│   ├── FILESTRUCTURE.md            this file
│   ├── TASK_IDS.md                 identity claims, allocation only
│   ├── adr/                        decisions, including event-driven loop termination
│   ├── design/                     game design authority
│   │   ├── loop-and-determinism.md
│   │   ├── anchors.md
│   │   ├── world-state-model.md
│   │   ├── save-and-session.md
│   │   ├── chapter-1-authored.md
│   │   ├── chapter-2-authored.md   REW-0034 design authority only; not implementable yet
│   │   ├── camera-and-movement.md
│   │   ├── stairwell-visual-direction.md
│   │   ├── player-messages.md
│   │   └── tutorial-and-first-run.md
│   ├── acceptance/
│   │   ├── five-loops-test.md      first product proof, FL-01 to FL-16
│   │   └── chapter-2-conduit-test.md
│   │                               Chapter 2 criteria; no build evidence for any of them
│   ├── playtests/                  indexed evidence, including the Chapter 1 three-loop record
│   ├── concept/                    nine owner targets and construction references; never rules
│   ├── backlog/                    non-activated proposals
│   ├── paused/                     frozen parents awaiting a condition
│   ├── finished/                   archived tasks, immutable
│   ├── concepts_sandbox/           excluded ideas and imported legacy, never authority
│   └── baseline/                   frozen docs-first source, never edited, never authority
```

## Conventions

Every collection under `docs/` declares one discoverability mode in its
`README.md`: `index`, meaning every member is listed there, or a naming
convention that makes every member addressable without a list.
`docs/finished/` uses the convention `{PREFIX}-NNNN_task-slug.md`; the others
are indexed.

Collections whose members carry lifecycle state also declare
`Member state: required`, and each member declares a `Status:` line under its
title. State lives in content and in the index, never in a filename.

A record cited by `docs/JOURNAL.md`, `docs/finished/` or an accepted decision
keeps its path. Renaming it cannot be repaired, because the citations live in
records that may not be edited.

## Not yet present

Generated Unreal output (`Binaries/`, `Intermediate/`, `Saved/`,
`DerivedDataCache/`) is not in git. Tracked imported content is the measured
12.61 MiB Tier 1 character subset under `Rewind/Content/Characters/Tier1/`.
The stairwell and building-slice material family uses only project-authored
Unreal assets under `Rewind/Content/Art/Materials/`. The owner's hand-authored `4c_*` texture sources are tracked at
`Rewind/ArtSource/Textures/`, 76 KB. REW-0028 also tracks six supplied
decal/neon PNG sources under `EnvironmentTextureKit-v1/Decals_Emissive/`,
with their import settings and grid evidence recorded in
`EnvironmentTextureKit-v1/Decals_Emissive/REW-0028-decal-provenance.md`.
Locally imported Fab, OldWall and
PaintedBrickWall working material remains untracked and local: ADR-0011
decided that third-party packs stay outside the tracked dependency closure,
and REW-0022 unbound the default map from those packages. Its 2026-08-25
amendment corrects `Art/Texture`, which is owner-authored and was never what
that rule was about; REW-0024 re-imports and re-binds it.
