# Five Loops Test — evidence run, 2026-08-22

Status: Complete
Task: REW-0003
Runner: MrWhite, with one idle-pair run driven over the editor's MCP endpoint
Base commit: `49244c0`, plus the working tree on branch `rew-0003/run-evidence`

**All sixteen criteria have named evidence.** Three pass to a stated tick
resolution rather than exactly, and one passes on the letter with a caveat that
matters more than the pass does. Both are stated below rather than buried.

This is evidence that the criteria hold. It is not a claim that the loop is
fun; `five-loops-test.md` deliberately excludes enjoyment, and this record takes
no position on it beyond the observation under FL-14.

## Builds

| Build | Contents |
| --- | --- |
| A | Authored exposure, bounded outdoor run, closed floor gaps, patrol walking a beat behind a full-width barrier. Courtyard gate and turnstile still 400 cm and 450 cm |
| B | Build A plus courtyard gate and turnstile widened to span the 760 cm corridor |
| C | Build B plus the `LogRewind` run log |
| D | Build C plus a hub-entry event and the loop-start pose in the apply block |

All are editor Win64 Development builds. Every criterion below is evidenced on
build C or D. Build A and B observations were superseded and survive only in
the corrections section.

## Runs

| Run | Log | What it produced |
| --- | --- | --- |
| Idle pair | [`idle-pair-log.txt`](five-loops-2026-08-22/idle-pair-log.txt) | Two loops, no player input at all. FL-01, FL-03, FL-08, FL-09, FL-12, and the timer half of FL-02 |
| Played run | [`played-run-log.txt`](five-loops-2026-08-22/played-run-log.txt) | Clean session, full chain, Anchor committed, ended by death. FL-04, FL-05, FL-06, FL-11, FL-13, the death half of FL-02, the positive half of FL-10 |
| Sequence | [`sequence-run-log.txt`](five-loops-2026-08-22/sequence-run-log.txt) | Clean save, a baseline run and two learned runs. FL-15 and FL-14 |
| Session reload | [`session-reload-log.txt`](five-loops-2026-08-22/session-reload-log.txt) | A full editor quit and reopen, then a chain loop committing nothing. FL-16, FL-07, the negative half of FL-10 |

## Criteria

| ID | State | Evidence |
| --- | --- | --- |
| FL-01 | **Passed** | Every loop start in every run. `[apply] Gate: CLOSED`, `Fuse: available, courtyard power OFF`, `Generator: offline`, `Player: placed at loop-start pose (-150, 0, 96)`, then `LOOP START ... knowledge=0 anchor=None` at `t=0.00`. The pose is inside 4C's ±400 bounds; a live `get_actor_transform` during the idle run returned `(-150, 0, 98.15)` after the capsule settled |
| FL-02 | **Passed** | Both end conditions and no others. Timer: `t=420.23 LOOP END reason=Timer` on the idle run, after 420 seconds in which nothing else ended it. Death: `t=269.46 LOOP END reason=Death`. Across roughly a dozen logged loops, none ended from any other cause |
| FL-03 | **Passed**, to tick resolution | Two idle loops, no player input. Apply blocks identical line for line. Same cycle structure throughout. See the tick-resolution note |
| FL-04 | **Passed** | `LOOP START knowledge=0` at `t=0.00`, then `Radio: 7312 obtained` at `t=22.06`, so the code was obtainable on the first loop of a clean session |
| FL-05 | **Passed** | `Radio: 7312 obtained` at `t=22.06`, `SESSION WRITE knowledge=1` at the loop end, `LOOP START ... knowledge=1` on the next loop. `radio_code_7312` is the only fact the system defines |
| FL-06 | **Passed** | One loop from a clean session. `Lock: manual entry 0000` then `Lock: 0000 rejected` at `t=16.26`; `Lock: manual entry 7312` then `Lock: 7312 accepted, doorway open` at `t=28.59`. The log names the entry path, so manual entry is stated, not inferred |
| FL-07 | **Passed** | Session-reload run. Clean save, then a loop with `Fuse: routed` at `t=7.83`, `Generator: online, gate opened by this-loop play` at `t=11.74`, and no board interaction. `SESSION WRITE knowledge=1 anchor=None`, then the next apply block: `Fuse: available, courtyard power OFF`, `Generator: offline`, `Gate: CLOSED`, with **no anchor line following it**, and `LOOP START ... anchor=None` |
| FL-08 | **Passed**, to tick resolution | Patrol boundary at phase 20 of a 40 s cycle in both idle loops, at `t = 20, 40, 60, 80` onward. Zero drift across 420 seconds |
| FL-09 | **Passed** | Twelve consecutive uncovered windows across two idle loops, every one exactly **20.00 s**, repeating every 40 s |
| FL-10 | **Passed** | Both halves. Negative: the FL-07 loop started the generator, committed nothing, and the next loop carries `anchor=None` with the gate closed. Positive: `Board: commit courtyard_gate_open ACCEPTED (gate open from this-loop play=yes)` at `t=47.34`, and the next loop start carries `anchor=courtyard_gate_open`. A refusal is also recorded, `REFUSED (gate open from this-loop play=no)` at `t=32.17`, when a commit was attempted before the gate was opened |
| FL-11 | **Passed** | Loop after the commit: `[apply] Gate: CLOSED` then `[apply] Gate: OPEN, held by anchor courtyard_gate_open`, and `LOOP START ... anchor=courtyard_gate_open` at `t=0.00`, with no this-loop input |
| FL-12 | **Passed**, to tick resolution | Turnstile open at phase 0, closed by phase 2.50 to 2.67, on a 30 s cycle, more than twenty cycles observed across all runs |
| FL-13 | **Passed** | Session-reload run, one loop from a clean save: `Radio: 7312 obtained` at `t=3.83`, `Fuse: routed` at `t=7.83`, `Lock: 7312 accepted` at `t=9.88`, `Generator: online, gate opened` at `t=11.74`, and `Hub: entered past the turnstile (turnstile was OPEN)` at `t=30.26`. All inside one 420-second loop |
| FL-14 | **Passed on the letter.** See the caveat | Sequence run, one PIE session. Baseline from a clean save: `Hub: entered` at `t=30.93`. Learned runs with `radio_code_7312` true and `courtyard_gate_open` active: `t=30.25` and `t=30.24`. Smaller, as required |
| FL-15 | **Passed** | `CLEAN SAVE knowledge=0 anchor=None -> CLEAN` at `t=14.07`, immediately followed by an apply block satisfying FL-01 and `LOOP START ... knowledge=0 anchor=None` |
| FL-16 | **Passed** | Full editor quit and reopen, a new log file. First apply block of the new session: `Gate: OPEN, held by anchor courtyard_gate_open`, `Player: placed at loop-start pose (-150, 0, 96)`, then `LOOP START duration=420s knowledge=1 anchor=courtyard_gate_open`. Both facts restored; play begins at the loop-start pose, not the quit pose |

