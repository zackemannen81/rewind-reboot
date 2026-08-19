# Legacy RE:WIND

Status: Non-authority. Frozen at import. Never edited.
Imported: 2026-08-19
Owner: unassigned
Updated: 2026-08-19

## What this is

The design documents, roadmaps, task files and process notes of the previous
RE:WIND project, imported as source material for the reboot.

Source: `C:\code\reWiND`, commit `9f959140e25163bddef390979f23f352e8f0c2f6`,
branch `main`, last commit 2025-10-24, working tree clean at import. The source
repository was not modified, and nothing here depends on it.

## What this material is not

This material:

- describes earlier intentions and experiments, not decisions;
- contains contradictions, some of which are load-bearing;
- does not describe this repository, its engine, or any current implementation;
- records several systems as `DONE` that were never implemented;
- may not be implemented, cited as authority, or used to justify scope.

No task, design document or decision record may cite anything here as
authority. Reading it is encouraged. Acting on it is not.

## What it does not decide

Nothing. It decides nothing. In particular it does not decide the engine, the
determinism model, what an Anchor is, what Echo does, whether Insight exists,
the loop length, the chapter structure, the art direction, or the schedule.

## How material leaves this directory

```text
legacy material
  → activated task or discovery
  → explicit decision, recorded in docs/adr/
  → restatement in an owning document under docs/
  → implementation
```

A restatement is a rewrite, not a copy. When a rule moves into `docs/design/`,
it is stated in this project's own terms, with the reasoning that made it a
decision. The legacy file stays where it is, unchanged, as provenance.

## Contents

| Path | What it holds |
| --- | --- |
| [`original-gdd/`](original-gdd/) | The 123 KB design document, the art and asset GDD, the resources and roadmap GDD, and the technical solutions note |
| [`old-roadmaps/`](old-roadmaps/) | The five-phase development plan, the Chapter 1 blockout and environment plans, the art production plan, and three asset lists |
| [`old-task-material/`](old-task-material/) | Seventeen task files, the development journal, and the previous project's own agent and contribution instructions |
| [`code-inventory.md`](code-inventory.md) | What the Unity implementation actually did, verified file by file against the source |
| [`design-conflicts.md`](design-conflicts.md) | Contradictions found in the material above, each stated as a question a decision must answer |
| [`reboot-plan-source.md`](reboot-plan-source.md) | The discussion this reboot was performed from, kept verbatim as provenance for why the repository is shaped this way |

## What was deliberately not imported

| Not imported | Reason |
| --- | --- |
| `Assets/Scripts/` (about thirty C# files) | Engine decision changed to Unreal in `ADR-0001`. The code is reference, and `code-inventory.md` records what it did in more usable form than the code itself. The source repository still holds it |
| `Assets/` (140 MB of art, audio and scenes) | Unity assets, superseded by the engine decision. Importing them would put binary weight in history for material that cannot be used |
| `Packages/`, `ProjectSettings/`, the Unity scene | Unity project configuration, of no use here |
| Git history of the source repository | It stays with the source repository, where it remains readable |

## The value that is actually here

Two things, and it is worth naming them so the material is read for the right
reasons.

**Authored Chapter 1 content.** The radio code `7312` revealed in fragments over
successive loops, the fuse and generator chain, the courtyard gate, the guard's
twenty-second window, the turnstile's thirty-second breathing cycle with a
2.5-second opening, and the five-loop structure from cold start to breach. This
is specific, coherent, and engine-independent. It is the reason the first
product proof does not need new design invention.

**A recorded failure.** The previous project's status surface said its core
systems were complete while the loop could not reset the world. The evidence
discipline in `AGENTS.md` exists because of this material, and it is more useful
as a warning than as a plan.
