# Baseline

Status: Frozen. Never edited.

This tree holds the hardened docs-first model exactly as it stood in the
repository that produced it. It is provenance, not documentation of this
project, and nothing here is this project's authority.

## acme-2026-08-19

| Field | Value |
| --- | --- |
| Source repository | `zackemannen81/acme-engine`, private |
| Source revision | `75e4b5ee72201d02ad57f22b1a5fcfb3244d521e` |
| Source tag | `protocol-baseline-2026-08-19` |
| Extracted | 2026-08-19 |
| Extracted by | Claude, under ACME-0173 |
| Rule | Never edited. Corrections belong in this project's own documents. |

Every file was written with `git show <tag>:<path>` and verified byte for byte
against the tagged revision:

```bash
git show "$TAG:$f" | sha256sum
sha256sum "baseline/acme-2026-08-19/$f"
```

All fifteen files matched at extraction time. Re-run the comparison against the
tag if you ever need to prove this tree was not touched.

## Why the copy is verbatim

The value of the source model is not that it reads well. It is that almost
every rule was added after a specific observed failure, and several of those
failures are invisible in the text. A generalization pass written from memory
keeps the parts that sound reasonable and silently drops the failure knowledge.

So the baseline is transcribed, and generalization happens only in
`extraction/ledger.md`, one classified rule at a time, where every change is
visible and reversible.

## Links inside this tree do not resolve

The baseline is a partial verbatim copy, so its internal links point at files in
the source repository that were not copied: backlog proposals, the concepts
sandbox, tooling. Twenty-two such links were broken at extraction, and that is
the correct state.

Repairing them would mean editing the baseline, which would destroy the one
property it exists for. Any link checker in this repository must exclude
`baseline/`, and must say why rather than special-casing it silently.

## What was deliberately not copied

| Not copied | Reason |
| --- | --- |
| The source repository's active charter | It holds another contributor's in-progress work, and copying an in-flight charter is neither verbatim evidence nor ours to take |
| `docs/JOURNAL.md` | 6500 lines containing client, product and personal material. Journal evidence belongs to the evidence milestone, aggregated and anonymized, never copied raw |
| `docs/CURRENT_STATUS.md`, `docs/SYSTEMDOC.md`, `docs/PROJECT_BRIEF.md`, `docs/adr/` | Project-specific reality, architecture and decisions. The model is the workflow, not the product it was used on |
| `docs/design/`, `docs/acceptance/`, `docs/ops/` | Same reason |

Five archived tasks are included as real lifecycle evidence: the two that
founded the model and the three that hardened it on 2026-08-19.
