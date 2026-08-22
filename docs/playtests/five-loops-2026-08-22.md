# Five Loops Test — evidence run, 2026-08-22

Status: In progress
Task: REW-0003
Runner: MrWhite, with an idle-pair run driven over the editor's MCP endpoint
Base commit: `49244c0`, plus an uncommitted working tree

Five criteria have named evidence. Three are partial. Eight have none.

## Builds

Three builds are involved, and the distinctions matter because defects were
found between them.

| Build | Contents |
| --- | --- |
| A | Authored exposure, bounded outdoor run, closed floor gaps, patrol walking a beat behind a full-width barrier. Courtyard gate and turnstile still 400 cm and 450 cm |
| B | Build A plus courtyard gate and turnstile widened to span the 760 cm corridor |
| C | Build B plus the `LogRewind` run log: loop start and end, apply-order baseline, session writes, interactions, and patrol and turnstile transitions, each carrying elapsed loop time |

All are editor Win64 Development builds of the working tree above.

## The idle-pair run

Build C. Two consecutive loops with **no player input of any kind**: PIE was
started over the editor's MCP endpoint and nothing touched it afterwards. That
is the condition FL-03 and FL-08 require.

Session state at both loop starts: `knowledge=0  anchor=None`. The slot was
already clean from an earlier `Rewind.CleanSave`, so the run began from
clean-save state as `save-and-session.md` defines it. The clean save *action*
was not exercised during this run, which is why FL-15 is still unevidenced.

Raw log: [`idle-pair-log.txt`](five-loops-2026-08-22/idle-pair-log.txt).

Loop 1 ran the full 420 seconds. Loop 2 was captured to `t = 92.67`.

## Tick resolution, and what it does and does not limit

Transitions are written to the log on the tick that first observes the new
state, not at the instant the boundary is crossed. Both loops therefore report
the same boundary at slightly different times.

The offsets are constant inside each loop. Loop 1 reported every patrol
transition at `X.23` and every turnstile close at `X.57`, for all ten patrol
windows and all thirteen turnstile cycles across 420 seconds. Loop 2 reported
`X.00` and `X.67` throughout. A fixed per-loop offset is tick phase relative to
loop start; a drifting clock would wander within a loop, and neither loop
wandered at all.

So the log demonstrates agreement **to within one tick, about 0.25 s**, not
exact equality at an arbitrary `t`. The underlying state is computed as a pure
function of elapsed loop time, so equality at a given `t` follows from the
code; the log corroborates it rather than proving it directly. Closing that gap
needs state sampled at a stated `t`, which the current instrumentation cannot
do.

## Criteria

| ID | State | Evidence |
| --- | --- | --- |
| FL-01 | **Passed** | Build C, idle run, both loop starts. `get_actor_transform` on `RewindCharacter_0` returns `(-150, 0, 98.15)`, the loop-start pose, inside 4C's ±400 bounds. Apply block reads `Gate: CLOSED`, `Fuse: available, courtyard power OFF`, `Generator: offline`. `LOOP START duration=420s knowledge=0 anchor=None` at `t=0.00` |
| FL-02 | Partial | Timer half only. `t=420.23 LOOP END reason=Timer`, after 420 seconds in which nothing else ended the loop. The death half needs `Rewind.EndLoopDeath`, which is a console command and was not run |
| FL-03 | **Passed**, to tick resolution | Two idle loops with no player input. Apply blocks identical line for line. Same cycle structure throughout: patrol boundary at phase 20 of 40, turnstile boundary at phase 2.5 of 30. Deltas −0.23 s and +0.10 s, constant within each loop. See the tick-resolution note above |
| FL-04 | Partial | Build A. `Radio: 7312` on screen at `t = 13.9 s`. Whether it was the first loop of a clean save is not established |
| FL-05 | None | No loop boundary observed with the fact surviving it. The idle run carried `knowledge=0` throughout, so it says nothing here |
| FL-06 | Partial | Build A, one loop, both halves. `Lock: 0000 rejected` at `t = 7.8 s`, then `Lock: 7312 accepted` with buffer echo `Code: 7312` at `t = 15.9 s`. The echo establishes manual entry, since the auto-submit path calls `Submit` directly and never writes the buffer. Clean-save state not established |
| FL-07 | None | The idle run shows the apply order restoring baseline at a loop boundary, but no fuse was routed, no generator started and no gate opened, so the criterion's precondition never occurred |
| FL-08 | **Passed**, to tick resolution | Patrol boundary at phase 20 of a 40 s cycle in both idle loops, at `t = 20, 40, 60, 80` and onward. Zero drift across 420 seconds in loop 1 |
| FL-09 | **Passed** | Twelve consecutive uncovered windows measured across both loops, every one exactly **20.00 s**, repeating every 40 s: loop 1 at `20.23→40.23`, `60.23→80.23` and so on to `380.23→400.23`; loop 2 at `20.00→40.00` and `60.00→80.00` |
| FL-10 | None | No Anchor committed |
| FL-11 | None | Follows FL-10 |
| FL-12 | **Passed**, to tick resolution | Turnstile open at phase 0 and closed by phase 2.57 / 2.67, on a 30 s cycle, sixteen cycles observed. Observed open durations of 2.34 s (loop 1) and 2.67 s (loop 2) bracket the authored 2.5 s from below and above, which is what tick sampling of a 2.5 s window produces |
| FL-13 | None | The hub was reached on an earlier build past a turnstile that did not block. Not evidence. See corrections |
| FL-14 | None | Follows FL-13 |
| FL-15 | None | The slot was already clean when the idle run started. `Rewind.CleanSave` was not invoked during it, so the reachability of the action is still unevidenced. Build C now prints `CLEAN SAVE knowledge=N anchor=X -> CLEAN`, which makes this a one-command observation |
| FL-16 | None | Quit and load not run |

