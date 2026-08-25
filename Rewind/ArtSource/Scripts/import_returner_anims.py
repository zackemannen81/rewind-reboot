"""Import the Returner's Meshy clips onto its own skeleton and report them.

The question that matters is whether each clip is in place. `ARewindCharacter`
drives locomotion from CharacterMovement velocity, so a clip carrying forward
root motion would fight it and the character would slide or double-move.

Reports, per clip: frame count, length, whether root motion is enabled, and
how far the root bone actually travels across the clip. The last one is the
real test — a clip can have root motion disabled as a flag while still baking
translation into the root track.

Run headless:
    UnrealEditor-Cmd.exe <uproject> -ExecutePythonScript="<this file>"
      -unattended -nopause -nosplash
"""
import unreal

SKELETON = "/Game/Characters/Returner/Returner_Skeleton"
DEST = "/Game/Characters/Returner/Animations"
SRC = "C:/code/rewind-reboot/Rewind/ArtSource/Characters/Returner/"

CLIPS = [
    ("Returner_Walking.fbx", "A_Returner_Walk"),
    ("Returner_Running.fbx", "A_Returner_Run"),
    ("Returner_WalkingWoman.fbx", "A_Returner_Walk2"),
    ("Returner_Alert.fbx", "A_Returner_Alert"),
]


def log(m):
    unreal.log("[REW-0026] " + str(m))


def import_anim(filename, asset_name, skeleton):
    task = unreal.AssetImportTask()
    task.filename = SRC + filename
    task.destination_path = DEST
    task.destination_name = asset_name
    task.automated = True
    task.replace_existing = True
    task.save = True

    opts = unreal.FbxImportUI()
    opts.import_mesh = False
    opts.import_as_skeletal = True
    opts.import_animations = True
    opts.import_materials = False
    opts.import_textures = False
    opts.skeleton = skeleton
    opts.set_editor_property("mesh_type_to_import",
                             unreal.FBXImportType.FBXIT_ANIMATION)
    opts.anim_sequence_import_data.set_editor_property(
        "import_bone_tracks", True)
    opts.anim_sequence_import_data.set_editor_property(
        "remove_redundant_keys", False)
    task.options = opts

    unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks([task])
    return list(task.get_editor_property("imported_object_paths") or [])


def root_travel(anim):
    """How far the first bone moves from first to last frame, in cm."""
    try:
        n = anim.get_editor_property("number_of_sampled_keys")
    except Exception:
        n = 0
    if not n:
        return None
    try:
        bone = anim.get_editor_property("skeleton").get_editor_property(
            "bone_tree")
        # Bone name comes from the mesh's reference skeleton instead; the
        # first track in the sequence is the root either way.
        names = unreal.AnimationLibrary.get_animation_track_names(anim)
        if not names:
            return None
        root = names[0]
        a = unreal.AnimationLibrary.get_bone_pose_for_frame(anim, root, 0, False)
        b = unreal.AnimationLibrary.get_bone_pose_for_frame(anim, root, n - 1, False)
        return (b.translation - a.translation).length(), str(root)
    except Exception as e:
        return ("error: " + str(e), "?")


def run():
    skeleton = unreal.EditorAssetLibrary.load_asset(SKELETON)
    if not skeleton:
        log("FATAL: skeleton not found at " + SKELETON)
        return
    log("skeleton loaded: " + SKELETON)

    for filename, asset_name in CLIPS:
        try:
            made = import_anim(filename, asset_name, skeleton)
            log(f"{filename} -> {made}")
        except Exception as e:
            log(f"{filename} -> IMPORT FAILED: {e}")

    log("=" * 60)
    ar = unreal.AssetRegistryHelpers.get_asset_registry()
    for a in ar.get_assets_by_path(DEST, recursive=True):
        if str(a.asset_class_path.asset_name) != "AnimSequence":
            continue
        anim = unreal.EditorAssetLibrary.load_asset(str(a.package_name))
        if not anim:
            continue
        try:
            frames = anim.get_editor_property("number_of_sampled_keys")
        except Exception:
            frames = "?"
        try:
            length = anim.get_editor_property("sequence_length")
        except Exception:
            length = "?"
        try:
            rm = anim.get_editor_property("enable_root_motion")
        except Exception:
            rm = "?"
        travel = root_travel(anim)
        log(f"{a.asset_name}: frames={frames} length={length} "
            f"root_motion_flag={rm} root_travel={travel}")
    log("=" * 60)
    log("DONE")


run()
