# ADR-0001 — Engine: Unreal Engine 5

Status: Accepted
Date: 2026-08-19
Deciders: MrWhite
Supersedes: nothing
Superseded by: nothing

## Context

RE:WIND's previous implementation was built in Unity, in C#, across roughly
thirty scripts covering a player controller, Chapter 1 interactions, patrols and
three manager singletons. The reboot has to state an engine before any task can
produce anything runnable.

Three facts framed the decision.

**Unity is not installed on the development machine.** Unreal Engine 5.8 is,
at `C:\Program Files\Epic Games\UE_5.8`. Continuing in Unity means installing
and configuring a toolchain that is currently absent.

**The legacy implementation carries less value than its size suggests.** The
inventory in
[`code-inventory.md`](../concepts_sandbox/legacy-rewind/code-inventory.md)
verified, against the source, that the systems the game actually depends on were
not implemented: there is no world reset, no canonical anchor application, no
Echo replay, and no reproducible patrol. What exists is a loop timer, two
PlayerPrefs-backed stores, and a body of authored Chapter 1 detail that is
engine-independent.

**The engine-independent part is the valuable part.** The radio code, the fuse
and generator chain, the courtyard gate, the patrol window, the turnstile
breathing cycle and the five-loop structure are design, not code. They survive
the move at full value.

The cost of switching engines is therefore close to the cost of rewriting
systems that would have to be written from scratch in either engine.

## Decision

RE:WIND is built in **Unreal Engine 5.8**.

The Unity implementation at `C:\code\reWiND` becomes design reference. It is not
ported, not migrated, and not consulted as a base. It is read for authored
content and for what its failures taught, both of which are recorded in
`docs/concepts_sandbox/legacy-rewind/`.

The version is pinned at 5.8 until a decision changes it, because an engine
version bump mid-milestone is a scope event, not a routine update.

## Alternatives considered

**Stay in Unity and keep the C# implementation.** Rejected. It requires
installing a toolchain that is not present, and the implementation it preserves
does not include the systems the first product proof depends on. The saving is
smaller than it appears.

**Defer the decision until the canonical project state is established.**
Rejected, though it was reasonable. The inventory needed to make the decision
was completed during the bootstrap, so deferring would have kept a decided
question open, and an undecided engine blocks every task that produces anything
runnable.

**A custom or third-party engine.** Not considered seriously. The project needs
to test a design hypothesis, not build technology.

## Consequences

**Accepted costs:**

- Roughly thirty C# scripts stop being a codebase and become reference. The
  player controller, the interaction scripts and the patrol behaviour are all
  rewritten.
- The 140 MB of Unity assets in the legacy project do not transfer. Any art
  direction they encode is reference only.
- Determinism must be built against Unreal's tick, actor lifecycle and
  replication assumptions. This is new work with no legacy answer, but it was
  new work in Unity too.

**Accepted benefits:**

- Development can begin on the installed toolchain.
- Nothing is inherited from an implementation whose status could not be trusted.
- Third-person character, camera and rendering are engine-provided, so early
  work goes into the loop rather than into locomotion.

**Consequences that bind future tasks:**

- No task may propose porting Unity code. Legacy code is read, not migrated.
- Determinism in Unreal is a design problem before it is an implementation
  problem. Frame-rate-dependent motion, uninitialised actor state and tick
  ordering are exactly what broke the legacy patrol, and Unreal does not solve
  them for free.
- The asset storage strategy is now urgent. Unreal repositories accumulate
  binary content quickly, and the choice between git, Git LFS and external
  storage is expensive to reverse once binaries are in history. It is an open
  decision in `docs/PROJECT_BRIEF.md` and must be settled before the first
  `.uasset` is committed.
- Blueprint versus C++ is not decided here. It is a smaller decision and belongs
  to the task that first needs it.
