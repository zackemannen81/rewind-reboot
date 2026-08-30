# Delegation

Status: Accepted
Date: 2026-08-27
Owns: how a delegated worker wave is set up and run **in this repository** —
local wiring, launch commands, locks and the failure modes learned here
Does not own: task states and scope freeze, which are
[`TASK_WORKFLOW.md`](TASK_WORKFLOW.md)'s; the editor, MCP endpoint, build and
PIE procedure, which are [`EDITOR_AUTOMATION.md`](EDITOR_AUTOMATION.md)'s; wave
plan naming, which is [`waves/README.md`](waves/README.md)'s; the generic
protocol and add-on procedure, which live in the add-on's own tutorial

The generic method is documented upstream at
`C:\code\docs-first-multiagent-orchestrator-addon\docs\TUTORIAL.md`. This
document is the local record: what is actually installed on this machine, what
the commands really are, and what has actually gone wrong.

## Part 1 — The docs-first surface

Already done here, and not repeatable as a checklist because it was a
bootstrap, not a procedure. What exists:

- `AGENTS.md` as the single entry point, naming a read order.
- One owner per kind of truth under `docs/`: active task, workflow, brief,
  status, system, journal, file map, identity register, roadmap, decisions,
  design, acceptance, archives.
- `docs/baseline/acme-2026-08-19/` as frozen provenance, never edited, never
  authority.

For a new project, run the add-on's questionnaire rather than copying this
tree:

```powershell
Set-ExecutionPolicy -Scope Process Bypass
& C:\code\docs-first-multiagent-orchestrator-addon\scripts\New-DocsFirstBootstrap.ps1
```

The one step that cannot be skipped is rewriting every live role for the new
project. A tree carrying another project's status, brief and charter is not a
docs-first instance; it is a copy.

## Part 2 — The machinery on this machine

| Thing | Where |
| --- | --- |
| Orchestrator add-on | `C:\code\docs-first-multiagent-orchestrator-addon` |
| MCP server entry | `server\dist\src\index.js`, launched with `node` |
| Registered for Codex | `.codex/config.toml`, as `docs_first_agents` |
| Unreal editor MCP | `.mcp.json` and `.codex/config.toml`, HTTP `127.0.0.1:8000/mcp` |
| Worker clones | `C:\code\rewind-multiagents\rewind-reboot`, `…2`, `…3` |
| Briefs | `C:\code\rewind-multiagents\briefs\` |
| Quarantine | `C:\code\rewind-multiagents\quarantine\` |

The server exposes exactly five tools: `run-agent`, `run-agent-task`,
`about-agent`, `interrupt-agent`, `halt-agent`.

Three things about this wiring are not obvious:

- **The agent server is stdio.** Codex loads it from `.codex/config.toml`, but
  a session that is already running cannot pick up a config change — start a
  fresh one. A client that cannot spawn stdio servers needs a small Node bridge
  that keeps **one** long-lived server process on localhost HTTP. One process
  matters: the agent registry lives in memory, so a fresh spawn per call loses
  every agent it was tracking.
- **The Unreal MCP endpoint exists only while this project's editor is open.**
  Port 8000 closed is the normal state with the editor shut. Editor autostart
  is not Windows autostart.
- **The server runs PowerShell as you.** Decide the approval policy before
  launching an unattended worker, and never reword a command to get past a
  permission prompt.

Workers run Codex, not Grok, and not a frontier model:

```powershell
Get-Content C:\code\rewind-multiagents\briefs\brief-0042.md -Raw |
  & "$env:APPDATA\npm\codex.cmd" exec -m gpt-5.6-terra `
    -C C:\code\rewind-multiagents\rewind-reboot3 `
    --dangerously-bypass-approvals-and-sandbox
