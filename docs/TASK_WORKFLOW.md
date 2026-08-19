# Task Workflow

## States

```text
Draft → Ready → In Progress → Complete
                     ↓
                   Paused
                     ↓
                In Progress

Draft / Ready / In Progress / Paused
  → Cancelled or Superseded
```

`Draft` is editable. `Ready` freezes the charter: goal, primary deliverable,
scope, out-of-scope, definition of done and minimum verification gates.

A frozen charter is superseded, never rewritten. If the goal turns out to be
wrong, archive the task as `Superseded` and charter its replacement. Rewriting a
frozen charter into a different task destroys the only record of what was
actually agreed.

## Task Identity

A task identity is an address. It appears in the active charter, the archive
filename, journal entries, branch names, commit messages and pull request
titles, and several of those cannot be rewritten afterwards.

Claim the identity in `docs/TASK_IDS.md` and merge that claim to `main` before
the charter moves to `Ready`. Append one row at the end of the table; never
insert into the middle and never sort. Two people claiming at the same moment
then edit the same region and the second gets a merge conflict rather than a
silent duplicate.

The register cannot prevent the race. It converts it into a conflict, which is
the only reliable outcome available between actors who cannot see each other's
branches.

The register allocates identity only. It carries no status column: task state
already has owners in `docs/CURRENT_TASK.md` and `docs/finished/`, and a
trunk-level statement about how much work is in progress would contradict the
one-active-task rule.

The prefix in force is `REW`. Historical `DFC-` identities remain valid
addresses.

## Routing Discovered Work

Every discovery gets one of four destinations. Choosing none of them, and
simply doing the work, is how a frozen charter erodes.

```text
Is it required by the frozen charter?
├─ Yes → add a checklist step and do it
└─ No
   ├─ Does it block the charter?
   │  ├─ Yes → pause the parent, activate a bounded child task
   │  └─ No
   │     ├─ In project scope, later → docs/backlog/
   │     └─ Outside project scope   → docs/concepts_sandbox/
```

A backlog proposal records discovery context, proposed outcome, why it is
outside the active charter, dependencies and suggested verification. Add it to
`docs/backlog/README.md` in the same change; an unindexed proposal is invisible.

Resolving a proposal updates its `Status:` line and its index row. It never
renames or moves the file, because journal entries and archived tasks cite
proposals by path and cannot be edited to follow a rename.

## Pause

A pause records what blocked the work, what the next step is, which
verifications did not run, and the condition that resumes the task. Move the
frozen parent to `docs/paused/` and keep its identity, goal and definition of
done unchanged.

## Completion

- Verify in proportion to risk. State what was not verified and why.
- Update every affected owning document in the same change.
- Archive the task under `docs/finished/` as `REW-NNNN_task-slug.md`,
  unmodified, or under the prefix the identity was claimed with.
- Restore `docs/CURRENT_TASK.md` from the template, or fill it with the next
  approved task.
- Add a dated, signed journal entry.

A task is not complete because the work feels done. It is complete when the
repository shows what was produced, what was verified, and what the next actor
should do.
