# Journal

Newest first. Append only: entries are never edited or reflowed, because other
records cite them and because their value is that they record what was believed
at the time.

## 2026-08-20 — REW-0003 overnight handoff

- Date: 2026-08-20
- Author: Grok
- Task: REW-0003
- Branch: `main`
- Change: Five Loops Test implementation is In Progress. Unreal 5.8
  project, loop clock, apply order, session save, CleanSave, 4C blockout
  and puzzle actors exist. Lighting intensity no longer calls
  `SetLightUnits`. Loop subsystem draws elapsed `t=` on screen. One
  editor play opened the 4C door, used the radio, started the generator
  and opened the courtyard gate.
- Verification: RewindEditor compiled earlier this wave. PIE ran
  `/Game/Maps/FiveLoops` with `RewindGameMode`. Player report is the
  evidence for the 4C chain. `t=` overlay is in source and was not in
  that play.
- Did not run: FL-01 to FL-16 from `Rewind.CleanSave`. Idle-loop match
  and patrol clock at a stated `t`. Anchor commit, turnstile, hub reach,
  faster second run, quit-and-load. Packaged build.
- Handoff: charter stays in `docs/CURRENT_TASK.md`. Resume by closing
  the editor, compiling RewindEditor, PIE, `Rewind.CleanSave`, then the
  remaining FL script. Nothing is blocked. Stopped for rest.
- Signature: Grok

## 2026-08-19 — REW-0002 Five Loops Test authority

- Date: 2026-08-19
- Author: Grok
- Task: REW-0002
- Branch: `dev/grok-01`
- Change: ADR-0002, ADR-0003 and ADR-0004 are Accepted. Design documents
  own loop and determinism, Anchors, world-state apply order, save and
  session, and the authored facts the first proof needs. Acceptance
  criteria FL-01 to FL-16 are in
  `docs/acceptance/five-loops-test.md`. Insight is not in this proof.
  Echo, asset storage and the license remain open.
- Verification: each FL criterion names one owner; design ownership lines
  do not overlap; live-doc links and fences checked excluding
  `docs/baseline/` and imported legacy files; `git diff --check` clean.
- Did not run: Unreal, Unity, playtest, build. No implementation exists.
- Handoff: `docs/CURRENT_TASK.md` is restored from the template. Next
  work is an implementation task against FL-01 to FL-16. Asset storage
  still blocks the first `.uasset`. This working tree is not committed.
- Signature: Grok

## 2026-08-19 — REW-0001 canonical project state

- Date: 2026-08-19
- Author: Grok
- Task: REW-0001
- Branch: `main`
- Change: this repository is now a RE:WIND docs-first instance rather than a
  leftover protocol-extraction tree. DFC-0001 is cancelled and archived.
  Live documents that still described the protocol were restated. The Unity
  project at `C:\code\reWiND` commit `9f959140e25163bddef390979f23f352e8f0c2f6`
  was inventoried from source into
  `docs/concepts_sandbox/legacy-rewind/code-inventory.md`. Twelve design
  contradictions are registered in
  `docs/concepts_sandbox/legacy-rewind/design-conflicts.md`. The Five Loops
  Test is proposed, not activated, in `docs/backlog/five-loops-test.md`.
  `.gitignore` exists for an Unreal project that does not yet exist.
- Verification: relative links and fenced blocks checked on twenty-four live
  documents; `git diff --check` clean; Unity managers, Chapter 1 scripts and
  `Chapter1_Blockout.unity` GUIDs read; Unreal Engine 5.8 present at
  `C:\Program Files\Epic Games\UE_5.8`. Unity Editor was not launched. No
  Unreal project was created. `docs/baseline/` was not edited and its broken
  internal links were not repaired.
- Finding that was not in the recovery notes: TimeManager, KnowledgeManager
  and AnchorManager are not instantiated in the committed Unity scene, so
  `LoopStartEvent` is never published there.
- Exception: REW-0001 was completed without merging the identity claim to
  `main` first. The charter recorded that exception while still editable.
- Handoff: `docs/CURRENT_TASK.md` is restored from the template. Next work is
  not activated. The candidate is `docs/backlog/five-loops-test.md`, which
  depends on open decisions in `docs/PROJECT_BRIEF.md`. This working tree is
  not committed.
- Signature: Grok

## 2026-08-19 — Repository bootstrap from a frozen baseline

- Date: 2026-08-19
- Author: Claude
- Task: bootstrap, performed under `ACME-0173` in the source repository. This
  repository's own task numbering starts at DFC-0001, which is chartered but not
  started.
- Branch: `main`
- Change: this repository now exists as a docs-first instance running the model
  it intends to specify. `baseline/acme-2026-08-19/` holds fifteen files copied
  verbatim from tag `protocol-baseline-2026-08-19`
  (`75e4b5ee72201d02ad57f22b1a5fcfb3244d521e`) in `zackemannen81/acme-engine`,
  with provenance in `baseline/README.md`. `extraction/ledger.md` classifies
  twenty-eight rule groups as CORE, PROFILE or PROJECT.
- Verification: every copied file was compared by SHA-256 against
  `git show <tag>:<path>` at extraction. All fifteen matched. The comparison is
  repeatable against the tag, which is why the tag exists.
- Not copied, deliberately: the source repository's active charter, because it
  holds another contributor's in-progress work; `docs/JOURNAL.md`, because 6500
  lines of client, product and personal material must never be copied raw and
  journal evidence belongs to the evidence milestone, aggregated and anonymized;
  and the source project's status, architecture, brief and decisions, because
  the model is the workflow rather than the product it was used on.
- Identity prefix: `DFC`, encoding the descriptive method rather than a brand.
  The project name is undecided, and an identity carrying the name would need a
  rename that the addressing rule forbids.
- Honesty note: four rules in the baseline are hours old, not months.
  Path stability, collection discoverability, tense-aware citation validation
  and trunk identity claims were each added on 2026-08-19 after a real failure
  in the source repository. `extraction/ledger.md` marks them so that the
  evidence report does not treat them as equally proven.
- Handoff: DFC-0001 is chartered in `docs/CURRENT_TASK.md` as `Draft` and
  unassigned. It writes `SPEC.md` from the CORE rows of the ledger. Claim the
  identity on `main` before freezing it. This repository is private and
  unlicensed, and is therefore not open source.
- Signature: Claude