## The FL-14 caveat, which matters more than the pass

FL-14 asks that the learned run reach the turnstile at a smaller elapsed `t`.
It did: 30.24 against a 30.93 baseline. But every crossing in every run lands
between `t = 30.24` and `t = 30.93`, and the turnstile opens at `t = 30`.

The player arrived before the window and waited, on every run. The baseline run
did the whole chain — radio, fuse, generator, board commit — and still arrived
early. One learned run finished the chain at `t = 8.37` and then stood at the
turnstile for twenty-two seconds.

So the 0.69 s difference measures when the turnstile opened, not how much
faster the player was. The criterion passes as written, and the number is close
to meaningless as evidence that knowledge makes a player faster. The player
cannot be slower than the gate cycle.

This is the measurement in
[`loop-pressure-and-interaction.md`](../backlog/loop-pressure-and-interaction.md)
appearing inside the acceptance criteria themselves. It is recorded here so
that no later reader treats FL-14's pass as evidence that the loop has time
pressure. It does not.

## Tick resolution, and what it limits

Transitions are written on the tick that first observes the new state, not at
the instant the boundary is crossed, so two loops report the same boundary
slightly apart.

The offsets are constant inside each loop. One idle loop reported every patrol
transition at `X.23` and every turnstile close at `X.57`, across all ten patrol
windows and thirteen turnstile cycles in 420 seconds. The other reported `X.00`
and `X.67` throughout. A fixed per-loop offset is tick phase relative to loop
start; a drifting clock would wander within a loop, and neither did.

FL-03, FL-08 and FL-12 are therefore demonstrated to within one tick, about
0.25 s, not to exact equality at an arbitrary `t`. State is a pure function of
elapsed loop time, so exact equality follows from the code; the log corroborates
it rather than proving it.

## Supporting observations

Not FL criteria.

- `Generator: refused, no courtyard power` at `t=15.48`. That is testable
  statement 4 of [`chapter-1-authored.md`](../design/chapter-1-authored.md),
  which had no evidence before.
- `Fuse: refused, already used this loop`, the once-per-loop fuse rule.
- The two `[apply] Gate:` lines on an anchored loop show the apply order of
  [`world-state-model.md`](../design/world-state-model.md) directly: baseline at
  step 2, the Anchor overriding it at step 3.
- Two `Player: placed` lines appear at PIE start only.
  `ARewindGameMode::RestartPlayer` calls `PlacePlayerBody` when the pawn spawns,
  and `ApplyLoopStart` calls it again at step 4. Two callers, not a double
  apply. The second placement wins, and loop boundaries show one line. Benign.

## Corrections

Four defects were found and fixed during this session. Each invalidates
impressions formed before it.

1. **Exposure was never authored.** The sun is 50000 lux and
   `r.DefaultFeature.AutoExposure` is false, with nothing reconciling the two.
   Fixed by an unbound post-process volume with manual exposure.
2. **The outdoor run was unbounded**, with a 20 cm gap at the 4C threshold and
   a 30 cm gap at the courtyard seam.
3. **The patrol had no route and its barrier did not cover the path.** It
   teleported between two poses, and the barrier spanned 200 cm of a 760 cm
   corridor. FL-09 was not demonstrable.
4. **The courtyard gate and the turnstile did not block either**, at 400 cm and
   450 cm against the same corridor.

**Any impression that the hub had been reached through the turnstile, or that a
closed gate held, is void for every run before build B.** Both could be walked
around. That includes an end-to-end run recorded informally earlier the same
day, and it is why every criterion above cites build C or D.

## Not verified

- **Frame-rate independence.** All runs were at one frame rate. The zero drift
  across 420 seconds is evidence at that rate only.
- **Exact equality at an arbitrary `t`** for FL-03, FL-08 and FL-12.
- **A packaged build.** Everything here is editor PIE.
- **Whether any of it is enjoyable.** Excluded from these criteria by design,
  and untouched by this record.
