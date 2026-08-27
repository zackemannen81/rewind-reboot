# ADR-0014 — Source organisation and the cost of a new class

Status: Accepted

Date: 2026-08-26

Owns: how C++ under `Rewind/Source/` is laid out and named, and the test a new
class must pass before it is allowed to exist

## Context

`Rewind/Source/Rewind/` holds 43 `.cpp` files and 40 headers in one flat
directory. That is readable today because Chapter 1 is one apartment, one
stairwell, one lift and six interactable objects, and because almost every file
is a real system: the loop clock, the world-state model, the camera rig, the
message subsystem.

It stops being readable at the next chapter. Chapter 2 is designed at roughly
three times Chapter 1's size, with more puzzles, more loop reruns, inner
monologue, transport between spaces, and — the important one — objects the
player can interact with that have no rule behind them at all. Five chapters
are planned.

The existing habit is one class per interactable: `RewindFuse`, `RewindRadio`,
`RewindCodeLock`, `RewindGenerator`, `RewindTurnstile`. Applied to a chapter
full of flavour objects, that habit produces exactly the tree the owner named:
forty files whose names look like `UseTheTempItem41_default_new_new.cpp`, each
holding thirty lines that differ from a neighbour by a string and a mesh.

The failure is not the flat directory. The flat directory is a symptom. The
failure is that **a new object gets a new class**, and nothing in the
repository says it should not.

## Decision

**A C++ class is justified by a new rule, never by a new object. Source is
grouped by responsibility, and every file is named after the single type it
contains.**

### 1. Directory layout

`Rewind/Source/Rewind/` gains subdirectories:

| Directory | Holds | Today's members |
| --- | --- | --- |
| `Core/` | module, ids, logging, settings, save game, game mode | `Rewind`, `RewindIds`, `RewindLog`, `RewindDeveloperSettings`, `RewindSaveGame`, `RewindGameMode` |
| `Loop/` | loop time, world state, session, and how a loop ends | `RewindLoopClock`, `RewindLoopSubsystem`, `RewindWorldStateSubsystem`, `RewindSessionSubsystem`, `RewindLoopParticipant`, `RewindCausalCheckpoint`, `RewindLatchedRewind`, `RewindLoopBreakSignature` |
| `Camera/` | the authored camera of ADR-0007 | `RewindCameraRegion`, `RewindCameraRig` |
| `Player/` | the pawn and what it can address | `RewindCharacter`, `RewindInteractable` |
| `Interactables/` | world objects that carry a rule | `RewindFuse`, `RewindFuseSocket`, `RewindCodeLock`, `RewindRadio`, `RewindLift`, `RewindStairwell`, `RewindGenerator`, `RewindCourtyardGate`, `RewindTurnstile`, `RewindPatrol`, `RewindAnchorBoard` |
| `Messages/` | the player-facing text channel of REW-0018 | `RewindMessageSubsystem`, `RewindMessageCatalog`, `RewindMessageIds`, `RewindMessageQueue`, `RewindMessageOverlay`, `RewindFirstRun` |
| `Chapters/Chapter1/` | one chapter's layout and its measured constants | `RewindProofLayout`, `RewindFourCBlockout`, `RewindAuthoredCourtyard`, `RewindChapter1Metrics`, `RewindAuthoredCourtyardMetrics` |
| `Tests/` | every automation test | the nine `*Test.cpp` files |

`Rewind.Build.cs` lists each subdirectory in `PrivateIncludePaths`, so existing
`#include "RewindFuse.h"` lines keep resolving and the move stays a pure
rename. The move is one commit that only moves files, so `git log --follow`
still follows.

### 2. Naming

- One primary type per file. The file is named after that type with its Unreal
  prefix removed: `ARewindFuse` lives in `RewindFuse.h` and `RewindFuse.cpp`.
- Every name starts with `Rewind`. That is already true and stays true.
- A header with no `.cpp` is for constants and pure functions only, and says so
  in its first comment. `RewindChapter1Metrics.h` is the pattern.
- **Forbidden in a filename or a type name:** `New`, `Old`, `Tmp`, `Temp`,
  `Copy`, `Default`, `Final`, `V2`, a trailing digit, or a space. A name that
  needs one of those is describing the author's history with the file rather
  than what the file is.

