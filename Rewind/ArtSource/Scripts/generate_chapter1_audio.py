"""Generate RE:WIND Chapter 1 audio without downloaded source material.

Run from the repository root with the workstation's Python:
    python Rewind/ArtSource/Scripts/generate_chapter1_audio.py

The only voice source is the locally installed Windows SAPI Microsoft Zira
Desktop voice. All remaining sound is deterministic numpy/scipy synthesis. A
fixed seed is intentional: generated assets must be reproducible source, not
new random content on each invocation.
"""

from __future__ import annotations

import json
import math
import subprocess
import tempfile
from pathlib import Path

import numpy as np
from scipy import signal
from scipy.io import wavfile


TASK = "REW-0033"
SEED = 7312
SAMPLE_RATE = 44_100
PEAK_TARGET = 10 ** (-3.0 / 20.0)
ROOT = Path(__file__).resolve().parents[2]
OUTPUT = ROOT / "ArtSource" / "Audio"


def dbfs(value: float) -> float:
    return 20.0 * math.log10(max(float(value), 1e-12))


def normalise_with_headroom(samples: np.ndarray, peak: float = PEAK_TARGET) -> np.ndarray:
    samples = np.asarray(samples, dtype=np.float64)
    # Remove DC before scaling. Sparse crackle impulses and the asymmetric
    # soft clip both bias the waveform off centre, and peak normalisation
    # preserves that bias rather than correcting it: the static bed came out
    # at -0.09 DC, which steals headroom and thumps audibly when a loop starts
    # or stops. Centring first also buys back the level the offset was using.
    if samples.size:
        samples = samples - float(np.mean(samples))
    maximum = float(np.max(np.abs(samples)))
    if maximum > 0.0:
        samples = samples * (peak / maximum)
    return samples.astype(np.float32)


def bandpass(samples: np.ndarray, low: float = 300.0, high: float = 3000.0) -> np.ndarray:
    sos = signal.butter(6, [low, high], btype="bandpass", fs=SAMPLE_RATE, output="sos")
    return signal.sosfilt(sos, np.asarray(samples, dtype=np.float64)).astype(np.float32)


def high_frequency_energy(samples: np.ndarray) -> float:
    """Return FFT power above 4 kHz divided by total FFT power."""
    samples = np.asarray(samples, dtype=np.float64)
    window = np.hanning(len(samples))
    power = np.abs(np.fft.rfft(samples * window)) ** 2
    frequencies = np.fft.rfftfreq(len(samples), 1.0 / SAMPLE_RATE)
    return float(np.sum(power[frequencies > 4000.0]) / max(np.sum(power), 1e-30))


def radio_noise(length: int, rng: np.random.Generator, amount: float) -> np.ndarray:
    hiss = bandpass(rng.normal(0.0, 1.0, length), 450.0, 2900.0)
    impulses = np.zeros(length, dtype=np.float32)
    # Sparse impulses make intermittent crackle rather than file-corruption hiss.
    count = max(1, int(length / SAMPLE_RATE * 9.0))
    locations = rng.integers(0, length, size=count)
    impulses[locations] = rng.uniform(0.35, 1.0, size=count) * rng.choice([-1.0, 1.0], size=count)
    crackle = bandpass(impulses, 800.0, 2600.0)
    return (amount * (0.011 * hiss + 0.09 * crackle)).astype(np.float32)


def flanger(samples: np.ndarray, feedback: float = 0.38) -> np.ndarray:
    """Short modulated delay with feedback; deliberately written samplewise."""
    source = np.asarray(samples, dtype=np.float32)
    result = np.zeros_like(source)
    positions = np.arange(len(source), dtype=np.float64) / SAMPLE_RATE
    delays = (3.0 + 2.0 * np.sin(2.0 * np.pi * 0.19 * positions)) * SAMPLE_RATE / 1000.0
    for index in range(len(source)):
        delayed_index = max(0, index - int(delays[index]))
        delayed = result[delayed_index] if index else 0.0
        result[index] = source[index] + feedback * delayed
    wet = np.zeros_like(source)
    for index in range(len(source)):
        delayed_index = max(0, index - int(delays[index]))
        wet[index] = result[delayed_index]
    return (0.76 * source + 0.38 * wet).astype(np.float32)


