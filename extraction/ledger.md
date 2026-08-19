# Extraction Ledger

Status: Open. Rows are added as the specification is written.

Every rule in `baseline/acme-2026-08-19/` is classified here before it reaches
this project's own documents. The ledger is what makes the generalization
auditable: a reader can take any rule in the specification and find where it
came from, or take any rule in the baseline and find out why it was dropped.

## Classification

| Class | Meaning | Destination |
| --- | --- | --- |
| CORE | True for any long-running work in any domain | The specification, unchanged in force |
| PROFILE | True for a stack, domain or work type | A profile document, clearly optional |
| PROJECT | Source-project identity, architecture or history | Dropped, or replaced by a placeholder |

Tie-break: a rule that cannot be confidently classified as CORE is PROFILE. The
core stays small. A profile rule can be promoted later once several independent
adopters need it; a core rule that turns out to be local is expensive to remove.

## Rules

| Baseline location | Rule | Class | Destination | Change |
| --- | --- | --- | --- | --- |
| `AGENTS.md` "Start Here" | Read the named documents in order before changing the repository | CORE | C-01 | Names parameterized |
| `AGENTS.md` "Documentation Ownership" | One owning document per semantic truth | CORE | C-02 | Verbatim intent |
| `AGENTS.md` "Documentation Ownership" | Current reality, approved direction and history are separate documents | CORE | C-03 | Verbatim intent |
| `AGENTS.md` "Task Workflow" | Charter freezes when status becomes `Ready` | CORE | C-06 | Verbatim intent |
| `AGENTS.md` "Task Workflow" | Documentation is part of the task, not a follow-up chore | CORE | C-10 | Verbatim intent |
| `AGENTS.md` "Pause or Handoff" | A pause records blockers, next steps and resume condition | CORE | C-08 | Verbatim intent |
| `AGENTS.md` "Pause or Handoff" | A contributor must be able to resume without chat history | CORE | C-15 | Verbatim intent |
| `AGENTS.md` "Finish" | Archive the completed task, restore a clean active state | CORE | C-11 | Verbatim intent |
| `AGENTS.md` "Verification Baseline" | Record exactly what was skipped and why | CORE | C-12 | Verbatim intent |
| `AGENTS.md` "One Active Task, Per Branch" | At most one active task per branch | CORE | C-04 | Verbatim intent |
| `AGENTS.md` "Addressing and Discoverability" | A cited record keeps its path; status lives in content | CORE | C-16 | Verbatim intent |
| `AGENTS.md` "Addressing and Discoverability" | Each collection declares index or naming convention | CORE | C-17 | Verbatim intent |
| `AGENTS.md` "Addressing and Discoverability" | Present-tense surfaces are validated; history and charters warn | CORE | C-16 note | Verbatim intent |
| `docs/TASK_WORKFLOW.md` "Task Identity" | Identities are claimed on the trunk before `Ready` | CORE | C-18 | Verbatim intent |
| `docs/TASK_WORKFLOW.md` | Blocking prerequisite pauses the parent and activates a bounded child | CORE | C-07 | Verbatim intent |
| `docs/TASK_WORKFLOW.md` "Non-blocking Discoveries" | Discoveries route to the backlog rather than expanding scope | CORE | C-07 | Verbatim intent |
| `docs/concepts_sandbox/README.md` | Undecided material has a named non-authority home | CORE | C-13, C-14 | Verbatim intent |
| `docs/TASK_IDS.md` | Claims are appended in strictly ascending order | CORE | C-18 | Verbatim intent |
| `docs/TASK_IDS.md` | The register records identity, never status | CORE | C-18 | Verbatim intent |
| `docs/template_CURRENT_TASK.md` | Charter sections: goal, deliverable, scope, done, gates | CORE | C-05 | Names parameterized |
| `AGENTS.md` "Verification Baseline" | Documentation tasks verify links, fences and diagrams | PROFILE | Documentation profile | Retained |
| `AGENTS.md` "Verification Baseline" | Code tasks define typecheck, unit, conformance, integration, scenario gates | PROFILE | Software profile | Retained as an example set |
| `AGENTS.md` "Dependency Direction" | apps to adapters to modules to core | PROFILE | Software profile | Example guardrail, not a requirement |
| `AGENTS.md` "Live-call policy" | Bounded plan, derived call count, no arbitrary ceiling | PROFILE | AI-systems profile | Retained as an optional add-on |
| `AGENTS.md` "Safety and External Effects" | Credentials, live calls and deployments need explicit scope | PROFILE | Software and operations profiles | Retained |
| `AGENTS.md` "Fixed Architecture Guardrails" | Core stays domain-neutral, model output is never canonical state | PROJECT | Dropped | Shown only as an example of what a profile may add |
| `AGENTS.md` "Project Identity" | Source project purpose, phase and milestone history | PROJECT | Dropped | Replaced by a placeholder section |
| `docs/FILESTRUCTURE.md` | Everything below the conventions sections | PROJECT | Dropped | Only the conventions generalize |

## Rules that were not in the original model

Four rules in the baseline were added on 2026-08-19, after this extraction was
already planned, because the source repository hit the failures they prevent.
They are marked here so that nobody mistakes them for long-tested material.

| Rule | Added after | Age at extraction |
| --- | --- | --- |
| A cited record keeps its path (C-16) | A rename broke 39 links in append-only history | Hours |
| Collections declare index or naming convention (C-17) | An unindexed operations directory was found by the check itself | Hours |
| Present-tense validation, tense-aware exemptions | The check failed a charter that named its own deliverable | Hours |
| Identities are claimed on the trunk (C-18, C-04 scope) | Two actors froze two charters under one identity | Hours |

The rest of the model has been in continuous use across several repositories,
stacks and actors. The claim ladder in the evidence report must keep that
distinction visible: long-tested and newly added rules do not carry the same
weight of evidence.
