"""Capture the REW-0039 Apartment 4C comparison camera without saving the map.

The accepted runtime camera is approximately (1969.78, 1300, 1390), rotation
(0, 180, 0), horizontal FOV 37.5. This script fixes those values directly so
the comparison does not drift with the region centre or an editor viewport.

Environment variables:
    REW0039_SHOT       output basename (default REW-0039_exact-camera)
    REW0039_W/H        output size (default 1600 x 900)
    REW0039_WARMUP     shader warmup seconds (default 45)
    REW0039_STANDIN    1 to pose the Returner at PlayerStart (default 1)

Run without -NullRHI.
"""
import os
import time
import unreal


MAP = "/Game/Maps/FiveLoops_Stairwell_Blockout"
CAMERA = unreal.Vector(1969.78, 1300.0, 1390.0)
CAMERA_TARGET = unreal.Vector(-30.22, 1300.0, 1390.0)
FOV = 37.5
SHOT = os.environ.get("REW0039_SHOT", "REW-0039_exact-camera")
WIDTH = int(os.environ.get("REW0039_W", "1600"))
HEIGHT = int(os.environ.get("REW0039_H", "900"))
WARMUP = float(os.environ.get("REW0039_WARMUP", "45"))
STANDIN = os.environ.get("REW0039_STANDIN", "1") == "1"
OUT_DIR = unreal.Paths.project_saved_dir() + "Screenshots"


def log(message):
    unreal.log("[REW-0039-shot] " + str(message))


def editor_world():
    return unreal.get_editor_subsystem(
        unreal.UnrealEditorSubsystem).get_editor_world()


def find_actor(subsystem, label):
    for actor in subsystem.get_all_level_actors():
        if actor.get_actor_label() == label:
            return actor
    return None


def spawn_returner(subsystem):
    if not STANDIN:
        return None
    mesh = unreal.EditorAssetLibrary.load_asset(
        "/Game/Characters/Returner/Returner")
    idle = unreal.EditorAssetLibrary.load_asset(
        "/Game/Characters/Returner/A_Returner_Alert")
    start = find_actor(subsystem, "Stairwell_PlayerStart")
    if not mesh or not start:
        log(f"stand-in unavailable: mesh={bool(mesh)} start={bool(start)}")
        return None
    capsule_location = start.get_actor_location()
    actor = subsystem.spawn_actor_from_class(
        unreal.SkeletalMeshActor,
        unreal.Vector(capsule_location.x, capsule_location.y,
                      capsule_location.z - 96.0),
        unreal.Rotator(0.0, 0.0, -90.0))
    if not actor:
        return None
    component = actor.skeletal_mesh_component
    component.set_skeletal_mesh(mesh)
    if idle:
        component.play_animation(idle, True)
        component.tick_animation(0.5, False)
        component.refresh_bone_transforms()
    log("Returner stand-in at "
        f"({capsule_location.x:.2f},{capsule_location.y:.2f},"
        f"{capsule_location.z - 96.0:.2f})")
    return actor


def run():
    unreal.EditorLoadingAndSavingUtils.load_map(MAP)
    if WARMUP > 0:
        log(f"waiting {WARMUP:.1f}s for shader warmup")
        time.sleep(WARMUP)

    world = editor_world()
    subsystem = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    standin = spawn_returner(subsystem)
    target = unreal.RenderingLibrary.create_render_target2d(
        world, WIDTH, HEIGHT, unreal.TextureRenderTargetFormat.RTF_RGBA8)
    rotation = unreal.MathLibrary.find_look_at_rotation(CAMERA, CAMERA_TARGET)
    capture = subsystem.spawn_actor_from_class(
        unreal.SceneCapture2D, CAMERA, rotation)
    component = capture.capture_component2d
    component.set_editor_property("texture_target", target)
    component.set_editor_property("fov_angle", FOV)
    component.set_editor_property(
        "capture_source", unreal.SceneCaptureSource.SCS_FINAL_COLOR_LDR)
    for prop, value in (("capture_every_frame", False),
                        ("capture_on_movement", False),
                        ("always_persist_rendering_state", True)):
        try:
            component.set_editor_property(prop, value)
        except Exception:
            pass
    component.capture_scene()
    unreal.RenderingLibrary.export_render_target(
        world, target, OUT_DIR, SHOT + ".png")
    subsystem.destroy_actor(capture)
    if standin:
        subsystem.destroy_actor(standin)

    path = os.path.join(OUT_DIR, SHOT + ".png")
    log(f"camera=({CAMERA.x:.2f},{CAMERA.y:.2f},{CAMERA.z:.2f}) "
        f"rotation=(0.00,180.00,0.00) fov={FOV:.2f}")
    if os.path.exists(path):
        log(f"wrote {path} ({os.path.getsize(path)} bytes)")
    else:
        log(f"FAILED: no output at {path}")
    log("DONE")


run()
