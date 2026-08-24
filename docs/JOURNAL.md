# Journal

Newest first. Append only: entries are never edited or reflowed, because other
records cite them and because their value is that they record what was believed
at the time.

## 2026-08-24 — REW-0010 complete, owner-scale stairwell blockout

- Date: 2026-08-24
- Author: Codex
- Task: REW-0010
- Branch: `codex/rew-0010-stairwell-blockout`
- Change: preserved the owner-authored Handmade2 construction as the untouched
  stable reference `/Game/Maps/Reference/FiveLoops_Handmade2_Reference` and
  built `/Game/Maps/FiveLoops_Stairwell_Blockout` as a separate human-scale
  four-floor switchback. Floor surfaces are at Z = 0, 300, 600, 900 and
  1200 cm. Four alternating flights each use seventeen 17.65 cm risers,
  28 cm treads and 170 cm width; invisible ramps provide continuous capsule
  collision while visible steps retain the authored shape.
- Door and circulation correction: floors 1-3 have static closed doors on the
  same side as 4C. The entrance and fourth-floor openings remain open. The
  stairs follow the owner-corrected guardrail direction rather than the earlier
  sketch's outer first run.
- Runtime isolation: the standalone map tags its WorldSettings with
  `Rewind.SkipProofLayout`. `URewindWorldStateSubsystem` honors that opt-out, so
  the C++ Five Loops proof is not generated over the authored map in PIE. The
  existing proof remains the default for maps without the tag.
- Character correction: the imported mannequin is offset to the capsule bottom
  and rotated -90 degrees relative to the character root. Feet now meet the
  collision floor and forward locomotion uses the animation's forward facing.
- Verification: the stable reference and its source each enumerated 26 actors;
  `stairwayCamera` matched at `(17020, -6330, 1360)`, rotation `(0, 180, 0)`.
  The saved target reopened with 33 actors. Standard possessed PIE descended
  all four flights and reported Z = 1298, 998, 698, 398 and 98 cm at the five
  levels; held keys were released before PIE stopped. An MCP viewport capture
  showed the complete four-floor composition. Unreal Editor Win64 Development
  built successfully after the C++ changes.
- Storage evidence: the target map is 441,048 bytes and the reference is
  462,724 bytes. Both map paths resolve Git attributes to LFS for filter, diff
  and merge with text unset. Locally imported Fab, OldWall and PaintedBrickWall
  content was inventoried but remains untracked; REW-0010 neither approves its
  license/provenance nor spends repository storage on it.
- Not verified: no packaged build and no final environment-art pass. Runtime
  vertical camera following, complete authored-map camera coverage, reflections
  and final lighting remain outside the frozen stairwell charter.
- Handoff: merge REW-0010 before claiming its successor. The next bounded task
  should adopt playable camera coverage around the authored stairwell and its
  adjacent upper/ground circulation; a separate decision must move environment
  art off the project brief's non-goal list and approve imported-asset storage.
- Signature: Codex

## 2026-08-24 — REW-0009 complete, rewind is causal rather than global

- Date: 2026-08-24
- Author: Codex
- Task: REW-0009
- Branch: `codex/rew-0009-event-driven-rewind`
- Change: ADR-0009 makes event-driven rewind the default. A loop now ends by
  authored causal-contract failure, player death or successful first-time
  Anchor commit. Contract failure and commit latch a perceptible one-to-three
  second prelude before loop-start apply. Elapsed time still starts at zero and
  drives radio, patrol, turnstile and deliberately timed situations; it no
  longer ends Chapter 1 merely by reaching a global duration.
- Chapter 1 authority: `GroundFuseGate` checks ground-floor power or an
  already active `courtyard_gate_open` Anchor at the no-return threshold.
  Missing both causes the signaled rewind. Carrying the fuse down passes the
  checkpoint and enables the generator. A successful explicit gate-Anchor
  commit then ends that loop, so the next loop can keep the fuse in the
  building socket, use the lift and continue toward Transit. The second socket
  is now named by its authored location, ground floor, rather than by the
  timer-driven implementation's courtyard label.
