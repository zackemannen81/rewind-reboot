# Current Task

Task ID: REW-0016
Parent Task: None
Status: Complete
Owner: Claude
Created: 2026-08-25
Last updated: 2026-08-25
Charter frozen at: 2026-08-25

## Read First

- `AGENTS.md`
- `docs/TASK_WORKFLOW.md`
- `docs/PROJECT_BRIEF.md`
- `docs/CONTRIBUTING.md`
- `docs/CURRENT_STATUS.md`
- `docs/SYSTEMDOC.md`
- `docs/JOURNAL.md`
- `docs/EDITOR_AUTOMATION.md`

## Task Summary

The owner asked for the remaining parts of Chapter 1 to be finished, and for
that work to be run through delegated coding agents rather than done in one
session. This task is the operator role: it charters the child tasks, claims
their identities, briefs and launches the agents, monitors them, reviews their
pull requests and merges them.

The delegation mechanism is the owner's `powershell-agent-mcp` stdio server,
driven through a localhost bridge because a stdio MCP server cannot be loaded
into an already-running session. Each agent works in its own repository clone
under `C:\code\rewind-multiagents`, so no two agents share a working tree.

This task does not implement Chapter 1 content itself. Its deliverable is the
coordination and the merged result.

## Task Charter

The charter is editable while status is `Draft` and immutable once status is
`Ready`.

### Goal

Land the three chartered Chapter 1 child tasks on `main` through delegated
agents, each with its own frozen charter, branch, pull request and journal
entry.

### Primary Deliverable

REW-0017, REW-0018 and REW-0019 merged to `main`, each archived under
`docs/finished/` with a signed `docs/JOURNAL.md` entry and named verification
evidence.

### In Scope

- Claim REW-0016 to REW-0019 on `main` before their charters freeze.
- Author each child charter and freeze it at `Ready`; agents transcribe, never
  redefine.
- Assign one clone per agent and one file-ownership boundary per task.
- Launch, monitor and steer agents through the `powershell-agent-mcp` tools.
- Review each pull request against its charter's definition of done.
- Merge, or send the agent back with named defects.
- Keep `docs/TASK_IDS.md` under operator ownership so agents cannot conflict on
  it.

### Out of Scope

- Writing the child tasks' implementation code. If an agent cannot finish, the
  operator reports that, and does not silently absorb the work.
- Deciding Echo semantics, the asset-provenance question, or audio and mesh
  generation. Those need owner decisions and are not chartered here.
- Changing any accepted rule in `docs/design/` or `docs/adr/`.
- Blockout of the unbuilt last part of the level, which has no owner-approved
  plan yet.

### Definition of Done

- REW-0017, REW-0018 and REW-0019 are merged to `main`.
- Each is archived under `docs/finished/` and carries a journal entry naming
  what was verified and what was not.
- `docs/CURRENT_TASK.md` on `main` is the restored template.
- No agent branch is left unmerged without a stated reason.

### Minimum Verification Gates

- [ ] Every merged child names an editor-closed build result.
- [ ] Every merged child names an automation-test result, or states why it
      could not run one.
- [ ] Every merged pull request is reviewed against its own definition of done
      before merge, not on the agent's assertion that it is finished.

## References

- `C:\code\powershell-agent-mcp` — the delegation server under test
- `C:\code\rewind-multiagents\rewind-reboot{,2,3}` — one clone per agent
- `docs/backlog/art-direction.md`
- `docs/backlog/event-driven-rewind-implementation.md`
- `docs/adr/ADR-0009_event-driven-loop-termination.md`

## Checklist

- [x] Merge REW-0015 to `main`.
- [x] Bridge the stdio agent server to localhost HTTP.
- [x] Claim REW-0016 to REW-0019 on `main`.
- [x] Sync the three clones to `main`.
- [x] Brief and launch the three agents.
- [x] Monitor and steer. One correction was needed: two agents were running
      brief-0018 in the same clone, and the duplicate was halted through the
      orchestrator's own `halt-agent`.