FL-04 and FL-06 were observed on build A. Neither depends on the courtyard
gate or the turnstile, and the radio and code lock were unchanged through
builds B and C, so both observations carry. Re-confirming them on build C is
cheap and would also capture them in the log rather than on screen.

## Supporting observations

Not FL criteria. Recorded because later criteria depend on them.

- Build A, one loop: `Fuse: courtyard has power` at `t = 24.7 s`, then
  `Fuse: already used this loop` at `t = 32.9 s`. That is the once-per-loop
  fuse rule in [`chapter-1-authored.md`](../design/chapter-1-authored.md).
- Build A, same loop: `Generator: online, gate open` at `t = 55.5 s`, after
  power was routed. The generator was not tried before the fuse, so "the
  generator does not start without courtyard power" is not evidenced.
- Build A, same loop: `Lock: 7312 accepted` at `t = 43.8 s` with **no** buffer
  echo, the auto-submit path taken because `radio_code_7312` was already true.
- Build B: the runner confirmed the patrol barrier, the courtyard gate and the
  turnstile all block the corridor with no way around.
- Build C, idle run: `SESSION WRITE knowledge=0 anchor=None` at the loop
  boundary, so the write path runs even with nothing to write.

## Corrections

Four defects were found and fixed during this session. Each invalidates
impressions formed before it.

1. **Exposure was never authored.** The sun is 50000 lux and
   `r.DefaultFeature.AutoExposure` is false, with nothing reconciling the two.
   Lit surfaces clipped to white and everything else read as black. Fixed by an
   unbound post-process volume with manual exposure.
2. **The outdoor run was unbounded**, and the walkway had a 20 cm gap at the 4C
   threshold and a 30 cm gap at the courtyard seam. Fixed with edge walls and a
   continuous courtyard slab.
3. **The patrol had no route and its barrier did not cover the path.** It
   teleported between two poses, and the barrier spanned 200 cm of a 760 cm
   corridor, leaving 280 cm open on each side. FL-09 was not demonstrable.
   Fixed: the body walks a beat, the barrier spans the corridor.
4. **The courtyard gate and the turnstile did not block either**, at 400 cm and
   450 cm against the same 760 cm corridor.

The consequence for earlier work: **any impression that the hub had been
reached through the turnstile, or that a closed gate held, is void for every
run before build B.** The player could walk around both. That includes the
end-to-end run recorded informally earlier on 2026-08-22.

## Images

Screenshots from the build A and build B runs were taken by the runner and are
not in this repository. They belong in
`docs/playtests/five-loops-2026-08-22/`, named by criterion and elapsed time,
for example `FL-06_0000-rejected_t7.8.png`. Image types are tracked by Git LFS
under [`ADR-0005`](../adr/ADR-0005_asset-storage.md), so no configuration
change is needed to add them.

Where a row above cites a build A or B observation, its evidence is the
transcribed on-screen message and its elapsed time, which is weaker than the
image and is recorded as such. Build C rows cite the log instead.

## Not verified

- **The clean save action.** Every criterion above ran from clean-save *state*,
  but `Rewind.CleanSave` itself was not invoked in an observed run.
- **The death end condition.** Only the timer half of FL-02 is evidenced.
- **Anything requiring player movement or interaction on build C.** The MCP
  endpoint exposes no console-command or input tool, so the idle-pair run could
  not press a key. FL-04 to FL-07, FL-10, FL-11, FL-13, FL-14 and FL-16 all
  need a human at the keyboard.
- **Frame rate was not varied**, so the clock's independence from frame rate is
  untested. The zero drift across 420 seconds is evidence at one frame rate.
- **Exact equality at an arbitrary `t`** for FL-03, FL-08 and FL-12, per the
  tick-resolution note.

## Interpretation settled for FL-03 and FL-08

Both criteria say "no player input". The idle-pair run satisfies that
literally: PIE was started and nothing touched it, so no interaction occurred
and the player never left the loop-start pose.

That leaves the reading untested rather than settled for a *played* run, where
reaching the patrol requires passing the code lock and the gate, both of which
are input. A later run that claims FL-03 or FL-08 while moving must state which
reading it used. This run did not have to.
