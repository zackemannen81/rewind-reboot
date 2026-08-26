"""Import and read back the REW-0033 WAV set without opening or changing a map.

Run from the project root with the UE 5.8 commandlet:
  UnrealEditor-Cmd.exe Rewind.uproject -ExecutePythonScript="<this file>"
  -unattended -nopause -nosplash -NullRHI
"""

import os

import unreal


TASK = "[REW-0033]"
DESTINATION = "/Game/Audio/Chapter1"
SOURCE = os.path.join(
    unreal.Paths.convert_relative_path_to_full(unreal.Paths.project_dir()),
    "ArtSource", "Audio",
).replace(chr(92), "/")
FILES = (
    "A_REW_Radio_Digit_7.wav",
    "A_REW_Radio_Digit_3.wav",
    "A_REW_Radio_Digit_1.wav",
    "A_REW_Radio_Digit_2.wav",
    "A_REW_Radio_Station_Loop.wav",
    "A_REW_Radio_Static_Loop.wav",
    "A_REW_Radio_Click_On.wav",
    "A_REW_Radio_Click_Off.wav",
    "A_REW_Radio_Tune.wav",
    "A_REW_Step_Interior_01.wav",
    "A_REW_Step_Interior_02.wav",
    "A_REW_Step_Interior_03.wav",
    "A_REW_Step_Interior_04.wav",
    "A_REW_Step_Interior_05.wav",
    "A_REW_Step_Interior_06.wav",
)
LOOPS = {"A_REW_Radio_Station_Loop", "A_REW_Radio_Static_Loop"}


def log(message):
    unreal.log(f"{TASK} {message}")


def stem(filename):
    return filename.rsplit(".", 1)[0]


def set_looping(sound, desired):
    """SoundWave property spelling differs across UE minor versions."""
    for property_name in ("looping", "b_looping"):
        try:
            sound.set_editor_property(property_name, desired)
            return property_name
        except Exception:
            pass
    raise RuntimeError(f"SoundWave looping property unavailable for {sound.get_path_name()}")


def read_looping(sound):
    for property_name in ("looping", "b_looping"):
        try:
            return bool(sound.get_editor_property(property_name)), property_name
        except Exception:
            pass
    raise RuntimeError(f"SoundWave looping property unreadable for {sound.get_path_name()}")


def import_audio():
    tasks = []
    for filename in FILES:
        task = unreal.AssetImportTask()
        task.filename = f"{SOURCE}/{filename}"
        task.destination_path = DESTINATION
        task.destination_name = stem(filename)
        task.automated = True
        task.replace_existing = True
        task.replace_existing_settings = True
        task.save = False
        tasks.append(task)
    unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks(tasks)
    for task in tasks:
        log(f"IMPORT source={task.filename} result={list(task.imported_object_paths or [])}")


def configure_and_verify():
    errors = []
    for filename in FILES:
        name = stem(filename)
        path = f"{DESTINATION}/{name}"
        sound = unreal.EditorAssetLibrary.load_asset(path)
        expected_loop = name in LOOPS
        if not sound:
            errors.append(f"missing SoundWave: {path}")
            continue
        if not isinstance(sound, unreal.SoundWave):
            errors.append(f"not SoundWave: {path} ({sound.get_class().get_name()})")
            continue
        property_name = set_looping(sound, expected_loop)
        unreal.EditorAssetLibrary.save_loaded_asset(sound)
        # Reload after saving so the reported setting is a persisted value, not a
        # temporary in-memory assignment.
        sound = unreal.EditorAssetLibrary.load_asset(path)
        looping, read_property = read_looping(sound)
        duration = float(sound.get_editor_property("duration"))
        log(
            f"VERIFY asset={path} exists=True class={sound.get_class().get_name()} "
            f"duration_seconds={duration:.6f} looping={looping} "
            f"write_property={property_name} read_property={read_property}"
        )
        if looping != expected_loop:
            errors.append(f"looping mismatch: {path}, expected {expected_loop}, got {looping}")
        if duration <= 0.0:
            errors.append(f"nonpositive duration: {path}")
    if errors:
        raise RuntimeError("; ".join(errors))
    log(f"VERIFY PASS: {len(FILES)} SoundWaves exist; only {sorted(LOOPS)} loop")


def main():
    log("starting WAV import; no map asset is opened or modified")
    import_audio()
    configure_and_verify()
    log("DONE")


main()
