# Event-driven rewind implementation

Status: Proposed
Discovered: 2026-08-24, during REW-0009
Owner: unassigned
Activated: not activated. No REW identity is claimed for this.

ADR-0009 and the owning design documents now require causal checkpoints,
successful Anchor commit and death to end loops by default. The running build
still ends automatically at 240 seconds or on death. This proposal routes that
implementation gap outside the authority-only REW-0009 charter.

## Proposed outcome

- Replace Timer as Chapter 1's automatic end reason with explicit
  CausalContract and AnchorCommit paths while leaving elapsed time running for
  radio, patrol and turnstile phase.
- Add a single latched rewind request that records checkpoint, reason, request
  time and an authored prelude duration between one and three seconds.
- Implement GroundFuseGate with the accepted predicate:

```text
GROUND_FUSE_POWERED || ANCHOR_courtyard_gate_open_ACTIVE
```

- Make a successful first-time courtyard_gate_open commit write the Anchor,
  begin the prelude and start the next loop. Rejected and redundant commits do
  not rewind.
- Present a mechanically legible prelude without claiming final sound or visual
  art.
- Rename the second socket in player-facing and design-facing surfaces from
  courtyard to ground floor, with any code migration kept explicit.
- Replace timer-budget tests with named contract, prelude and commit-boundary
  tests.

## Why this is outside REW-0009

REW-0009 is frozen as a decision and authority task. It explicitly excludes
C++, Blueprint, map, camera and blockout changes, and Unreal verification.

## Dependencies

- ADR-0009 Accepted.
- loop-and-determinism.md, anchors.md and chapter-1-authored.md as amended by
  REW-0009.
- A stable choice of implementation map before placing checkpoint volumes.

## Suggested verification

- Named automation tests for pass/fail predicates, prelude lower and upper
  bounds, one latched request, no 240-second automatic reset, successful commit,
  rejected commit and redundant commit.
- Agent-driven PIE from CleanSave covering FL-02, FL-03, FL-07, FL-17 and
  FL-18.
- A long idle PIE observation beyond 240 seconds showing that radio, patrol and
  turnstile phase continue and the loop does not end.
- Explicit release of every held PIE key before stopping.

## Not proposed

Final static, sound, post-process or animation; Echo; a general-purpose time
engine; or adopting the owner-authored level geometry. Those remain separate
scope.
