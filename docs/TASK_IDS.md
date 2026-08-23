# Task ID Register

Floor: DFC-0001

This register allocates task identities. It records that a number is taken and
by whom. It never records whether work is in progress, complete or abandoned:
task state belongs to `docs/CURRENT_TASK.md` and `docs/finished/`, and a second
statement about it here would drift from both and would make the trunk claim how
much work is active across the repository.

The prefix in force is `REW`. `DFC-0001` was claimed when this repository was
still the protocol extraction; that identity stays taken. New claims use `REW`.

## How to claim

1. The next free identity is one above the highest of this register and
   `docs/finished/` that shares the prefix in force. `DFC-0001` does not
   consume `REW-0001`.
2. Append one row at the end of the table below. Never insert into the middle,
   never sort, never group by owner. The append point is the safety mechanism:
   two people claiming at the same moment edit the same region and the second
   gets a merge conflict instead of a silent duplicate.
3. Merge the claim to `main`. The identity is not yours until that lands.
4. Only then move the charter to `Ready`.

## Claims

| Task ID | Title | Owner | Claimed | Work |
| --- | --- | --- | --- | --- |
| DFC-0001 | protocol specification, C-01 to C-18 | unassigned | 2026-08-19 | docs/finished/DFC-0001_protocol-specification.md |
| REW-0001 | establish canonical project state | Grok | 2026-08-19 | docs/finished/REW-0001_establish-canonical-project-state.md |
| REW-0002 | Five Loops Test authority | Grok | 2026-08-19 | docs/finished/REW-0002_five-loops-test-authority.md |
| REW-0003 | Five Loops Test implementation | Grok, then Claude | 2026-08-19 | docs/finished/REW-0003_five-loops-test-implementation.md |
| REW-0004 | Chapter 1 spatial and camera foundation | Claude | 2026-08-23 | docs/finished/REW-0004_chapter-1-spatial-and-camera-foundation.md |
| REW-0005 | Rewind consequence and the Chapter 1 chain | Claude | 2026-08-23 | docs/finished/REW-0005_rewind-consequence-and-chapter-1-chain.md |
| REW-0006 | Chapter 1 space, the chain made playable | Claude | 2026-08-23 | docs/finished/REW-0006_chapter-1-space-the-chain-made-playable.md |
| REW-0007 | Complete the Chapter 1 spatial chain | Codex | 2026-08-23 | docs/CURRENT_TASK.md |
| REW-0008 | Agent-driven PIE and editor automation | Codex | 2026-08-23 | docs/finished/REW-0008_agent-driven-pie-and-editor-automation.md |
| REW-0009 | Event-driven rewind authority | Codex | 2026-08-24 | docs/finished/REW-0009_event-driven-rewind-authority.md |
| REW-0010 | Owner-scale Chapter 1 stairwell blockout | Codex | 2026-08-24 | docs/CURRENT_TASK.md |
