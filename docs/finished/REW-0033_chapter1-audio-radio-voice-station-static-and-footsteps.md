# Current Task

Task ID: REW-0033
Parent Task: None
Status: Complete
Owner: codex-audio
Created: 2026-08-26
Last updated: 2026-08-26
Charter frozen at: 2026-08-26

## Read First

- `AGENTS.md`
- `docs/TASK_WORKFLOW.md`
- `docs/design/chapter-1-authored.md` (Radio and code)
- `docs/adr/ADR-0002_world-determinism.md`
- `docs/adr/ADR-0011_asset-source-and-provenance.md`
- `docs/EDITOR_AUTOMATION.md`

## Task Summary

Chapter 1's deterministic radio is silent because the project has no audio
assets. Generate project-owned, deterministic source WAVs and import them for
the radio's individual loop-clocked digit triggers, its channel beds and
interaction sounds, plus hard-floor footsteps.

## Task Charter

### Goal

Provide reproducible Chapter 1 audio that makes the radio's `7312` puzzle
audible without changing its loop-clock implementation.

### Primary Deliverable

Generated mono 44.1 kHz 16-bit PCM WAV sources in `Rewind/ArtSource/Audio/`
and their imported Unreal sound assets under `/Game/Audio/Chapter1/`.

### In Scope

- A seed-fixed Python generator under `Rewind/ArtSource/Scripts/` using offline
  Windows SAPI and numpy/scipy only.
- Four separately triggered old-radio digit assets, station/static looping
  beds, radio interaction sounds and six interior hard-floor footsteps.
- Measured written-WAV evidence, seamless-loop seam checks and radio-band
  spectral-energy measurements.
- Headless Unreal import and read-back verification of asset duration and
  looping flags.
- Archive this charter and add the task journal entry.

### Out of Scope

- C++ changes, radio wiring, loop-clock triggers and footstep animation
  notifies.
- Changes to `Rewind/Content/Maps/*`, `docs/CURRENT_STATUS.md` or
  `docs/FILESTRUCTURE.md`.
- Downloaded or third-party audio and any editor map work.

### Definition of Done

- All named WAVs are generated as mono 44.1 kHz 16-bit PCM and measure below
  the specified -3 dBFS peak ceiling.
- The digit assets are independent one-shots; radio and station loops are
  seamless by measured wrap delta.
- The old-radio chain is applied to digits and station bed, static has the
  required radio noise character, and six non-identical footstep variations
  exist.
- The assets import into `/Game/Audio/Chapter1/`; only the two loop assets are
  marked looping on read-back.
- The committed generator deterministically reproduces the WAVs, including
  documented measured evidence.

### Minimum Verification Gates

- [x] Run the generator and inspect WAV metadata, duration, peak and RMS from
  the written files.
- [x] Measure both loop seam deltas and voice/station spectral energy above
  4 kHz before/after band-pass.
- [x] Use UnrealEditor-Cmd headless import, then load each SoundWave to verify
  existence, duration and looping flag.
- [x] Confirm only declared paths changed and `git diff --check` passes.

## References

- `docs/design/chapter-1-authored.md` — 20-second sequence, 50-second cycle,
  digits `7312` at phases 4/9/14/19 and one correct channel.
- `docs/adr/ADR-0002_world-determinism.md` — no loop-count-dependent audio.
- `docs/adr/ADR-0011_asset-source-and-provenance.md` — generated,
  project-owned sources and provenance.
- `docs/EDITOR_AUTOMATION.md` — canonical headless editor procedure.

## Checklist

- [x] Create deterministic generator and headless import/verification script.
- [x] Generate the WAV set and record read-back measurements.
- [x] Import and read back Unreal SoundWave assets.
- [x] Complete documentation, archive and final repository checks.

## Decisions and Notes

- The sequence is intentionally not baked into a monolithic file: each digit
  is a separate one-shot for loop-clock scheduling and partial learning.
- SAPI's installed Microsoft Zira Desktop voice is the project-owned offline
  source. Its synthetic quality serves the numbers-station fiction.
- The generator has a fixed RNG seed; deterministic content remains compatible
  with ADR-0002 because runtime behaviour will be driven by the existing loop
  clock, not randomness.

