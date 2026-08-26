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
import unreal

MAP = "/Game/Maps/FiveLoops_Stairwell_Blockout"
REGION = os.environ.get("REW_REGION", "Apartment4C_Region")
SHOT = os.environ.get("REW_SHOT", "region")
# A SceneCapture renders one frame, so it has no eye-adaptation history and
# a dark scene comes back black whether or not the lighting is actually wrong.
# REW_EV forces manual exposure so geometry can be judged separately from
# lighting; leave it unset for the exposure the game actually runs.
EV = os.environ.get("REW_EV")
SURFACE_DETAILS = os.environ.get("REW_SURFACE_DETAILS", "maps")
PERSIST_FLAT_DETAILS = os.environ.get("REW_PERSIST_FLAT_DETAILS") == "1"
LOG_SURFACE_BINDINGS = os.environ.get("REW_LOG_SURFACE_BINDINGS") == "1"
W = int(os.environ.get("REW_W", "1600"))
H = int(os.environ.get("REW_H", "900"))
OUT = unreal.Paths.project_saved_dir() + "Screenshots"

SURFACE_MATERIAL_DIR = "/Game/Art/Materials/Surfaces"
SURFACE_NAMES = (
    "BrickDark_Aged", "CeramicTile_Ivory_Aged", "FloorTile_Interior",
    "MetalBlack_Painted_Aged", "PavingStone_Courtyard", "PlasterGray_Aged",
    "PlasterGreen_Aged", "WoodDark_Worn",
)


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


def temporarily_flatten_surface_details():
    """Return transient overrides to reproduce a base-colour-only capture.

    This is capture-only state: it is restored before the commandlet exits and
    is never saved, so named asset bindings remain untouched.
    """
    if SURFACE_DETAILS != "flat":
        return []
    flat_normal = unreal.EditorAssetLibrary.load_asset("/Engine/EngineMaterials/DefaultNormal")
    white = unreal.EditorAssetLibrary.load_asset("/Engine/EngineResources/WhiteSquareTexture")
    if not flat_normal or not white:
        log("FATAL: engine fallback textures unavailable for flat-detail capture")
        return []
    overrides = []
    for surface in SURFACE_NAMES:
        instance = unreal.EditorAssetLibrary.load_asset(f"{SURFACE_MATERIAL_DIR}/MI_REW_{surface}")
        if not instance:
            continue
        normal = unreal.MaterialEditingLibrary.get_material_instance_texture_parameter_value(instance, "NormalTex")
        roughness = unreal.MaterialEditingLibrary.get_material_instance_texture_parameter_value(instance, "RoughnessTex")
        overrides.append((instance, normal, roughness))
        unreal.MaterialEditingLibrary.set_material_instance_texture_parameter_value(instance, "NormalTex", flat_normal)
        unreal.MaterialEditingLibrary.set_material_instance_texture_parameter_value(instance, "RoughnessTex", white)
        unreal.MaterialEditingLibrary.update_material_instance(instance)
    log(f"temporarily flattened {len(overrides)} surface instances for base-colour-only capture")
    if PERSIST_FLAT_DETAILS:
        for instance, _, _ in overrides:
            unreal.EditorAssetLibrary.save_loaded_asset(instance)
        log("saved flat capture state; restore with import_surface_detail_maps.py before normal operation")
        return []
    return overrides


def restore_surface_details(overrides):
    for instance, normal, roughness in overrides:
        unreal.MaterialEditingLibrary.set_material_instance_texture_parameter_value(instance, "NormalTex", normal)
        unreal.MaterialEditingLibrary.set_material_instance_texture_parameter_value(instance, "RoughnessTex", roughness)
        unreal.MaterialEditingLibrary.update_material_instance(instance)
    if overrides:
        log("restored transient surface-detail overrides without saving assets")


def log_surface_bindings(world):
    if not LOG_SURFACE_BINDINGS:
        return
    actors = unreal.get_editor_subsystem(unreal.EditorActorSubsystem).get_all_level_actors()
    count = 0
    for actor in actors:
        for component in actor.get_components_by_class(unreal.StaticMeshComponent):
            for slot in range(component.get_num_materials()):
                material = component.get_material(slot)
                path = material.get_path_name() if material else "None"
                if "/Game/Art/Materials/" in path:
                    log(f"binding actor={actor.get_actor_label()} component={component.get_name()} slot={slot} material={path}")
                    count += 1
    log(f"surface-binding inspection found {count} /Game/Art/Materials/ slots")


def run():
    unreal.EditorLoadingAndSavingUtils.load_map(MAP)
    world = get_world()
    if not world:
        log("FATAL: no editor world")
        return
    log_surface_bindings(world)

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

    overrides = temporarily_flatten_surface_details()
    try:
        comp.capture_scene()
        unreal.RenderingLibrary.export_render_target(world, rt, OUT, SHOT + ".png")
    finally:
        restore_surface_details(overrides)
        sub.destroy_actor(cap)

    path = os.path.join(OUT, SHOT + ".png")
    if os.path.exists(path):
        log(f"wrote {path} ({os.path.getsize(path)} bytes)")
    else:
        log(f"FAILED: nothing at {path}")
    log("DONE")


run()