```

`codex exec` reads its prompt from **stdin**, not from an argument. The model
id is hyphenated; `gpt-5.6terra` is rejected.

**Two `grok` binaries exist and they are different programs.**
`C:\Users\zakri\.grok\bin\grok.exe` is the real CLI and supports
`--prompt-file`; `%APPDATA%\npm\grok` is an unrelated Node package that answers
`error: unknown option '--prompt-file'`. Which one wins depends on PATH order.
Always invoke by absolute path.

## Part 3 — Running a wave

1. **Charter one operator task.** It owns scope, task IDs, merge and the
   canonical gates, and it does not implement. Freeze it at `Ready`.
2. **Claim every child ID in `docs/TASK_IDS.md` and merge that claim to `main`
   before freezing.** A local count cannot see another branch.
3. **Prepare one physical clone per concurrent worker**, at the claimed base
   commit, verified clean. One agent per clone, always. Two agents in one clone
   corrupt each other's branch, and the server refuses a second live process in
   the same `cwd` unless `allow_shared_cwd` is explicitly true.
4. **Freeze a wave plan** under `docs/waves/` as `{TASK-ID}_task-slug.md`:
   allocation table, exclusive worker paths, global locks, checkpoints, merge
   order, recovery.
5. **Write one brief per worker**, at an operator-owned path outside the worker
   clones. It carries the frozen charter **verbatim** — a brief that asks the
   worker to design its own task has already lost the scope freeze. It also
   names the clone, the branch, read-first authority, owned and forbidden
   paths, build and test commands, which documents the worker may touch, PR
   behaviour, what to do when blocked, and a requirement to push the first
   meaningful checkpoint.
6. **Take the locks.** Close the canonical editor before launch. The worker
   owns the Unreal process lease until every editor and commandlet it started
   has stopped.
7. **Smoke-test the process plane** with a harmless task, and confirm it
   launched the CLI you meant.
8. **Launch asynchronously** with `run-agent-task`, a stable `agent_id` and the
   worker clone as `cwd`. Do not wait.
9. **Monitor by cursor** (Part 5), and verify claims against git.
10. **Steer only inside the frozen charter.** `interrupt-agent` may ask for
    evidence or ordering. It may not change goal, scope, definition of done or
    gates. That needs re-chartering through the repository.
11. **Review the whole diff** against the charter, merge, then rerun the
    combined gates on the merged tree — build with the editor closed, the full
    `Rewind.*` suite, standard PIE.
12. **Finish**: archive under `docs/finished/`, add a signed journal entry,
    restore `docs/CURRENT_TASK.md` from the template.

## Part 4 — Locks and the failures that produced them

Every rule here exists because of a specific incident.

- **One Unreal editor across all clones.** Live Coding is global to the shared
  engine install, so an editor open on any clone blocks `Build.bat` for every
  other clone. Workers verify with editor-closed builds and headless
  `UnrealEditor-Cmd … -ExecCmds='Automation RunTests Rewind;Quit' -unattended
  -nopause -nosplash -NullRHI`.
- **A running editor locks `.uasset` files against git**, which surfaces as
  `unable to unlink … Invalid argument` on pull. Close it before any git
  operation touching tracked content.
- **Never run `git add -A` in this repository.** On 2026-08-25 it swept 71
  files into a commit and pushed 358 MB of LFS objects before it was caught.
  The branch was rebuilt and deleted; the objects remain unreferenced on the
  remote and still count against quota. Stage explicit paths.
- **Give workers non-overlapping file ownership, documents included.** Three
  workers appending to `JOURNAL.md`, `CURRENT_STATUS.md` and
  `FILESTRUCTURE.md` made every merge conflict. Each worker owns only its own
  journal entry; the shared status files stay with the operator.
- **A binary `.umap` cannot be merged.** One worker owns the map for a wave, or
  the wave loses work. This is why REW-0038 used a single implementation child
  instead of parallel art workers.
- **Repository state is recovery authority, not MCP process state.** Every
  worker step ends in a pushed checkpoint. After a crash, inspect the worker
  branch and resume from its last push — never replay committed mutations.
- **A worker that cannot get a lock should stop, not improvise.** REW-0039
  reached checkpoint 1, found the canonical editor holding the Unreal lease,
  and exited without touching the map or spending credits. That is the correct
  outcome, and it is only possible because the brief told it what to do when
  blocked.

## Part 5 — Monitoring, and what "watchers" actually are

**There is no watcher primitive.** Nothing pushes worker events to the
operator. Monitoring is two things:

**Cursor-based polling.** `about-agent` with an `agent_id`, `after_cursor` and
`max_chars` returns buffered output plus a `nextCursor`. Save it and pass it as
the next `after_cursor` so each read returns only what is new. Called with no
`agent_id`, it lists every agent the server is tracking.

Poll on a signal, not on a timer. A worker in a long compile is not stuck, and
frequent polling floods the operator's context with output that changes
nothing — which is the actual cost, since the operator's context is the scarce
resource in a wave.

**Durable checkpoints, which are the real mechanism.** Worker output is a
progress signal and nothing more. What a claim is verified against is a commit,
a passing named gate, a PR, or a recorded artifact. The wave plan requires a
pushed checkpoint before each risky step precisely so that the operator never
has to trust the process plane, and so a crashed wave is resumable from `git`
alone.

If you want something closer to a watcher, the honest version is a scheduled
operator pass that reads `git log` on each worker branch rather than the
agent's stdout. The repository is the surface that survives.
