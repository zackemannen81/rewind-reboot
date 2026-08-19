# Technician test, run against this repository at bootstrap

- Date: 2026-08-19
- Updated at: 2026-08-19
- Owner: Claude
- Status: Record of one self-assessment. Not a conformance result, because no
  specification exists yet to conform to.

## What this does not decide

It does not define the technician test, set a conformance level, or claim that
this repository passes a standard. It records the answers a reader can find
today, so that the same questions can be asked again later and compared.

Self-assessment by the actor who wrote the repository is the weakest form of
this test. The real version needs someone who has not seen it.

## The questions

**What is the active task?**
DFC-0001, in `docs/CURRENT_TASK.md`: write `SPEC.md` from the CORE rows of the
extraction ledger. Status `Draft`, unassigned, not started.

**Which document owns the relevant truth?**
`AGENTS.md` lists the ownership map, and `docs/SYSTEMDOC.md` repeats it as
semantic roles. A reader looking for what exists goes to
`docs/CURRENT_STATUS.md`; for approved direction, `docs/PROJECT_BRIEF.md`; for
how work is run, `docs/TASK_WORKFLOW.md`.

**What exists now?**
`docs/CURRENT_STATUS.md` answers in two tables: the frozen baseline, the
provenance, the ledger and this docs-first instance exist; the specification,
templates, validator, profiles, case studies, evidence report, license and name
do not.

**What changed, when, why and by whom?**
One entry in `docs/JOURNAL.md`, dated 2026-08-19, signed, naming the source tag
and revision, what was copied, what was deliberately not copied and why.

**What was verified and what was not?**
Verified: fifteen baseline files compared by SHA-256 against
`git show <tag>:<path>`, all matching; this repository's own fifteen Markdown
documents checked for link resolution and fence balance, zero problems.
Not verified: nothing here is checked by tooling, because there is none. The
twenty-two unresolved links inside `baseline/` are recorded as correct rather
than repaired.

**What remains, and what is explicitly out of scope?**
DFC-0001 states both. Its out-of-scope list names templates, profiles, the
validator, examples, case studies and the evidence report as separate tasks, and
forbids editing the baseline or simplifying its rules.

**Which document may change which truth?**
`AGENTS.md`, Documentation Ownership. The two boundaries that carry the most
weight: `baseline/` is never edited by anything, and `docs/concepts_sandbox/`
may never be cited as authority — including this file.

## Honest weaknesses

- The assessor wrote the repository. Every answer was easy to find because the
  author knew where to look.
- There is no specification, so "passes the technician test" cannot mean
  "conformant". It means a reader can currently answer the seven questions.
- Four of the rules this repository operates under are hours old.
  `extraction/ledger.md` marks them, and the evidence report must not treat
  them as equally proven.