- Acceptance: FL-02 and FL-03 are amended without renumbering; FL-01 and FL-07
  use the ground-floor power name; FL-17 and FL-18 cover the causal checkpoint,
  prelude bounds and Anchor-commit boundary. Earlier playtests remain evidence
  for the timer-driven builds and their old criteria, not for the amended
  surface.
- Owner blockout inspection: the live saved Handmade level was inspected
  without modification. It contains four large alternating stair flights and
  three owner-placed 35 mm, 16:9 CineCameraActors for 4C, the upper hallway and
  the stairwell. The map remained owner work outside this task and was not
  moved, committed or cited by path from this immutable record.
- Routed work: `docs/backlog/event-driven-rewind-implementation.md` owns the
  proposed C++ checkpoint/prelude/commit work.
  `docs/backlog/handmade-level-adoption.md` owns stabilizing and adopting the
  owner blockout while preserving its layout and camera transforms. The older
  global loop-pressure proposal is resolved by ADR-0009; its measurements
  remain historical.
- Current implementation gap: `URewindLoopSubsystem` still ends automatically
  at 240 seconds or on death, has no causal-contract or Anchor-commit reason,
  and has no latched prelude. The current build therefore does not pass amended
  FL-02 or new FL-17/FL-18.
- Verification: searched live authority and acceptance for conflicting timer
  rules and reviewed every remaining match as history, rejected alternative or
  explicit implementation gap; fifteen changed Markdown files had zero broken
  relative links and zero unclosed fences; ownership reviewed manually;
  `git diff --check` clean; diff-type audit found no C++, header, Blueprint,
  `.uasset`, `.umap`, project or config change.
- Not run: Unreal build, PIE, automation tests and packaged build. REW-0009 was
  frozen as authority-only, and the status documents explicitly do not claim
  the new rule is implemented.
- Handoff: activate the event-driven implementation proposal first. Give the
  owner blockout a stable tracked path before any immutable task cites it, then
  activate level adoption against ADR-0007 and ADR-0009.
- Signature: Codex

## 2026-08-23 — REW-0007 complete, Chapter 1 has its authored spatial chain

- Date: 2026-08-23
- Author: Codex
- Task: REW-0007
- Branch: `rew-0007/chapter-1-spatial-chain`
- Change: completed the chain that REW-0006 could not finish under its
  superseded radio timing. FiveLoops now builds 4C and its common upper
  hallway, a powered six-second cage lift, three physical switchback flights,
  an entrance hallway, one large courtyard, a 170 m service branch folded
  around its patrol yard and Transit Hub. Eleven authored camera regions cover
  that route. The player uses the imported UAL1 mannequin with idle/walk
  animation, and project PIE input gained exact game-time holds, queued input
  sequences and restricted `Rewind.*` console execution.
- Owner construction reference: nine images now live under `docs/concept/`.
  The final paired plans settle top and bottom: stairs, lift and 4C share the
  top corridor; both vertical routes meet one long entrance hallway; its
  fuse-box threshold opens into the large courtyard containing gate, generator,
  patrol area and the route to Transit. These images directed blockout
  placement and cameras without becoming game-rule authority or finished-art
  scope.
- Timing evidence: the formal stairs route reached the entrance in 55.67
  seconds and the powered lift in 6.00, a 49.67-second difference against the
  required 30. Loop B carried the only fuse to the courtyard, opened and
  committed the gate at 211.34 seconds, and timed out at 240 without entering
  Transit Hub. Loop C applied the held gate, left the fuse for the lift and
  entered Transit Hub at 90.67 seconds. The naive/learned difference is
  therefore at least 149.33 seconds, also above the required 30.
- Three-loop evidence: from `Rewind.CleanSave`, Loop A heard `7312` across the
  complete radio sequence and persisted the fact; Loop B spent the fuse on the
  generator and committed `courtyard_gate_open`; Loop C used that held change,
  the same fuse's building use and the lift to reach the hub. The durable
  record is `docs/playtests/chapter-1-three-loops-2026-08-23.md`. Diagnostic
  route-calibration loops between formal B and C are explicitly excluded.
