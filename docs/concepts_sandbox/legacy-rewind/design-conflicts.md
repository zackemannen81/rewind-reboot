# Design conflicts in the imported RE:WIND material

Status: Non-authority. A register of contradictions, not a decision record.
Read: 2026-08-19, from `original-gdd/`, `old-task-material/` and
`code-inventory.md`. Nothing here is a rule. Each row is a question that a
decision in `docs/adr/` must answer before a task may depend on an answer.

Conflicts are not resolved by picking the version that reads most reasonably.

## How to use this file

A future task that needs a rule looks up the question, writes an ADR, and
restates the chosen rule in an owning document under `docs/design/` or
`docs/PROJECT_BRIEF.md`. Until that happens, both sides remain proposals.

## Conflicts

### C1. Is the world the same every loop, or does it change?

- `docs/PROJECT_BRIEF.md` (this repository, approved) states: the world is
  deterministic; the player accumulates knowledge.
- `original-gdd/GDD_About.md` states that the world does not appear to follow
  the same rules each time, and that small details change between loops: a
  door that was closed is open, a statue moves, a poster changes.
- The same GDD later requires pattern-driven patrols that the player can
  learn, which only works if those patterns are stable.

Question: what, exactly, is identical across loops, and what is allowed to
change?

### C2. Which engine is the project built in?

- `original-gdd/TECHNICAL_SOLUTIONS.md`: Unity 2022.3 LTS.
- `original-gdd/GDD_About.md`: "Unreal Engine 5 or Unity HDRP (URP fallback)",
  in more than one pasted overview.
- This repository: Unreal Engine 5.8, accepted in
  `docs/adr/ADR-0001_engine-unreal-engine-5.md`.

Question: closed for this repository by ADR-0001. Left here because the
imported material still contradicts itself and will be misread if the
conflict is not named.

### C3. What is an Anchor?

- GDD: a causality lock that survives the next loop, costs Insight, limited
  to 1–2 per chapter, chosen by the player at the apartment board.
- `TECHNICAL_SOLUTIONS.md`: `AnchorManager` listens for `AnchorAppliedEvent`
  and modifies `WorldState` at loop start.
- Code: a `HashSet<string>` in PlayerPrefs. One identifier exists. The
  courtyard gate opens itself at loop start if that string is present.
  `Chapter1LoopOrchestrator` writes the string automatically after a
  successful generator activation. There is no Insight spend and no board.

Question: is an Anchor a player-chosen, Insight-priced lock, an automatic
flag, or something else? What objects may it override?

### C4. Does Insight exist?

- GDD: Insight is the progression currency. Anchors cannot be placed without
  it. It is earned by understanding, not by XP.
- Code: no Insight type, no spend, no board spend flow.
- This repository's brief leaves Insight undecided.

Question: is there a spendable meta-currency at all?

### C5. What is Echo?

- GDD overview: silhouette ghosts of the player's best attempt, or of an
  "optimal future". Follow or break the route.
- GDD section 10.3: echoes are non-interactive, visible and audible.
- The next bullet in the same section: echoes can trigger environmental
  changes (buttons, switches). Those cannot both be true.
- GDD later: echo cooperation puzzles with a previous self; deterministic
  action replay on a fixed timestep.
- Code: transform samples on the echo object, never saved, never played back.
  A sphere trail is built at loop end and destroyed at the next start.

Question: is Echo a visual hint, a recorded action replay that can press
things, a narrative device, or absent from the first product proof?

### C6. Is there a central world state?

- GDD and `TECHNICAL_SOLUTIONS.md` describe `WorldState`, reset at loop
  start, then anchors applied, then the player spawns.
- Code: no `WorldState` type. Each Chapter 1 object that has `OnLoopStart`
  resets itself if the orchestrator calls it. Patrols, player and echo are
  not in that list.

Question: is world reset a single ordered apply, or a set of object-local
handlers?

### C7. What persists across loops, and what persists across sessions?

- GDD save path: Insight, Anchors, Knowledge at loop end; volatile world
  reset.
- Code: knowledge and anchors go to PlayerPrefs with no clear. If those
  managers ran, a "clean save" would not be achievable from inside the game.
- Player pose, patrol index and `HasCompletedGoldenPath` are not specified
  as persistent in the GDD and are not reset in code.

Question: what is a clean save? What survives a loop? What survives quitting?

### C8. How long is the turnstile window, and what is the guard window?

- GDD Chapter 1: turnstile breathes every 30 seconds, open ~2.5 seconds.
  The guard's route leaves a 20-second window to the hub gate.
- Code: turnstile `windowDuration = 3f`. Guard investigate duration is 4
  seconds. There is no 20-second hub window in the patrol script.

Question: which timings are the design, and which were placeholders?

### C9. Do later temporal mechanics belong in this phase?

- GDD Chapters 2–5 introduce backwards time, echo chambers, temporal
  anchoring devices, paradoxes, local time language.
- `TECHNICAL_SOLUTIONS.md` specifies a `ParadoxValidator`.
- This repository's brief lists all of that as non-goals for the current
  phase.

Question: closed for this phase by `docs/PROJECT_BRIEF.md`. Named here so a
reader of the GDD does not treat Chapter 3 systems as implied work.

### C10. Does the world learn the player?

- One GDD passage: enemies are pattern-driven; failure teaches the route.
- Another: meta-twists in which some loops break the game's own rules; the
  player wakes in the wrong place or is a different person.
- Brief for this repository: knowledge is worthless if the world is not the
  same next time.

Question: are rule-breaking loops a narrative exception recorded as such, or
part of baseline behaviour?

### C11. Is there a diegetic-only interface, or a debug interface?

- GDD: no traditional UI or text; information is environmental.
- Code: fuse and generator interactions `Debug.Log` instructions and results.
  Radio knowledge is stored as a string, not written to the apartment board
  described in the GDD.

Question: what must the first product proof show the player, and through
which surface?

### C12. Which status documents were true?

- `task_core_systems.md` (`SYS-001`): `DONE`.
- `task_player_controller.md` (`PC-001`): `DONE`.
- `task_chapter_1.md` (`CH1-001`): `IN_PROGRESS`, with a golden-path
  acceptance criterion that includes the first causality anchor.
- `code-inventory.md`: the player controller exists as a substantial class;
  the core systems named in SYS-001 are not a running loop; Chapter 1 is a
  bootstrap-built blockout with authored interactions that do not reset the
  world.

Question: none to decide. The answer is already in `code-inventory.md`. The
conflict is recorded so that `DONE` in the sandbox is never re-imported as
a starting position.

## Conflicts that are not in this register

Duplicate prose inside `GDD_About.md` (the document concatenates several
overviews) is noise, not a load-bearing disagreement, unless it changes a
rule. Those copies were not listed one by one.

Engine-version pinning inside Unreal, Blueprint versus C++, and asset
storage are open in `docs/PROJECT_BRIEF.md` and do not come from a clash
inside the legacy material.
