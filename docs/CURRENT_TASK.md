# Current Task

Task ID: REW-0005
Parent Task: REW-0004, paused pending this task
Status: Ready
Owner: Claude
Created: 2026-08-23
Last updated: 2026-08-23
Charter frozen at: 2026-08-23

## Read First

- `AGENTS.md`
- `docs/TASK_WORKFLOW.md`
- `docs/PROJECT_BRIEF.md`
- `docs/CURRENT_STATUS.md`
- `docs/design/anchors.md`
- `docs/design/world-state-model.md`
- `docs/design/chapter-1-authored.md`
- `docs/design/save-and-session.md`
- `docs/adr/ADR-0002_world-determinism.md`
- `docs/adr/ADR-0003_anchor-semantics.md`
- `docs/backlog/fuse-radio-and-elevator.md`
- `docs/backlog/what-anchors-are-for.md`
- `docs/backlog/loop-pressure-and-interaction.md`
- `docs/playtests/five-loops-2026-08-22.md`
- `docs/paused/REW-0004_chapter-1-spatial-and-camera-foundation.md`

## Task Summary

Write the design authority for what a rewind takes back, what an Anchor is
worth, and the Chapter 1 chain of radio, fuse, stairs, lift and gate. Rules
only. No implementation, no level building.

## Task Charter

The charter is editable while status is `Draft` and immutable once status is
`Ready`.

### Goal

Rules that state why an Anchor is worth having, and a Chapter 1 chain in which
knowledge and one Anchor together make a route possible that was not.

### Primary Deliverable

Amended or superseding design documents that own: what an Anchor is worth and
how scarce Anchors must be; the radio's cost; the fuse as a carried, contested
resource; and the lift-or-stairs consequence. Every rule has exactly one owning
document.

### In Scope

- State what makes an Anchor worth having. `anchors.md` says what an Anchor is
  and gives no reason. The reason is that it frees a resource and so changes the
  consequences of other actions in the next rewind.
- Decide whether Anchor scarcity is a rule with its own argument, or remains a
  consequence of ADR-0003's scope. If it supersedes ADR-0003's reasoning, write
  the ADR.
- Decide the radio's cost in elapsed loop time, and whether tuning is a mechanic
  or a single interaction. This is interaction cost, which nothing has decided.
- Decide what the fuse is: a carried single-use item with two possible
  consumers, or something else. Decide which bucket in `world-state-model.md`
  holds a carried object, and whether that model needs a bucket it does not have.
- Decide the lift and the stairs: what each costs, and what the lift depends on.
- Supersede the parts of `chapter-1-authored.md` that the above contradicts, in
  particular that the fuse is "routed" to one consumer.
- Keep every rule determinism-safe. The imported GDD makes the radio clearer on
  later loops, which `ADR-0002` forbids. The owner's version, where listening
  costs time and the player learns, is the one to restate.
- Restate what the acceptance criteria must become, or record that FL-01 to
  FL-16 survive unchanged. Identifiers are permanent.
- Update `docs/CURRENT_STATUS.md`, `docs/SYSTEMDOC.md` and `docs/JOURNAL.md` in
  the same change as what they describe.

### Out of Scope

- Implementation of any kind. No C++, no level geometry, no assets.
- The camera. ADR-0007 is decided and REW-0004 implemented it.
- The spatial rebuild. That is REW-0004, paused, and it resumes after this.
- Chapters 2 to 5, Echo, Insight, later temporal mechanics.
- Consequence and death. A drone that can catch the player on the stairs is a
  separate decision, and nothing defines what killing the player means.
- Art direction, which stays gated on the non-goal list in
  `docs/PROJECT_BRIEF.md`.
- Inventing rules the chain does not need in order to be stated.

### Definition of Done

- Every rule above lives in exactly one owning document under `docs/design/`,
  and no two documents state the same rule.
- The Chapter 1 chain is stated in testable terms: a naive run, a run that knows
  the code, and a run with the gate anchored each have a stated shape and a
  stated reason they differ.
- The knowledge-saving rule in `chapter-1-authored.md` is satisfiable by the
  stated chain, shown by argument rather than by measurement, since nothing is
  built yet.
- Every rule is checked against `ADR-0002`: no world object reads loop count.
- The acceptance criteria are either unchanged, or amended with their
  identifiers intact and the change recorded.