- [x] Review, merge and verify each child's archive and journal entry.
- [x] Report to the owner with what landed and what did not.

## Decisions and Notes

- A checkpoint after each step or substep is required.
- **Exception, recorded while editable:** as with REW-0015, the identity claims
  were pushed to `main` immediately before this charter froze rather than
  through a separate merged pull request. The register rows are appended and
  unsorted, so its conflict property holds.
- File ownership is assigned per task so two agents cannot edit the same file.
  Only REW-0019 may modify `.umap` or `.uasset` binaries, because those cannot
  be merged. REW-0018 may not touch `RewindLoopSubsystem.cpp`, which REW-0017
  owns.
- Agents do not edit `docs/TASK_IDS.md`. The operator claimed their identities
  in advance for exactly that reason.
- The Unreal editor is operator-owned. Agents verify with editor-closed builds
  and headless automation runs, never by launching the interactive editor,
  because only one editor can hold the project's module lock and MCP port.

## Charter Amendment Log

Only non-semantic corrections are allowed after `Ready`.

-none

## Verification

- [x] Each child pull request read against its charter before merge. Claims
      were checked against the tree rather than taken from the journal entry:
      named tests confirmed present in source, `bUseWholeSpaceDeadline`
      confirmed `false`, `AddOnScreenDebugMessage` confirmed absent from the
      migrated actors, and every archive and restored template confirmed.
- [x] `main` carries three new archives and three journal entries.
- [x] Editor-closed build of the merged tree succeeded in 23.12 s.
- [x] All seventeen `Rewind.*` tests run together on the fully merged tree:
      17 passed, 0 failed, `TEST COMPLETE. EXIT CODE: 0`.
- [ ] Not run: no PIE, no packaged build, no playtest of the merged result.
      This task merged and verified other tasks' work; it did not re-play the
      game. Each child's own unverified list stands.

## Documentation Updates

- [x] `docs/CURRENT_STATUS.md`
- [x] `docs/JOURNAL.md`
- [ ] `docs/SYSTEMDOC.md` — the three children described their own systems; the
      operator added nothing to describe
- [ ] `docs/FILESTRUCTURE.md` — no structural change of this task's own
- [ ] ADRs — ADR-0010 belongs to REW-0019

## Handoff and Follow-ups

- Current state: complete. REW-0017, REW-0018 and REW-0019 are merged,
  archived and journalled. Seventeen tests pass together on the merged tree.
- Operator defect worth recording: `RewindAnchorBoard.*` was granted to both
  REW-0017 and REW-0018. The file-ownership split existed to make that
  impossible and the split was written wrong. Both branches edited it and the
  merge was resolved by hand; taking either side alone would have shipped a
  three-placeholder format string with two arguments.
- Second operator defect: two agents ran brief-0018 in the same clone for
  about seven minutes, because the operator launched one while the owner
  launched another. Halted through `halt-agent`; no work was lost, and the
  surviving branch's file set was checked for orphans before merge.
- Delegation mechanism: the owner's `powershell-agent-mcp` worked. A stdio MCP
  server cannot be loaded into a running session, so it was driven through a
  localhost bridge holding one long-lived server process.
- Next recommended step: an owner playtest of the merged build. Three things
  changed that only play can judge — the loop now ends on causal-contract
  failure rather than a timer, text comes from a message overlay that has
  never been seen against the authored camera, and the renderer changed.
- Blockers: none.
- Child tasks: REW-0017, REW-0018, REW-0019, all complete.
- Resume condition: not paused.
- Open questions: unchanged and now recorded as backlog proposals — Echo
  semantics, the untracked Fab and `Art/Texture` dependency, and the remaining
  Chapter 1 content.

## Finalize When Complete

- Archive this file under `docs/finished/`.
- Restore this template or populate the next approved task.
- Add a signed `docs/JOURNAL.md` entry.
- If Goal or Definition of Done changed, supersede this task instead of
  rewriting it.
