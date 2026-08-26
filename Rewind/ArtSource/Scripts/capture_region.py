"""Render a RewindCameraRegion's authored shot to a PNG, without opening the editor.

Every look-and-feel judgement so far has been made from coordinates in a log,
which is how a room ends up geometrically correct and visually wrong. This
places a SceneCapture2D exactly where the region's own CameraOffset and
FieldOfView put the game camera, so what lands on disk is the shot the player
gets -- not an editor perspective that happens to point the same way.

Set REW_REGION to pick a region (default Apartment4C_Region) and REW_SHOT to
name the file. Output goes to Saved/Screenshots/.

Run WITHOUT -NullRHI; there is nothing to capture with a null renderer:
    UnrealEditor-Cmd.exe <uproject> -ExecutePythonScript="<this file>"
      -unattended -nopause -nosplash
"""
import os
import time
import unreal

MAP = "/Game/Maps/FiveLoops_Stairwell_Blockout"
REGION = os.environ.get("REW_REGION", "Apartment4C_Region")
SHOT = os.environ.get("REW_SHOT", "region")
# A SceneCapture renders one frame, so it has no eye-adaptation history and
# a dark scene comes back black whether or not the lighting is actually wrong.
# REW_EV forces manual exposure so geometry can be judged separately from
# lighting; leave it unset for the exposure the game actually runs.
EV = os.environ.get("REW_EV")
# REW_STANDIN=1 drops the player mesh at the PlayerStart before capturing.
# A room shot with nothing human in it says nothing about whether the camera
# distance is playable -- the whole point of a fixed camera is how the player
# reads inside it, and this camera moved from 687 to 1972.
STANDIN = os.environ.get("REW_STANDIN") == "1"
STANDIN_MESH = "/Game/Characters/Returner/Returner"
STANDIN_AT = "Stairwell_PlayerStart"
W = int(os.environ.get("REW_W", "1600"))
H = int(os.environ.get("REW_H", "900"))
# Default non-zero on purpose. A SceneCapture taken before async shader
# compilation finishes renders fallback materials, so back-to-back captures of
# the SAME state can differ. A zero default is a trap: it makes every capture
# look like evidence while some of them are not. This cost the project a whole
# investigation -- a material-swap bisect that captured immediately after
# swapping .uasset files, and drew confident conclusions from noise.
# Set REW_WARMUP_SECONDS=0 only when you know the shaders are already warm.
WARMUP_SECONDS = float(os.environ.get("REW_WARMUP_SECONDS", "45"))
OUT = unreal.Paths.project_saved_dir() + "Screenshots"


def log(m):
    unreal.log("[REW-shot] " + str(m))


def get_world():
    for getter in (
            lambda: unreal.get_editor_subsystem(
                unreal.UnrealEditorSubsystem).get_editor_world(),
            lambda: unreal.EditorLevelLibrary.get_editor_world()):
        try:
            w = getter()
            if w:
                return w
        except Exception:
            pass
    return None


def run():
    unreal.EditorLoadingAndSavingUtils.load_map(MAP)
    if WARMUP_SECONDS > 0:
        log(f"waiting {WARMUP_SECONDS:.1f}s for async shader compilation before capture")
        time.sleep(WARMUP_SECONDS)
    world = get_world()
    if not world:
        log("FATAL: no editor world")
        return

    sub = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    region = None
    for a in sub.get_all_level_actors():
        if a.get_actor_label() == REGION:
            region = a
            break
    if not region:
        log(f"FATAL: no region {REGION}")
        return

    centre = region.get_actor_location()
    offset = region.get_editor_property("CameraOffset")
    fov = float(region.get_editor_property("FieldOfView"))
    eye = unreal.Vector(centre.x + offset.x, centre.y + offset.y,
                        centre.z + offset.z)
    look = unreal.MathLibrary.find_look_at_rotation(eye, centre)
    log(f"{REGION}: eye=({eye.x:.0f},{eye.y:.0f},{eye.z:.0f}) "
        f"aim=({centre.x:.0f},{centre.y:.0f},{centre.z:.0f}) fov={fov:.2f}")

    # RTF_RGBA8 on purpose. The default target is float, and export_render_target
    # then writes an EXR regardless of the filename you give it -- a .png that
    # no image viewer will open.
    stand = None
    if STANDIN:
        mesh = unreal.EditorAssetLibrary.load_asset(STANDIN_MESH)
        start = None
        for a in sub.get_all_level_actors():
            if a.get_actor_label() == STANDIN_AT:
                start = a
                break
        if mesh and start:
            loc = start.get_actor_location()
            # The capsule pivot sits 96 above the feet; the mesh is authored
            # with its origin at the feet, so drop it to stand on the floor.
            stand = sub.spawn_actor_from_class(
                unreal.SkeletalMeshActor,
                unreal.Vector(loc.x, loc.y, loc.z - 96.0),
                unreal.Rotator(0.0, 0.0, -90.0))
            if stand:
                stand.skeletal_mesh_component.set_skeletal_mesh(mesh)
                o, e = stand.get_actor_bounds(False)
                log(f"stand-in at ({loc.x:.0f},{loc.y:.0f},{loc.z - 96:.0f}), "
                    f"{e.z * 2:.0f} cm tall")
        else:
            log(f"stand-in skipped: mesh={bool(mesh)} start={bool(start)}")

    rt = unreal.RenderingLibrary.create_render_target2d(
        world, W, H, unreal.TextureRenderTargetFormat.RTF_RGBA8)
    if not rt:
        log("FATAL: could not create render target")
        return

    cap = sub.spawn_actor_from_class(unreal.SceneCapture2D, eye, look)
    comp = cap.capture_component2d
    comp.set_editor_property("texture_target", rt)
    comp.set_editor_property("fov_angle", fov)
    comp.set_editor_property(
        "capture_source", unreal.SceneCaptureSource.SCS_FINAL_COLOR_LDR)
    # Optional switches: names differ between engine versions, and none of
    # them is worth losing the capture over.
    for prop, val in (("capture_every_frame", False),
                      ("capture_on_movement", False),
                      ("always_persist_rendering_state", True)):
        try:
            comp.set_editor_property(prop, val)
        except Exception:
            pass
    # Without this the capture inherits none of the level's post processing and
    # the shot comes back looking nothing like the game.
    try:
        st = comp.get_editor_property("show_flags")
        st.set_editor_property("post_processing", True)
    except Exception:
        pass

    if EV is not None:
        pp = unreal.PostProcessSettings()
        pp.set_editor_property("override_auto_exposure_method", True)
        pp.set_editor_property(
            "auto_exposure_method", unreal.AutoExposureMethod.AEM_MANUAL)
        pp.set_editor_property("override_auto_exposure_bias", True)
        pp.set_editor_property("auto_exposure_bias", float(EV))
        comp.set_editor_property("post_process_settings", pp)
        log(f"manual exposure, bias {float(EV):+.1f} EV")

    comp.capture_scene()
    unreal.RenderingLibrary.export_render_target(world, rt, OUT, SHOT + ".png")
    sub.destroy_actor(cap)
    if stand:
        # Never saved: this is a measuring stick, not level content.
        sub.destroy_actor(stand)

    path = os.path.join(OUT, SHOT + ".png")
    if os.path.exists(path):
        log(f"wrote {path} ({os.path.getsize(path)} bytes)")
    else:
        log(f"FAILED: nothing at {path}")
    log("DONE")


run()
