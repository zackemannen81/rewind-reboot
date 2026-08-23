# Playtest Records

Discoverability: index. Every record in this directory is listed below.
Member state: required. Every record declares a `Status:` line under its title.

Evidence from named runs of a named build. A record says what was observed, at
what elapsed loop time, from what starting state, and what was not observed.

This collection holds evidence. It does not hold criteria.
[`docs/acceptance/`](../acceptance/README.md) owns what a build must
demonstrate, and a record here cites those criteria by identifier without
restating them. If a record and a criterion disagree about what must be shown,
the criterion is right.

A record is written from what was seen. An impression is not evidence, a
checklist tick is not evidence, and a passing compile is not evidence. A
criterion with nothing behind it is recorded as having nothing behind it, never
omitted and never left blank.

Records are not edited to look better later. A run that was invalidated by a
defect found afterwards keeps its entry and gains a correction.

## Naming

A record is `{proof-slug}-{YYYY-MM-DD}.md`. Its images, if any, live in a
directory of the same name without the extension, and the record names each one.
Image types are already tracked by Git LFS in `.gitattributes` under
[`ADR-0005`](../adr/ADR-0005_asset-storage.md).

## Records

| Record | Status | Notes |
| --- | --- | --- |
| [`five-loops-2026-08-22.md`](five-loops-2026-08-22.md) | Complete | All sixteen criteria have named evidence. Three pass to tick resolution; FL-14 passes on the letter with a stated caveat |
| [`chapter-1-three-loops-2026-08-23.md`](chapter-1-three-loops-2026-08-23.md) | Complete | Rebuilt Chapter 1 chain: formal information, commit and held-state loops; touched FL criteria re-verified |