## Charter Amendment Log

None.

## Verification

- [x] WAV metadata and DSP evidence recorded by the generator.
- [x] Imported asset verification log written by Unreal Python.
- [x] `git diff --check` clean.

## Documentation Updates

- [x] `docs/JOURNAL.md`
- [x] Archive to `docs/finished/REW-0033_chapter1-audio-radio-voice-station-static-and-footsteps.md`

## Handoff and Follow-ups

- Current state: complete; 15 generated WAV sources and 15 imported SoundWaves
  exist, with recorded source and saved-asset read-back evidence.
- Next recommended step: wire the imported one-shots and loops into
  `RewindRadio` and player footstep notifies in a separately chartered task.
- Blockers: none. Runtime wiring is explicitly outside this task.
- Child tasks: none.
- Resume condition: n/a.
- Open questions: none.

## Completion Evidence

`generate_chapter1_audio.py` read the following values back from its written
WAVs. Every row is mono 44.1 kHz, 16-bit PCM; peaks use PCM full-scale.

| File | Duration (s) | Peak (dBFS) | RMS (dBFS) |
| --- | ---: | ---: | ---: |
| `A_REW_Radio_Digit_7.wav` | 1.814376 | -3.000097 | -22.840235 |
| `A_REW_Radio_Digit_3.wav` | 1.569388 | -3.000097 | -20.639576 |
| `A_REW_Radio_Digit_1.wav` | 1.609388 | -3.000097 | -21.670889 |
| `A_REW_Radio_Digit_2.wav` | 1.404399 | -3.000097 | -21.042317 |
| `A_REW_Radio_Station_Loop.wav` | 10.000000 | -3.000097 | -13.841620 |
| `A_REW_Radio_Static_Loop.wav` | 10.000000 | -3.000097 | -15.873684 |
| `A_REW_Radio_Click_On.wav` | 0.110000 | -3.000097 | -19.250043 |
| `A_REW_Radio_Click_Off.wav` | 0.110000 | -3.000097 | -19.429213 |
| `A_REW_Radio_Tune.wav` | 0.110000 | -3.000097 | -19.078619 |
| `A_REW_Step_Interior_01.wav` | 0.251995 | -3.000097 | -17.285842 |
| `A_REW_Step_Interior_02.wav` | 0.263991 | -3.000097 | -18.647536 |
| `A_REW_Step_Interior_03.wav` | 0.275986 | -3.000097 | -18.298256 |
| `A_REW_Step_Interior_04.wav` | 0.287982 | -3.000097 | -18.837910 |
| `A_REW_Step_Interior_05.wav` | 0.300000 | -3.000097 | -21.034860 |
| `A_REW_Step_Interior_06.wav` | 0.311995 | -3.000097 | -18.929920 |

The loop wrap sample delta is `0` PCM units for both
`A_REW_Radio_Station_Loop.wav` and `A_REW_Radio_Static_Loop.wav` after the
tail-to-head crossfade and persisted PCM endpoint check. Measured spectral
energy above 4 kHz (fraction of FFT power, source then post-bandpass) was:

| Source | Before | After |
| --- | ---: | ---: |
| Digit 7 | 0.0022329980 | 0.0000543117 |
| Digit 3 | 0.0016164070 | 0.0000162259 |
| Digit 1 | 0.0015888991 | 0.0000479335 |
| Digit 2 | 0.0311018228 | 0.0004799478 |
| Station loop | 0.2025489484 | 0.0000690231 |
| Static loop | 0.8189756888 | 0.0017209403 |

Two consecutive generator runs produced byte-identical SHA-256 values for all
15 WAVs. The total WAV source size is 2,507,260 bytes.

At `2026.08.26-02.15.07` to `02.15.08`, the headless commandlet log
`Rewind/Saved/Logs/Rewind.log` recorded `VERIFY PASS: 15 SoundWaves exist; only
['A_REW_Radio_Static_Loop', 'A_REW_Radio_Station_Loop'] loop`. Its persisted
read-back durations match this table (to six decimal places); it reports the
`looping` property false for all other assets. No map was opened or modified.
