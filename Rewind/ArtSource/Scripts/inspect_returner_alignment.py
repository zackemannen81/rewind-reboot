"""Report the imported Returner's bounds and reference-pose alignment data.

Run headless:
    UnrealEditor-Cmd.exe <uproject> -ExecutePythonScript="<this file>"
      -unattended -nopause -nosplash -NullRHI

The output is evidence for the presentation transform in ARewindCharacter;
the capsule remains authoritative for collision.
"""
import unreal


MESH = "/Game/Characters/Returner/Returner"


def log(message):
    unreal.log("[REW-0029] " + str(message))


def try_value(label, callback):
    try:
        value = callback()
        log(f"{label}: {value}")
        return value
    except Exception as error:
        log(f"{label}: unavailable ({error})")
        return None


def run():
    mesh = unreal.EditorAssetLibrary.load_asset(MESH)
    if not mesh:
        log("FATAL: Returner mesh is not present at " + MESH)
        return

    log("mesh: " + MESH)
    try_value("mesh bounds", mesh.get_bounds)

    skeleton = try_value("mesh skeleton", lambda: mesh.get_editor_property("skeleton"))
    if skeleton is None:
        log("FATAL: Returner has no readable skeleton")
        return
    reference_pose = try_value("skeleton reference pose", skeleton.get_reference_pose)
    if reference_pose is None:
        log("FATAL: the editor did not expose the skeleton reference pose")
        return
    names = try_value("reference pose bone names", reference_pose.get_bone_names)
    if names is None:
        log("FATAL: the reference pose has no readable bone names")
        return
    names = [str(name) for name in names]
    log(f"reference bone count: {len(names)}")
    local_poses = {}
    parents = {}
    for name in names:
        local_poses[name] = reference_pose.get_ref_bone_pose(name)
        parent = mesh.get_bone_parent(name)
        parent = str(parent) if parent is not None else None
        parents[name] = None if parent in (None, "", "None") else parent
        log(f"reference pose {name}: {local_poses[name]}")
        log(f"reference parent {name}: {parents[name]}")

    component_poses = {}

    def component_pose(name):
        if name not in component_poses:
            parent = parents[name]
            component_poses[name] = (local_poses[name] if parent is None
                                     else local_poses[name] * component_pose(parent))
        return component_poses[name]

    head = component_pose("Head")
    head_front = component_pose("headfront")
    forward = head_front.translation - head.translation
    log(f"component reference Head: {head}")
    log(f"component reference headfront: {head_front}")
    log(f"reference-pose head-to-front vector: {forward}")

    materials = try_value("material slots", lambda: mesh.get_editor_property("materials"))
    if materials is not None:
        log(f"material slot count: {len(materials)}")
    log("DONE")


run()
