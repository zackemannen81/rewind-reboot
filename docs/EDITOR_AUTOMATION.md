# Editor Automation

Status: Canonical operating context for the owner workstation and for agents
working with the RE:WIND Unreal Editor.

This document owns how an agent discovers, builds, launches and drives this
project in the editor. It does not own gameplay rules or acceptance criteria.
Those remain in `docs/design/` and `docs/acceptance/`.

## Installed environment

| Item | Current value |
| --- | --- |
| Engine | Unreal Engine 5.8 |
| Owner-workstation editor | `C:\Program Files\Epic Games\UE_5.8\Engine\Binaries\Win64\UnrealEditor.exe` |
| Project | `C:\code\rewind-reboot\Rewind\Rewind.uproject` |
| MCP transport | Streamable HTTP at `http://127.0.0.1:8000/mcp` |
| Server lifetime | Starts inside this editor when the project plugin loads; stops with the editor |
| Server settings | `Rewind/Config/DefaultEditor.ini` |
| Codex project config | `.codex/config.toml` |
| Other MCP-client configs | `.mcp.json` at the repo root and `Rewind/.mcp.json` |

`bAutoStartServer=True` means editor-plugin autostart. It does not mean a
Windows service or boot task. Port 8000 is therefore expected to be closed
while Unreal Editor is closed.

The committed Codex configuration is project-scoped. Start the task with this
repository as its primary/root folder. A task created outside the repo can read
these files later but may already have frozen its MCP tool inventory; restart
such a task from the repo root after the editor is running.

## Enabled editor capabilities

`Rewind/Rewind.uproject` enables these relevant engine plugins:

| Plugin | Role in this project |
| --- | --- |
| `ModelContextProtocol` | Hosts the MCP server inside Unreal Editor |
| `EditorToolset` | Editor state, viewport capture, actor inspection, logs and PIE start/stop |
| `AutomationTestToolset` | Discovers, runs, monitors and reports named Unreal automation tests |
| `MCPClientToolset` | Lets Unreal/toolset clients connect outward to other MCP servers; it is not the PIE input bridge |
| `Terminal` | Native editor terminal UI; enabled, but not itself an MCP toolset |
| `ModelingToolsEditorMode` | Editor modeling capability; unrelated to agent input |

The project-owned editor-only module `RewindEditor` registers
`RewindEditor.RewindPIEInputToolset`. It supplies:

- `GetPlayerState`
- `PressKey`
- `ReleaseKey`
- `TapKey`
- `ReleaseAllKeys`

The server uses tool search, so its top-level MCP surface is intentionally
small: `list_toolsets`, `describe_toolset` and `call_tool`. Use those to find
and invoke the registered engine and project toolsets. The high-value engine
toolsets for playtests are `EditorToolset.EditorAppToolset`,
`EditorToolset.LogsToolset` and
`AutomationTestToolset.AutomationTestToolset`.

## Build and launch

Close Unreal Editor before a full C++ build. The editor owns the module DLLs
and its MCP endpoint, so build and live editor control are separate phases.

From PowerShell at the repository root:

```powershell
& 'C:\Program Files\Epic Games\UE_5.8\Engine\Build\BatchFiles\Build.bat' `
  RewindEditor Win64 Development `
  '-Project=C:\code\rewind-reboot\Rewind\Rewind.uproject' `
  -WaitMutex -FromMsBuild
```

Then launch the interactive editor:

```powershell
Start-Process `
  -FilePath 'C:\Program Files\Epic Games\UE_5.8\Engine\Binaries\Win64\UnrealEditor.exe' `
  -ArgumentList 'C:\code\rewind-reboot\Rewind\Rewind.uproject'
```

Wait for the project to finish loading and verify the listener:

```powershell
Test-NetConnection 127.0.0.1 -Port 8000 -InformationLevel Quiet
```

For Codex, confirm that the `unreal` MCP entry is configured with
`codex mcp list --json`, or inspect the current session's MCP tools. Configured
does not mean connected: the port check and a successful `list_toolsets` call
are the readiness evidence.

## Agent playtest sequence

1. Confirm port 8000 and call `list_toolsets`.
2. Confirm the three high-value engine toolsets and
   `RewindEditor.RewindPIEInputToolset` are present.
3. Call `EditorToolset.EditorAppToolset.StartPIE` with standard PIE
   (`bSimulate=false`) and a warmup. Simulate mode has no possessed player.
4. Call `GetPlayerState` and record the pawn and starting transform.
5. Use Unreal key names such as `W`, `A`, `S`, `D`, `E`, `SpaceBar`, `Zero`,
   `One` through `Nine`. Use `PressKey`, allow the world to tick, then use
   `ReleaseKey`; use `TapKey` for actions that only need a pressed edge.
6. Read `GetPlayerState` again and collect the task's required state or
   `LogRewind` evidence through `EditorToolset.LogsToolset`.
7. Capture the PIE viewport when visual evidence materially helps the gate.
8. In a finally-style cleanup, call `ReleaseAllKeys` before
   `EditorToolset.EditorAppToolset.StopPIE`.

Never leave a movement key held across unrelated tool calls or a handoff. PIE
termination clears this module's tracking, but explicit release is still the
only valid clean completion of an input sequence.

## Repeatable acceptance

Input-driven exploration and named automation tests serve different purposes:

| Need | Mechanism | Evidence |
| --- | --- | --- |
| Explore or reproduce a player path | PIE input toolset | Before/after state, viewport, relevant logs |
| Assert a deterministic rule repeatedly | Unreal automation or functional test | Named test, result, duration, errors and warnings |
| Inspect editor/world state | Existing editor, actor and object toolsets | Returned structured state |
| Rebuild C++ | Editor closed, `Build.bat` | Successful target build |

Use `AutomationTestToolset.DiscoverTests` once per editor session, then
`ListTests`, `RunTests` or `RunTestsByFilter`, `GetTestStatus` and
`GetTestResults`. A gameplay task owns the tests and assertions required by its
acceptance criteria; this document owns only the route for running them.

## Failure modes

- **Port closed:** the editor is not running, the wrong project is open, or the
  MCP plugin failed to load. Check the editor Output Log before changing config.
- **MCP configured but no tools in the task:** the agent session started before
  the server or outside the repo. Start a new task from the repo root with the
  editor running.
- **PIE exists but input reports no pawn:** standard PIE has not finished its
  warmup, or Simulate was started. Stop it and start standard PIE.
- **Build cannot replace a module:** close Unreal Editor, confirm no
  `UnrealEditor.exe` remains for this project, then rebuild.
- **Movement did not occur:** compare `GetPlayerState` before and after, verify
  the key was released, and inspect the current camera region or collision.
  Do not substitute desktop key injection.
