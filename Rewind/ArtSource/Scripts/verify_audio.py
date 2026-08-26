"""Measure a directory of WAVs independently of whatever produced them.

Audio is the easiest kind of asset to claim and the easiest to check, so there
is no excuse for taking a generator's word for it. This reports what is
actually in the files: duration, peak, RMS, DC offset, how much energy survives
above the band-limit, and — for anything named *_Loop — the sample delta across
the wrap point, which is the difference between a loop and an audible click
every few seconds.

Pure numpy/scipy. No engine, no ffmpeg, neither of which is installed here.

    python verify_audio.py <dir>            # defaults to Rewind/ArtSource/Audio
"""
import glob
import os
import sys

import numpy as np
from scipy.io import wavfile

# An AM radio carries roughly 300-3000 Hz. Energy well above that means the
# band-pass did not happen, whatever the log said.
BAND_HZ = 4000.0
# A seam louder than this is audible as a click on loop.
SEAM_LIMIT = 0.02


def db(x):
    return -np.inf if x <= 0 else 20.0 * np.log10(x)


def measure(path):
    rate, data = wavfile.read(path)
    if data.dtype.kind == "i":
        data = data.astype(np.float64) / np.iinfo(data.dtype).max
    else:
        data = data.astype(np.float64)
    mono = data.mean(axis=1) if data.ndim > 1 else data
    n = len(mono)

    peak = float(np.max(np.abs(mono))) if n else 0.0
    rms = float(np.sqrt(np.mean(mono ** 2))) if n else 0.0

    # Fraction of total energy above BAND_HZ.
    spec = np.abs(np.fft.rfft(mono * np.hanning(n))) ** 2 if n else np.array([0.0])
    freqs = np.fft.rfftfreq(n, 1.0 / rate) if n else np.array([0.0])
    total = spec.sum()
    above = float(spec[freqs > BAND_HZ].sum() / total) if total > 0 else 0.0

    # Loop seam: how far the last sample sits from the first.
    seam = abs(float(mono[-1] - mono[0])) if n else 0.0

    return {
        "rate": rate, "ch": 1 if data.ndim == 1 else data.shape[1],
        "bits": 16 if data.dtype == np.int16 else str(data.dtype),
        "secs": n / rate if rate else 0.0,
        "peak_db": db(peak), "rms_db": db(rms),
        "dc": float(np.mean(mono)) if n else 0.0,
        "above_band": above, "seam": seam,
    }


def main():
    root = sys.argv[1] if len(sys.argv) > 1 else "Rewind/ArtSource/Audio"
    paths = sorted(glob.glob(os.path.join(root, "**", "*.wav"), recursive=True))
    if not paths:
        print(f"no wav files under {root}")
        return 1

    print(f"{'file':<34}{'sec':>7}{'peak':>8}{'rms':>8}"
          f"{'>4kHz':>8}{'DC':>9}{'seam':>8}  notes")
    print("-" * 104)
    problems = 0
    for p in paths:
        m = measure(p)
        notes = []
        if m["rate"] != 44100:
            notes.append(f"rate {m['rate']}")
        if m["ch"] != 1:
            notes.append(f"{m['ch']}ch (radio should be mono)")
        if m["peak_db"] > -1.0:
            notes.append("clipping risk")
            problems += 1
        if m["peak_db"] < -12.0:
            notes.append("very quiet")
        if abs(m["dc"]) > 0.01:
            notes.append("DC offset")
            problems += 1
        is_loop = "_Loop" in os.path.basename(p)
        if is_loop and m["seam"] > SEAM_LIMIT:
            notes.append(f"SEAM CLICKS ({m['seam']:.3f})")
            problems += 1
        # Only sound that came through the CARRIER is band-limited. A radio's
        # switch and tuning clicks are made by the physical box in the room,
        # not transmitted, so they are correctly broadband -- an earlier
        # version of this check flagged all three of them as failures.
        name = os.path.basename(p)
        carried = "Radio" in name and not any(
            k in name for k in ("Click", "Tune"))
        if carried and m["above_band"] > 0.05:
            notes.append(f"not band-limited ({m['above_band']*100:.1f}% >4kHz)")
            problems += 1
        print(f"{os.path.basename(p):<34}{m['secs']:>7.2f}"
              f"{m['peak_db']:>8.1f}{m['rms_db']:>8.1f}"
              f"{m['above_band']*100:>7.1f}%{m['dc']:>9.4f}"
              f"{m['seam']:>8.4f}  {', '.join(notes)}")

    print("-" * 104)
    print(f"{len(paths)} files, {problems} with problems")
    return 0


if __name__ == "__main__":
    sys.exit(main())
