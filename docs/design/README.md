# Design Authority

Discoverability: index. Every member of this directory is listed below.
Member state: required. Every member declares a `Status:` line under its title
and the area of the game it owns.

Game rules live here, one ownership area per document. Documents are created
when a task needs them, never in advance to make the structure look complete.
An empty index is an honest statement that no game rule has been decided yet.

If two documents would define the same rule, one of them is wrong. Fix the
ownership; do not duplicate the rule. A rule that is not in a design document
is not a rule. It is a proposal.

## Documents

| Document | Status | Owns |
| --- | --- | --- |
| [`loop-and-determinism.md`](loop-and-determinism.md) | Accepted | Loop semantics and world determinism |
| [`anchors.md`](anchors.md) | Accepted | Anchor meaning, commit, cost, first-proof override |
| [`world-state-model.md`](world-state-model.md) | Accepted | State buckets and loop-start apply order |
| [`save-and-session.md`](save-and-session.md) | Accepted | Persistence, session load, clean save, knowledge facts |
| [`chapter-1-authored.md`](chapter-1-authored.md) | Accepted | Authored space, timings and puzzle facts for the first proof |
| [`camera-and-movement.md`](camera-and-movement.md) | Accepted | How the world is framed, and the volume the player moves in |
| [`stairwell-visual-direction.md`](stairwell-visual-direction.md) | Accepted | First-pass visual grammar for the authored stairwell slice |
| [`player-messages.md`](player-messages.md) | Accepted | When the game speaks in text, how long a line stays, and that the debug overlay is not UI |
| [`tutorial-and-first-run.md`](tutorial-and-first-run.md) | Accepted | First-run copy, what it may say, and first-time gating |
| [`chapter-2-authored.md`](chapter-2-authored.md) | Accepted | Chapter 2 fiction, spaces, clocks, puzzles, antagonist, false information, entry and exit |
| [`echo.md`](echo.md) | Proposed | What an Echo is, what it may and may not do, how it is staged, and the order its meaning is revealed |
| [`the-macro-loop.md`](the-macro-loop.md) | Proposed | The macro-cycle frame, what of it is fiction and what is mechanism, and the position on determinism and freedom |
| [`planted-details.md`](planted-details.md) | Proposed | The register of details authored early because they pay off later, and the rule that protects them |
