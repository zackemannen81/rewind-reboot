# File Structure

The repository map. Update it in the same change as the structure it describes.

```text
rewind-reboot/
├── AGENTS.md                       entry point and operating rules
├── README.md                       what this is, for a first-time reader
├── LICENSE                         Apache-2.0 text leftover from model extraction; not a product license decision
├── .gitignore                      Unreal generated and local-only paths
├── .gitattributes                  Git LFS types for Unreal binaries, ADR-0005
├── Rewind/                         Unreal Engine 5.8 blank C++ project
│   ├── Rewind.uproject             engine 5.8, module Rewind
│   ├── Source/Rewind/              loop, session, proof layout, puzzle actors, character
│   ├── Config/                     default engine, game, input, editor
│   └── Content/Maps/FiveLoops.umap empty level; 4C blockout spawned in C++
├── docs/
│   ├── CURRENT_TASK.md             the active task, one per branch
│   ├── template_CURRENT_TASK.md    the charter form
│   ├── TASK_WORKFLOW.md            states, freeze, routing, identity
│   ├── PROJECT_BRIEF.md            approved product direction and non-goals
│   ├── CONTRIBUTING.md             how work is done here
│   ├── CURRENT_STATUS.md           what exists now, and the gaps
│   ├── SYSTEMDOC.md                durable shape of repo and, later, game systems
│   ├── JOURNAL.md                  dated, signed work waves, append-only
│   ├── FILESTRUCTURE.md            this file
│   ├── TASK_IDS.md                 identity claims, allocation only
│   ├── adr/                        decisions
│   ├── design/                     game design authority
│   │   ├── loop-and-determinism.md
│   │   ├── anchors.md
│   │   ├── world-state-model.md
│   │   ├── save-and-session.md
│   │   └── chapter-1-authored.md
│   ├── acceptance/
│   │   └── five-loops-test.md      first product proof, FL-01 to FL-16
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
`DerivedDataCache/`) is not in git. No `.uasset` has been committed yet.
