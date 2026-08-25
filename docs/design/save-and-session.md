# Save and session

Status: Accepted
Owns: what persists across loops, what persists across sessions, what a
clean save is, when a knowledge fact is stored
Does not own: world apply order, Anchor commit rules, authored facts
other than when the radio code becomes known, storage format
Source decision: [`ADR-0004`](../adr/ADR-0004_save-and-session.md)

A rule that is not in this document is not a save or session rule.

## Across loops

When a loop ends, the game writes knowledge facts and active Anchors.
It does not write LoopWorld or PlayerBody.

The next loop in the same session starts from that written set.

## Across sessions

A session load restores knowledge facts and active Anchors. It does not
restore LoopWorld or PlayerBody. Play after load begins at loop start,
not at the pose where the player quit.

Quitting is not a loop end and not a clean save.

## Clean save

A clean save is:

- knowledge empty;
- no active Anchors;
- the next loop starting from Baseline.

It is a reachable action. The first product proof is run from a clean
save.

## Knowledge facts for the first product proof

A knowledge fact is a named boolean the game stores.

| Fact | Becomes true when |
| --- | --- |
| `radio_code_7312` | The player has obtained the code `7312` from the radio in Apartment 4C this session, including earlier loops |

Hearing the radio is sufficient. The fact stays true across loops and
sessions until a clean save. It does not change the radio. The radio
obeys determinism and always offers the code.

The human tester will also remember. That does not replace this fact.
The fact is what a clean save clears.

Other owning documents may name additional knowledge facts. They use this
store. They do not create a second one. First-run seen-lines are named by
[`tutorial-and-first-run.md`](tutorial-and-first-run.md) as `seen.<MessageId>`.

## Testable statements

1. After a clean save, `radio_code_7312` is false and no Anchor is
   active.
2. After the player obtains `7312` and the loop ends, `radio_code_7312`
   is true at the next loop start.
3. After a session load, `radio_code_7312` and active Anchors match what
   was last written, and PlayerBody is at the loop-start pose.
4. A playtest can reach a clean save without leaving the game's own
   actions, once implementation provides that action.