- Build and automation evidence: Unreal Editor Win64 Development built
  successfully under UE 5.8 after the final seam fix. All six discoverable
  `Rewind.*` tests passed together, 6/6 with no errors in 0.0411 seconds. The
  camera enumeration logged all eleven regions complete; baseline logged fuse
  at rest, unpowered lift at 4C, generator offline and gate closed.
- Assets: 31 CC0 Tier 1 `.uasset` files measure 13,220,676 bytes (12.61 MiB)
  under Git LFS: 21 in-place animation assets and both pack mannequins. UAL1
  idle/walk are bound; the rest remain imported, not claimed as integrated.
- Corrections made while playing: opposing camera cuts initially inverted a
  held movement vector, so character input now latches world direction across
  a cut. Stair endpoint rails and the ground-floor seam were opened where the
  capsule route required them. Route automation was moved from MCP-call timing
  to game-time queues so network latency could not become acceptance evidence.
- Verification: successful editor build; formal clean-save A/B/C PIE run;
  measured vertical and full routes; region and baseline enumeration; six
  named automation tests; import size; links and fences reviewed manually;
  `git diff --check` clean. No packaged build was made and frame rate was not
  varied. Finished visual fidelity and enjoyment were not tested and were
  outside the frozen charter.
- Handoff: REW-0007 is archived Complete and `docs/CURRENT_TASK.md` is restored
  from the template. No successor is approved. The next implementation needs
  an owner decision between bounded visual/art-direction work against the
  concept set and a player-experience test of the proven blockout; neither is
  automatically authorized by this task.
- Signature: Codex

## 2026-08-23 — REW-0008 complete, agents can drive and test PIE

- Date: 2026-08-23
- Author: Codex
- Task: REW-0008
- Branch: `rew-0008/editor-automation`
- Change: added the editor-only `RewindEditor` module and registered
  `RewindEditor.RewindPIEInputToolset`. It reads player-zero state and sends
  press, release and tap events through Unreal's simulated player-input path,
  tracks project-injected held keys, supplies release-all, and clears tracking
  when PIE ends. `AutomationTestToolset` is enabled. `.codex/config.toml` now
  gives Codex the same localhost MCP endpoint already present in the committed
  `.mcp.json` files. `AGENTS.md` routes Unreal work to the new canonical
  `docs/EDITOR_AUTOMATION.md` instead of carrying the full machine procedure.
- Build evidence: Unreal Editor Win64 Development succeeded with UE 5.8 after
  compiling and linking `UnrealEditor-RewindEditor.dll`. No engine plugin
  source was changed.
- MCP evidence: a raw streamable-HTTP handshake on `127.0.0.1:8000/mcp`
  negotiated protocol `2025-06-18`. `list_toolsets` returned the project input
  toolset, `EditorAppToolset`, `LogsToolset` and `AutomationTestToolset`.
- PIE evidence: MCP started standard PIE and confirmed it running. Player zero
  possessed `RewindCharacter_0` at `(-150, 0, 98.15)`. Pressing `W` through the
  project toolset and then releasing it moved the pawn 337.90 cm to
  `(-150, 337.90, 98.15)`. The held-key list contained `W` while pressed and
  was empty after release; release-all reported no remaining keys. The same run
  returned a 623,908-character base64 PNG viewport payload, read 58
  `LogRewind` entries and stopped PIE cleanly.
- Automation evidence: discovery reached `ready` and listed four existing
  `Rewind.*` tests. `Rewind.Ids.MatchDesign` ran by name and passed 1/1 in
  0.00695 seconds with zero errors and zero warnings; status and result calls
  both reported the completed pass.
- Verification: successful editor build; live MCP inventory; start, state,
  input, release-all, viewport, log and stop calls; named automation-test run;
  live-document links and fences reviewed; `git diff --check` clean. No
  packaged build was made, and no REW-0007 gameplay acceptance was run. The
  Desktop app's packaged `codex.exe` could not be invoked from PowerShell due
  to WindowsApps access control, so `codex mcp list --json` was not evidence;
  the raw live handshake and tool calls were.