- `docs/JOURNAL.md` has a signed entry and this task is archived, and
  `REW-0004` is restored to `docs/CURRENT_TASK.md`.

### Minimum Verification Gates

- [ ] Every new or changed rule has exactly one owning document, checked by
      reading the ownership lines
- [ ] No rule makes a world object depend on loop count
- [ ] The chain's three loops are stated in testable terms
- [ ] Manual link and fence review
- [ ] `git diff --check` clean

## References

- `docs/backlog/fuse-radio-and-elevator.md`
- `docs/backlog/what-anchors-are-for.md`
- `docs/concepts_sandbox/legacy-rewind/original-gdd/GDD_About.md`, non-authority.
  Contains the chain in detail, including the explicit fuse choice, the radio
  channels, the kitchen drawer, the generator mini-puzzle and the stairwell
  drone. Read for detail, never cited as a rule.
- `docs/concepts_sandbox/legacy-rewind/design-conflicts.md`, C1 and C4.

## Checklist

- [x] Merge the `REW-0005` identity claim to `main` before moving this charter
      to `Ready`
- [x] Write what an Anchor is worth, and decide the scarcity question:
      ADR-0008 and the rule in `docs/design/anchors.md`
- [ ] Decide the radio's cost
- [ ] Decide what the fuse is, and which bucket holds it
- [ ] Decide the lift and the stairs
- [ ] Supersede the contradicted parts of `chapter-1-authored.md`
- [ ] State the three loops in testable terms
- [ ] Check every rule against ADR-0002
- [ ] Update status, system document
- [ ] Add a signed journal entry, archive this task, restore REW-0004

## Decisions and Notes

- The three-layer model the owner described is already canon in
  `world-state-model.md`: LoopWorld and PlayerBody discarded, Knowledge
  persisting, Anchors persisting. This task does not redesign it, and should
  resist the temptation to restate it in different words.
- The strongest new idea is that an Anchor frees a resource rather than granting
  an unlock. If that becomes a rule, it is also a test a candidate Anchor can be
  held against, which is more useful than a list of permitted overrides.
- The chain must not require Insight. ADR-0003 abolished it for this proof, and
  the chain does not need it.

## Charter Amendment Log

Only non-semantic corrections are allowed after `Ready`.

- none

## Verification

- [ ] Ownership review across `docs/design/`
- [ ] Loop-count independence review
- [ ] Testable statements for the three loops
- [ ] Manual link and fence review
- [ ] `git diff --check`
- [ ] Document skipped checks and reasons

## Documentation Updates

- [ ] `docs/design/anchors.md`
- [ ] `docs/design/chapter-1-authored.md`
- [ ] `docs/design/world-state-model.md`, only if a carried object needs a bucket
- [ ] `docs/acceptance/five-loops-test.md`, only if a criterion changes
- [ ] `docs/CURRENT_STATUS.md`
- [ ] `docs/SYSTEMDOC.md`
- [ ] `docs/JOURNAL.md`
- [ ] An ADR, if Anchor scarcity supersedes ADR-0003's reasoning

## Handoff and Follow-ups

- Current state: Ready, charter frozen 2026-08-23. Work is on branch
  `rew-0005/anchor-consequence`.
- Next recommended step: claim `REW-0005` on `main`, then decide what an Anchor
  is worth, because the Chapter 1 chain is an instance of that answer and reads
  as arbitrary without it.
- Blockers: none.
- Child tasks: none. This task is itself REW-0004's child.
- Resume condition: any session with the repository. This task writes rules
  and needs no editor.
- Verification gaps: none yet; nothing has been done.
- Open questions: whether a carried fuse needs a bucket `world-state-model.md`
  does not have. PlayerBody is discarded at loop start, which is the behaviour a
  carried fuse wants, but PlayerBody is described as pose, stance and traversal
  state rather than as things held.

## Finalize When Complete

- Archive this file under `docs/finished/`.
- Restore `docs/paused/REW-0004_chapter-1-spatial-and-camera-foundation.md` to
  `docs/CURRENT_TASK.md`, unchanged.
- Add a signed `docs/JOURNAL.md` entry.
- If Goal or Definition of Done changed, supersede this task instead of
  rewriting it.
