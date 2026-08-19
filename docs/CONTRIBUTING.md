# Contributing

## Licensing of contributions

The license of this repository is an open decision in `docs/PROJECT_BRIEF.md`.
Until that decision exists, treat new contributions as all rights reserved.
Do not assume Apache-2.0 applies to game design, code or assets.

A `LICENSE` file carrying Apache-2.0 text remains from the docs-first model
extraction. That file is not a decision that RE:WIND is open source.

## Git LFS

Unreal binary assets are stored with Git LFS, per
[`ADR-0005`](adr/ADR-0005_asset-storage.md). Install Git LFS before cloning
or pulling, or the working tree will contain pointer files instead of
assets. Do not `git add` a `.uasset` or `.umap` until `.gitattributes`
lists that type.

## Required reading

`AGENTS.md` first, then the reading order it names. Do not start work from an
issue title alone.

## The loop

1. Claim an identity in `docs/TASK_IDS.md` and merge the claim to `main`.
2. Fill `docs/CURRENT_TASK.md` from the template.
3. Move it to `Ready`, which freezes the charter.
4. Work the checklist, keeping it truthful as you go.
5. Verify in proportion to risk, and state what you did not verify.
6. Update every affected owning document in the same change.
7. Archive the task and add a signed journal entry.

## Scope freeze

Once a charter is `Ready`, its goal, primary deliverable, scope, out-of-scope,
definition of done and minimum verification gates do not change. Gates may be
strengthened, never removed.

If the work turns out to need something outside the charter, route it: checklist
step, paused parent with a bounded child, backlog proposal, or concepts sandbox.
Doing it anyway is the failure this rule exists to prevent.

## Files cited by documentation

- Do not rename, move or delete a file that documentation cites. Citations from
  `docs/JOURNAL.md`, `docs/finished/` and accepted decisions cannot be repaired
  afterwards, because those records may not be edited.
- This binds every file type, not only Markdown.
- Do not cite disposable material from an immutable record. Give it a stable
  path first, or describe it instead of linking it. A directory called `temp`
  stops being temporary the moment an archived task names a file inside it.
- Express state in content and in the collection index. A filename is an
  address, not a status field.

## Frozen trees

Nothing under `docs/baseline/` is ever edited. It is provenance for the
working model, and its value is that it can be verified against a tag.

Nothing under `docs/concepts_sandbox/legacy-rewind/` that was imported from
the previous project is ever edited. Corrections belong in this project's own
documents. Inventory and conflict-register files written by this project may
be added there; they are still not authority.

## Writing standards

- Say what is true now, and mark what is intended as intended.
- Do not describe unwritten things as available.
- Name the evidence behind a claim, or do not make the claim.
- Prefer a short document that is current over a long one that is stale.
- Write for a competent stranger with no access to the conversation that
  produced the work.

## Verification

This repository has no tooling yet. Until an Unreal project exists, no build,
test or automation gate may be claimed. Verification is manual review plus
`git diff --check`, and every task states which checks it ran and which it
could not.
