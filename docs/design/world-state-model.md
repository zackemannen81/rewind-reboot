# World-state model

Status: Accepted
Owns: named state buckets and the loop-start apply order
Does not own: what an Anchor means, what knowledge is, authored baseline
values, Echo
Answers: conflict C6, as far as the first product proof needs

A rule that is not in this document is not a world-state rule.

## Buckets

| Bucket | Contents | Across a loop boundary |
| --- | --- | --- |
| Baseline | Authored default configuration of the space | Unchanged. Read, never rewritten by play |
| LoopWorld | Object pose, puzzle flags, patrol phase, gate, fuse, generator, turnstile, collision | Discarded. Rebuilt at loop start |
| PlayerBody | Pose, stance, traversal state | Discarded. Replaced by the loop-start pose |
| Anchors | The committed override set | Persists. Stored by save-and-session |
| Knowledge | Discovered facts | Persists. Not world. Stored by save-and-session |

LoopWorld plus PlayerBody is the loop-volatile world. ADR-0002's "world
configuration" at time `t` is Baseline, then Anchors, then LoopWorld
simulated to `t`, with PlayerBody from this-loop play.

**An object the player is carrying is LoopWorld, not PlayerBody.** Being
held is one of the positions an object can be in, alongside sitting in a
drawer or in a socket. There is no inventory bucket and none is needed: a
carried object is discarded with the rest of LoopWorld at loop start and
rebuilt at its authored position, which is the behaviour a carried object
wants anyway.

## Apply order at loop start

A single ordered apply. Object-local reset that runs later and overwrites
this order is a defect.

1. Discard LoopWorld and PlayerBody.
2. Instantiate LoopWorld from Baseline.
3. Apply each active Anchor to LoopWorld.
4. Place PlayerBody at the loop-start pose.
5. Set elapsed loop time to `0`.
6. Simulate.

If step 3 and a later object `OnLoopStart` disagree, step 3 is right and
the object is wrong.

## What this does not decide

How an implementation represents a bucket. Whether that representation is
a struct, an actor flag or a save blob is the implementation task.

## Testable statements

1. At `t = 0`, before this-loop input, LoopWorld matches Baseline except
   for active Anchor overrides.
2. PlayerBody at `t = 0` is the loop-start pose, including when an Anchor
   is active.
3. Patrol phase, fuse, generator and turnstile at `t = 0` match Baseline.
   They are not restored from the previous loop.
4. An object that resets itself after step 3 and clears an applied Anchor
   has failed this model.