- Handoff: REW-0008 is archived Complete. REW-0007 is restored unchanged in
  its frozen gameplay charter at `Ready`; its tooling blocker is removed. The
  next work remains the lift and stairs, followed by the authored spatial
  chain and its task-owned playtest gates.
- Signature: Codex

## 2026-08-23 — REW-0006 superseded after the played radio rule changed

- Date: 2026-08-23
- Author: Codex
- Task: REW-0006
- Branch: `rew-0006/radio-sequence`
- Change: REW-0006 is archived as `Superseded` by REW-0007. Its frozen scope
  required a 45-second radio sequence repeating every 60 seconds. The played
  implementation exposed that rule as punitive, the design and code moved to
  20 seconds repeating every 50, and the owner confirmed that 20/50 is the
  rule to keep. The task is superseded rather than silently rewriting frozen
  scope. The same change claims REW-0007 and leaves its successor charter in
  `Draft` until that claim lands on `main`.
- Delivered before supersession: a four-channel radio driven by elapsed loop
  time, digits at phases 4, 9, 14 and 19, individual digits kept only in the
  player's memory, and `radio_code_7312` granted after a complete sequence;
  one carried LoopWorld fuse; two exclusive sockets; the generator requiring
  the courtyard socket; the 240-second loop duration authored in settings;
  and the fuse-box labelling decision. The radio work is commits `ec0d2ac`,
  `cb70695`, `d7610af`, `bf09036` and `fd0e3e6`; the fuse work is `4f61bc0`.
- Evidence already present in commit history: `cb70695` records PIE observing
  the 240-second duration, broadcast boundaries, fuse Baseline, both sockets
  and the 4C camera. `bf09036` records a played path from fuse pickup through
  gate Anchor and hub entry and the late-listener radio defect it fixed. No
  durable playtest artifact was added, so those observations are not evidence
  for the amended FL criteria.
- Verification for this supersession wave: authoritative documents and
  relative links reviewed manually; fenced blocks reviewed; `git diff --check`
  clean. No Unreal build, PIE run or new playtest was performed in this wave.
- Not completed: lift, stairs, landing and shaft, spatial rebuild, complete
  camera-region enumeration, animation import, timing measurements and the
  three-loop acceptance run. The existing blockout can still reach the hub
  without the intended lift-or-stairs consequence.
- Handoff: merge this branch so the REW-0007 identity claim lands on `main`.
  Then branch from updated `main`, move REW-0007 from `Draft` to `Ready`, and
  implement the lift and stairs as the first unchecked work. No gameplay
  decision blocks that implementation.
- Signature: Codex

## 2026-08-23 — REW-0005, what an Anchor is worth and the Chapter 1 chain

- Date: 2026-08-23
- Author: Claude
- Task: REW-0005
- Branch: `rew-0005/anchor-consequence`
- Change: rules only, no implementation. `ADR-0008` states why an Anchor
  exists: holding an override must release something the loop would
  otherwise commit, and that thing must have another use. Name the
  resource, name its two uses, or it is a shortcut. Scarcity becomes a
  design rule with its own argument rather than a consequence of the first
  proof's scope. `anchors.md` restates both. `chapter-1-authored.md` gains
  the chain: a radio that broadcasts on the loop clock and costs 45 seconds
  of a 240-second loop, one carried fuse with two exclusive sockets, a lift
  that runs only while the fuse is in the building socket, stairs that cost
  at least one turnstile period more, and the three loops stated as shapes
  the space must support.
- Verification: every new rule checked against ADR-0002, and the only
  loop-count mentions in the design documents are the ones forbidding it.
  Ownership checked by search: the numbers live in one document each.
  Manual link and fence review. `git diff --check` clean. Nothing was built
  and nothing was played, so nothing here is evidence about a build.
- Finding that changed a decision already made: `courtyard_gate_open` does
  not pass ADR-0008's test as the chain is implemented today. Holding the
  gate removes two steps and leaves every route available, only faster, so
  there is no contested resource to name. It passes under the chain this
  task wrote. That is a design gap rather than a defect in ADR-0003 or in
  the implementation, and it is what the Five Loops playtest measured
  without being able to say so.
