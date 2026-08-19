# ADR-0005 — Asset storage

Status: Accepted
Date: 2026-08-19
Deciders: MrWhite
Supersedes: nothing
Superseded by: nothing

## Context

RE:WIND is built in Unreal Engine 5.8. The first product proof will
create a `.uproject` and, soon after, `.uasset` / `.umap` files. Those
are binary. Once they are in git history as ordinary blobs, the cost of
changing strategy is paid on every clone forever.

`docs/PROJECT_BRIEF.md` listed three options: git, Git LFS, or storage
outside the repository. ADR-0001 already said this must be settled
before the first `.uasset` is committed. REW-0003 is that moment.

The repository is private, on GitHub, with one development machine
today. A later clone must be able to open the project. Git LFS 3.7.1 is
installed here.

## Decision

Unreal binary assets that belong in this repository are stored **in git
through Git LFS**.

Source that is text (C++, Build.cs, `.ini`, `.uproject`, Markdown) stays
ordinary git objects. Generated editor output stays out of git, as
already listed in `.gitignore`.

The LFS type list lives in `.gitattributes`. Adding a binary type is an
edit to that file in the same change as the first file of that type, not
a silent `git add`.

This decision does not license large marketplace packs or final art.
Those remain a deliberate size event.

The first `.uasset` must not appear in history before this record. That
is a REW-0003 verification gate.

## Alternatives considered

**Ordinary git blobs for `.uasset` and `.umap`.** Rejected. Unreal
rewrites those files often. History and clones grow without a bound we
can undo.

**Keep `Content/` out of git.** Rejected for the first product proof. The
repository would not contain a playable project. A later clone could not
run FL-01 to FL-16.

**Perforce, GitHub Releases, or another store outside git.** Rejected
for this phase. There is no such store in operation, and the proof is
small enough that LFS on GitHub is enough if Content stays a blockout.

## Consequences

**Accepted costs:**

- Every clone needs Git LFS. `git clone` without it yields pointer
  files, not assets.
- GitHub LFS storage and bandwidth are finite. Content for the Five
  Loops Test must stay a blockout. A 140 MB art drop like the Unity
  project is a new decision, not a side effect.
- Prefer spawning blockout geometry in C++ or in a single small map
  over importing kits.

**Accepted benefits:**

- A clone of `main` can open the Unreal project once one exists.
- Binary history stays pointers, so strategy remains reversible compared
  with raw blobs.

**Consequences that bind future tasks:**

- No task may commit a `.uasset`, `.umap`, or other LFS-listed type as a
  plain git blob.
- No task may add marketplace, scan, or film-quality assets without an
  explicit size decision.
- Changing this strategy after binaries are in LFS is still expensive.
  Do it only with a new ADR.