def schroeder_reverb(samples: np.ndarray) -> np.ndarray:
    """Small metallic room: four short combs into two all-pass stages."""
    source = np.asarray(samples, dtype=np.float32)
    comb_sum = np.zeros_like(source)
    for milliseconds, gain in ((23, 0.56), (29, 0.52), (37, 0.48), (43, 0.45)):
        delay = int(milliseconds * SAMPLE_RATE / 1000)
        comb = np.zeros_like(source)
        for index in range(len(source)):
            delayed = comb[index - delay] if index >= delay else 0.0
            comb[index] = source[index] + gain * delayed
        comb_sum += comb
    result = comb_sum / 4.0
    for milliseconds, gain in ((5, 0.62), (7, 0.58)):
        delay = int(milliseconds * SAMPLE_RATE / 1000)
        allpass = np.zeros_like(result)
        for index in range(len(result)):
            delayed_in = result[index - delay] if index >= delay else 0.0
            delayed_out = allpass[index - delay] if index >= delay else 0.0
            allpass[index] = -gain * result[index] + delayed_in + gain * delayed_out
        result = allpass
    return (0.78 * source + 0.22 * result).astype(np.float32)


def old_radio(samples: np.ndarray, rng: np.random.Generator, noise_amount: float) -> tuple[np.ndarray, dict[str, float]]:
    """The REW-0033 old-radio chain, in the documented order."""
    before = high_frequency_energy(samples)
    filtered = bandpass(samples)
    after = high_frequency_energy(filtered)
    t = np.arange(len(filtered), dtype=np.float64) / SAMPLE_RATE
    wobble = (1.0 + 0.075 * np.sin(2.0 * np.pi * 2.7 * t)
              + 0.035 * np.sin(2.0 * np.pi * 0.31 * t + 0.8))
    processed = flanger(filtered * wobble)
    processed = schroeder_reverb(processed)
    processed = processed + radio_noise(len(processed), rng, noise_amount)
    processed = np.tanh(processed * 1.45)
    return normalise_with_headroom(processed), {
        "above_4khz_before_fraction": before,
        "above_4khz_after_fraction": after,
    }


def enforce_loop_seam(samples: np.ndarray, overlap_seconds: float = 0.10) -> np.ndarray:
    """Crossfade tail into head, then match endpoints for an exactly quiet wrap."""
    result = np.asarray(samples, dtype=np.float64).copy()
    overlap = int(overlap_seconds * SAMPLE_RATE)
    tail = result[-overlap:].copy()
    head = result[:overlap].copy()
    fade_in = np.linspace(0.0, 1.0, overlap, endpoint=True)
    result[-overlap:] = tail * (1.0 - fade_in) + head * fade_in
    # Remove the residual slope so the final stored PCM sample equals the first.
    result -= np.linspace(0.0, result[-1] - result[0], len(result), endpoint=True)
    return result.astype(np.float32)


def write_pcm16(path: Path, samples: np.ndarray, loop: bool = False) -> None:
    samples = normalise_with_headroom(samples)
    encoded = np.round(np.clip(samples, -1.0, 1.0) * 32767.0).astype(np.int16)
    if loop:
        encoded[-1] = encoded[0]
    wavfile.write(path, SAMPLE_RATE, encoded)