- Second finding: FL-13 became unsatisfiable. It required the whole chain
  inside one loop, and the chain now makes that impossible on purpose,
  because carrying the fuse to the courtyard kills the lift. It was amended
  with its identifier intact, along with FL-07's wording, and the
  amendments are recorded in the acceptance document. The existing playtest
  evidence stays true of the criteria as they read before, and is not
  evidence for these.
- Did not do: any implementation. No C++, no geometry, no assets. The rules
  are deliberately ahead of the build, and `docs/CURRENT_STATUS.md` says so.
- Handoff: REW-0004 is restored to `docs/CURRENT_TASK.md` from
  `docs/paused/`, its resume condition met. **Its frozen out-of-scope list
  now conflicts with what this task decided**: it excludes branching routes,
  and the lift-or-stairs choice is one. A frozen charter is superseded
  rather than rewritten, so the next actor should decide whether to
  supersede REW-0004 before resuming it. That is recorded in its handoff and
  is not resolved here.
- Signature: Claude

## 2026-08-23 — REW-0003 complete, all sixteen FL criteria evidenced

- Date: 2026-08-23
- Author: Claude
- Task: REW-0003, re-assigned from Grok on 2026-08-23
- Branch: `rew-0003/run-evidence`
- Change: the Five Loops Test is demonstrated. FL-01 to FL-16 each have
  named evidence in `docs/playtests/five-loops-2026-08-22.md`, a new
  collection created because `docs/acceptance/` owns criteria and mixing
  evidence into it would break that ownership. Four defects were fixed
  first: exposure was never authored against a 50000 lux sun; the
  outdoor run was three slabs in a void with 20 cm and 30 cm gaps at the
  two doorways; the patrol teleported between two poses behind a barrier
  covering 200 cm of a 760 cm corridor; and the courtyard gate and the
  turnstile did not span the corridor either, at 400 cm and 450 cm. A
  `LogRewind` run log was added so evidence is read from stamped lines
  rather than transcribed from screenshots, and `.mcp.json` points at
  the editor's MCP endpoint, which drove the idle-pair run.
- Verification: four runs, all on editor Win64 Development builds, with
  raw logs committed beside the record. A two-loop idle run with no
  player input; a played run from a clean session; a scripted sequence
  covering clean save and a baseline against two learned runs; and a
  full editor quit and reopen. `git diff --check` clean, links and
  fences reviewed by hand.
- Finding that matters more than the pass: FL-14 passes on its wording
  and its number is close to meaningless. Every turnstile crossing in
  every run landed between `t = 30.24` and `t = 30.93`, and the
  turnstile opens at `t = 30`. The player arrived early and waited every
  time, including on the baseline run that did the whole chain. One
  learned run finished at `t = 8.37` and stood at the turnstile for
  twenty-two seconds. The 0.69 s difference measures the gate cycle, not
  the player. The loop has no time pressure, and the record says so
  plainly.
- Second finding: three criteria were undemonstrable before this wave
  and nobody knew, because a closed gate and a closed turnstile could
  both be walked around. Any earlier impression of reaching the hub
  through the turnstile is void. It was caught by measuring geometry
  rather than by playing, which is what "Evidence Discipline" is for.
- Did not run: a packaged build; frame rate was not varied, so the
  clock's independence from frame rate is evidence at one rate only;
  FL-03, FL-08 and FL-12 hold to tick resolution of about 0.25 s rather
  than to exact equality at an arbitrary `t`, because transitions are
  logged on the tick that observes them. The offsets are constant inside
  each loop, which is tick phase and not drift.
- Routed, not built: three proposals in `docs/backlog/` — art direction,
  loop pressure and interaction cost, and traversal as a knowledge axis.
  None is authority. The third argues the proof can pass FL-14 without
  representing spatial knowledge, and the FL-14 result above is that
  argument arriving as data.
- Handoff: `docs/CURRENT_TASK.md` is restored from the template and no
  task is active. The central product question is now open in a way it
  was not before: the loop is proven deterministic and knowledge does
  persist, and the same evidence shows the space is too small for the
  timer to mean anything. What follows is a product decision, not a bug
  fix. The branch is not merged to `main`.
- Signature: Claude

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