### 3. The admission test for a new class

A new class is allowed when one of these is true:

- it owns a **rule** no existing class owns — something that reads or writes
  loop time, world state, knowledge, an Anchor, or a causal checkpoint;
- it is a **boundary**: a subsystem, an interface, or a chapter's layout.

A new class is **not** allowed when it differs from an existing class only by:

- which mesh, sound, material or animation it presents;
- which line of text it speaks, or which message id it uses;
- which knowledge fact it grants or requires;
- where it stands, how big it is, or which loop it appears in;
- which of an existing class's settings are set differently.

Every item in that second list is data. Data goes in the level, in a data
asset, or in `FRewindMessageCatalog`, and the class already exists.

### 4. Flavour objects are one class

Chapter 2's interactable objects that carry no rule get one class,
`ARewindProp`, in `Interactables/`. It presents a mesh, speaks a catalog id on
interact, may require a knowledge fact to respond, and may grant one.
Everything that distinguishes one from another is a property set on the placed
actor.

The first flavour object in Chapter 2 does not get `RewindOldKettle.cpp`. If a
flavour object later earns a rule — it starts gating something, or it has to
reset differently — it graduates to its own class in the same change that gives
it the rule, and its actor is replaced in the level. Graduation is cheap.
Un-writing forty classes is not.

### 5. Chapters

A chapter gets at most one layout class and one metrics header, under
`Chapters/ChapterN/`. Anything a chapter builds that survives the chapter — an
interactable, a loop rule, a message behaviour — belongs in the shared
directory for its kind, not in the chapter folder. A chapter folder that grows
a subfolder is a chapter holding something that is not chapter-specific.

### 6. Tests

Every automation test lives in `Tests/`, named `Rewind<Thing>Test.cpp`, and
registers as `Rewind.<Area>.<Behaviour>`. That already matches the tree; this
records it so it survives the move.

### 7. When to split into modules

Not yet. A second module costs a `Build.cs`, an export macro on every public
type, and a dependency direction that is easy to get wrong and expensive to
reverse. Revisit when `Source/Rewind/` passes roughly 120 files, or when a
chapter genuinely needs to build without the others. Folders first.

## Alternatives considered

**Leave the flat directory and rely on naming discipline.** It has worked for
49 types. Rejected because the thing that breaks is not the count, and a rule
that lives only in the current contributor's head is the failure mode
`AGENTS.md` exists to prevent.

**Split by chapter first.** `Chapter1/`, `Chapter2/`, and so on. Rejected
because most of the code is not chapter-specific: the loop, the camera, the
message channel and the interactables are shared, and filing them under the
chapter that first needed them is how a system ends up copied instead of
reused.

**Split into modules now.** `RewindCore`, `RewindGameplay`, `RewindChapters`.
Rejected as premature, for the reason in point 7. It is the right answer later
and the wrong answer at 49 types.

**Move everything to Blueprint-configurable data assets.** Attractive for
flavour objects, and partly what point 4 does. Rejected as a general rule
because ADR-0006 already decided that C++ owns the systems, and a data asset
that grows behaviour is a class with worse tooling.

## Consequences

- One commit renames 83 files. A review that spans it is noisy once, and
  history still resolves because nothing changes except paths.
- `Rewind.Build.cs` gains a `PrivateIncludePaths` list that must be updated
  when a directory is added. That is the price of keeping bare includes.
- Chapter 2 implementation cannot begin by adding an actor class per puzzle
  object. Its first task has to state which of its objects carry rules, which
  is useful pressure on the design rather than on the code.
- `ARewindProp` has to exist before Chapter 2's dressing does. It does not
  exist yet, and this record does not implement it.
- A reviewer gains one question that is cheap to ask and hard to argue with:
  *what rule does this class own that no other class owns?*

## Acceptance

Accepted by the owner on 2026-08-27. The admission test in point 3 applies from
now: a class is justified by a new rule, never by a new object.

The tree is still flat. Moving 83 files is a separate task, because a rename of
that size and a behaviour change must never share a commit, and it is not
chartered by this record.