def speak_digit(word: str, path: Path) -> np.ndarray:
    """Use only local SAPI. Speech is resampled after SAPI writes its WAV."""
    script = (
        "Add-Type -AssemblyName System.Speech; "
        "$s = New-Object System.Speech.Synthesis.SpeechSynthesizer; "
        "$s.SelectVoice('Microsoft Zira Desktop'); $s.Rate = -2; "
        f"$s.SetOutputToWaveFile('{str(path).replace(chr(39), chr(39) + chr(39))}'); "
        f"$s.Speak('{word}'); $s.Dispose()"
    )
    subprocess.run(["powershell", "-NoProfile", "-NonInteractive", "-Command", script], check=True)
    rate, data = wavfile.read(path)
    if data.ndim > 1:
        data = data.mean(axis=1)
    scale = float(np.iinfo(data.dtype).max) if np.issubdtype(data.dtype, np.integer) else 1.0
    source = np.asarray(data, dtype=np.float32) / scale
    if rate != SAMPLE_RATE:
        divisor = math.gcd(rate, SAMPLE_RATE)
        source = signal.resample_poly(source, SAMPLE_RATE // divisor, rate // divisor).astype(np.float32)
    return source


def mechanical_click(kind: str, rng: np.random.Generator) -> np.ndarray:
    length = int(0.11 * SAMPLE_RATE)
    t = np.arange(length) / SAMPLE_RATE
    noise = rng.normal(0.0, 1.0, length)
    if kind == "on":
        carrier = np.sin(2.0 * np.pi * 165.0 * t) * np.exp(-t * 30.0)
        envelope = np.exp(-t * 48.0)
    elif kind == "off":
        carrier = np.sin(2.0 * np.pi * 115.0 * t) * np.exp(-t * 35.0)
        envelope = np.exp(-t * 55.0)
    else:
        carrier = (np.sin(2.0 * np.pi * 480.0 * t) + np.sin(2.0 * np.pi * 760.0 * t)) * np.exp(-t * 24.0)
        envelope = np.exp(-t * 37.0)
    return normalise_with_headroom(0.24 * carrier + 0.16 * noise * envelope)


def footstep(index: int, rng: np.random.Generator) -> np.ndarray:
    """Hard floor footstep: heel click, short noise body and resonant low thump."""
    length = int((0.24 + 0.012 * index) * SAMPLE_RATE)
    t = np.arange(length) / SAMPLE_RATE
    centre = 900.0 + 95.0 * index
    low = 72.0 + 6.0 * index
    decay = 19.0 + 1.4 * index
    heel = signal.sosfilt(signal.butter(3, [centre - 220.0, centre + 420.0], btype="bandpass", fs=SAMPLE_RATE, output="sos"), rng.normal(0.0, 1.0, length))
    heel *= np.exp(-t * (66.0 + index * 3.0))
    body = signal.sosfilt(signal.butter(2, [180.0, 1450.0 + index * 70.0], btype="bandpass", fs=SAMPLE_RATE, output="sos"), rng.normal(0.0, 1.0, length))
    body *= np.exp(-t * decay)
    thump = np.sin(2.0 * np.pi * low * t) * np.exp(-t * (20.0 + index))
    return normalise_with_headroom(0.27 * heel + 0.13 * body + (0.26 - index * 0.008) * thump)


def loop_source(kind: str, seconds: float, rng: np.random.Generator) -> tuple[np.ndarray, dict[str, float]]:
    length = int(seconds * SAMPLE_RATE)
    t = np.arange(length) / SAMPLE_RATE
    if kind == "station":
        source = (0.065 * np.sin(2.0 * np.pi * 610.0 * t)
                  + 0.035 * np.sin(2.0 * np.pi * 1270.0 * t + 0.2)
                  + 0.030 * rng.normal(0.0, 1.0, length))
        processed, spectrum = old_radio(source, rng, noise_amount=0.75)
    else:
        source = 0.23 * rng.normal(0.0, 1.0, length)
        processed, spectrum = old_radio(source, rng, noise_amount=2.8)
    return enforce_loop_seam(processed), spectrum


def measure_wav(path: Path, loop: bool) -> dict[str, object]:
    rate, data = wavfile.read(path)
    if rate != SAMPLE_RATE or data.dtype != np.int16 or data.ndim != 1:
        raise RuntimeError(f"invalid WAV format: {path}")
    samples = data.astype(np.float64) / 32767.0
    measurement: dict[str, object] = {
        "duration_seconds": len(samples) / rate,
        "peak_dbfs": dbfs(np.max(np.abs(samples))),
        "rms_dbfs": dbfs(np.sqrt(np.mean(samples ** 2))),
        "sample_rate": rate,
        "channels": 1,
        "pcm_bits": 16,
    }
    if loop:
        measurement["wrap_sample_delta_pcm"] = int(abs(int(data[-1]) - int(data[0])))
    return measurement


def main() -> None:
    OUTPUT.mkdir(parents=True, exist_ok=True)
    rng = np.random.default_rng(SEED)
    report: dict[str, object] = {"task": TASK, "seed": SEED, "files": {}, "bandpass": {}}
    digits = (("7", "seven"), ("3", "three"), ("1", "one"), ("2", "two"))
    with tempfile.TemporaryDirectory(prefix="rew_audio_") as temporary:
        temp = Path(temporary)
        for digit, word in digits:
            raw = speak_digit(word, temp / f"{word}.wav")
            processed, spectrum = old_radio(raw, rng, noise_amount=0.55)
            path = OUTPUT / f"A_REW_Radio_Digit_{digit}.wav"
            write_pcm16(path, processed)
            report["files"][path.name] = measure_wav(path, loop=False)
            report["bandpass"][path.name] = spectrum
    for name, kind, seconds in (("A_REW_Radio_Station_Loop.wav", "station", 10.0),
                                ("A_REW_Radio_Static_Loop.wav", "static", 10.0)):
        samples, spectrum = loop_source(kind, seconds, rng)
        path = OUTPUT / name
        write_pcm16(path, samples, loop=True)
        report["files"][path.name] = measure_wav(path, loop=True)
        report["bandpass"][path.name] = spectrum
    for filename, kind in (("A_REW_Radio_Click_On.wav", "on"),
                           ("A_REW_Radio_Click_Off.wav", "off"),
                           ("A_REW_Radio_Tune.wav", "tune")):
        path = OUTPUT / filename
        write_pcm16(path, mechanical_click(kind, rng))
        report["files"][path.name] = measure_wav(path, loop=False)
    for index in range(1, 7):
        path = OUTPUT / f"A_REW_Step_Interior_{index:02d}.wav"
        write_pcm16(path, footstep(index, rng))
        report["files"][path.name] = measure_wav(path, loop=False)
    (OUTPUT / "generation_report.json").write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
    print(json.dumps(report, indent=2))


if __name__ == "__main__":
    main()
