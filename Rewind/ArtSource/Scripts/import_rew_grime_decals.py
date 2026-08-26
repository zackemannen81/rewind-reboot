"""Import RE:WIND's decal kit and make one material instance per atlas cell.

Run headless from the repository root:

    & 'C:/Program Files/Epic Games/UE_5.8/Engine/Binaries/Win64/UnrealEditor-Cmd.exe' \
      'C:/code/rewind-multiagents/rewind-reboot2/Rewind/Rewind.uproject' \
      -ExecutePythonScript='C:/code/rewind-multiagents/rewind-reboot2/Rewind/ArtSource/Scripts/import_rew_grime_decals.py' \
      -unattended -nopause -nosplash -NullRHI

The source files are committed under ArtSource so this does not depend on the
operator's Desktop kit after the first import.  Re-running safely replaces the
six textures and updates the sixteen named instances; it never opens a map,
places actors, or changes M_REW_GrimeDecal.
"""
import os
import unreal


ROOT = os.path.normpath(os.path.join(os.path.dirname(__file__), "..", "Textures",
                                    "EnvironmentTextureKit-v1", "Decals_Emissive"))
TEXTURE_DESTINATION = "/Game/Art/Textures/Decals"
INSTANCE_DESTINATION = "/Game/Art/Materials/Decals/Instances"
MASTER_PATH = "/Game/Art/Materials/Decals/M_REW_GrimeDecal"

MASKS = (
    ("D_REW_PeelingPlaster_Atlas_Mask.png", "PeelingPlaster", 2, 2,
     unreal.LinearColor(0.075, 0.060, 0.040, 1.0)),
    ("D_REW_LeaksRust_Atlas_Mask.png", "LeaksRust", 3, 2,
     unreal.LinearColor(0.150, 0.055, 0.018, 1.0)),
    ("D_REW_WallCracks_Atlas_Mask.png", "WallCracks", 3, 2,
     unreal.LinearColor(0.018, 0.016, 0.014, 1.0)),
)

NEONS = (
    "T_REW_Neon_4C_Emissive.png",
    "T_REW_Neon_REWIND_Emissive.png",
    "T_REW_Neon_TRANSIT_Emissive.png",
)


def log(message):
    unreal.log("[REW-0028] " + str(message))


def import_texture(filename):
    """Import one tracked source PNG and return its loaded texture asset."""
    task = unreal.AssetImportTask()
    task.filename = os.path.join(ROOT, filename)
    task.destination_path = TEXTURE_DESTINATION
    task.destination_name = os.path.splitext(filename)[0]
    task.automated = True
    task.replace_existing = True
    task.save = True
    unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks([task])

    texture_path = TEXTURE_DESTINATION + "/" + task.destination_name
    texture = unreal.EditorAssetLibrary.load_asset(texture_path)
    if not texture:
        raise RuntimeError("Texture import did not create " + texture_path)
    return texture


def configure_mask(texture):
    texture.set_editor_property("srgb", False)
    texture.set_editor_property("compression_settings",
                                unreal.TextureCompressionSettings.TC_MASKS)
    texture.set_editor_property("address_x", unreal.TextureAddress.TA_CLAMP)
    texture.set_editor_property("address_y", unreal.TextureAddress.TA_CLAMP)
    unreal.EditorAssetLibrary.save_loaded_asset(texture)


def configure_neon(texture):
    texture.set_editor_property("srgb", True)
    texture.set_editor_property("compression_settings",
                                unreal.TextureCompressionSettings.TC_DEFAULT)
    # Explicitly keep alpha during compression: emissive signs use it as opacity.
    texture.set_editor_property("compression_no_alpha", False)
    texture.set_editor_property("address_x", unreal.TextureAddress.TA_CLAMP)
    texture.set_editor_property("address_y", unreal.TextureAddress.TA_CLAMP)
    unreal.EditorAssetLibrary.save_loaded_asset(texture)


def cell_uv(column, row, columns, rows):
    return (unreal.LinearColor(1.0 / columns, 1.0 / rows, 0.0, 0.0),
            unreal.LinearColor(column / columns, row / rows, 0.0, 0.0))


def get_or_create_instance(name):
    path = INSTANCE_DESTINATION + "/" + name
    instance = unreal.EditorAssetLibrary.load_asset(path)
    if instance:
        return instance
    factory = unreal.MaterialInstanceConstantFactoryNew()
    instance = unreal.AssetToolsHelpers.get_asset_tools().create_asset(
        name, INSTANCE_DESTINATION, unreal.MaterialInstanceConstant, factory)
    if not instance:
        raise RuntimeError("Could not create " + path)
    return instance


def configure_instance(master, texture, kind, number, columns, rows, tint):
    """Set every per-cell override required for immediate decal use."""
    instance = get_or_create_instance("MI_REW_Decal_{}{}".format(kind, number))
    unreal.MaterialEditingLibrary.set_material_instance_parent(instance, master)
    column = (number - 1) % columns
    row = (number - 1) // columns
    uv_scale, uv_offset = cell_uv(column, row, columns, rows)
    unreal.MaterialEditingLibrary.set_material_instance_texture_parameter_value(
        instance, "Mask", texture)
    unreal.MaterialEditingLibrary.set_material_instance_vector_parameter_value(
        instance, "UVScale", uv_scale)
    unreal.MaterialEditingLibrary.set_material_instance_vector_parameter_value(
        instance, "UVOffset", uv_offset)
    unreal.MaterialEditingLibrary.set_material_instance_vector_parameter_value(
        instance, "Tint", tint)
    # A dark, mostly rough pass keeps decals as stains in RE:WIND's near-black
    # rooms instead of turning the white source masks into light patches.
    unreal.MaterialEditingLibrary.set_material_instance_scalar_parameter_value(
        instance, "Opacity", 0.72)
    unreal.MaterialEditingLibrary.set_material_instance_scalar_parameter_value(
        instance, "Roughness", 0.88)
    unreal.EditorAssetLibrary.save_loaded_asset(instance)
    log("{}: Mask={} UVScale={} UVOffset={} Tint={}".format(
        instance.get_name(), texture.get_path_name(), uv_scale, uv_offset, tint))


def close_enough(a, b):
    return abs(a - b) < 0.0001


def verify_instance(master, texture, kind, number, columns, rows):
    """Read back the required persisted parent and cell parameter overrides."""
    path = INSTANCE_DESTINATION + "/MI_REW_Decal_{}{}".format(kind, number)
    instance = unreal.EditorAssetLibrary.load_asset(path)
    if not instance:
        raise RuntimeError("Missing expected instance " + path)
    if instance.get_editor_property("parent").get_path_name() != master.get_path_name():
        raise RuntimeError(path + " has the wrong parent")
    expected_scale, expected_offset = cell_uv((number - 1) % columns,
                                              (number - 1) // columns,
                                              columns, rows)
    actual_mask = unreal.MaterialEditingLibrary.get_material_instance_texture_parameter_value(
        instance, "Mask")
    actual_scale = unreal.MaterialEditingLibrary.get_material_instance_vector_parameter_value(
        instance, "UVScale")
    actual_offset = unreal.MaterialEditingLibrary.get_material_instance_vector_parameter_value(
        instance, "UVOffset")
    if not actual_mask or actual_mask.get_path_name() != texture.get_path_name():
        raise RuntimeError(path + " has the wrong Mask")
    if not (close_enough(actual_scale.r, expected_scale.r) and
            close_enough(actual_scale.g, expected_scale.g) and
            close_enough(actual_offset.r, expected_offset.r) and
            close_enough(actual_offset.g, expected_offset.g)):
        raise RuntimeError(path + " has the wrong UVScale or UVOffset")
    log("verified {} parent, Mask, UVScale and UVOffset".format(instance.get_name()))


def describe_texture(texture, label):
    log("{}: srgb={} compression={} address_x={} address_y={} compression_no_alpha={}".format(
        label,
        texture.get_editor_property("srgb"),
        texture.get_editor_property("compression_settings"),
        texture.get_editor_property("address_x"),
        texture.get_editor_property("address_y"),
        texture.get_editor_property("compression_no_alpha")))


def run():
    master = unreal.EditorAssetLibrary.load_asset(MASTER_PATH)
    if not master:
        raise RuntimeError("Required master material was not found: " + MASTER_PATH)

    mask_assets = []
    for filename, kind, columns, rows, tint in MASKS:
        texture = import_texture(filename)
        configure_mask(texture)
        describe_texture(texture, filename)
        mask_assets.append((texture, kind, columns, rows, tint))

    for filename in NEONS:
        texture = import_texture(filename)
        configure_neon(texture)
        describe_texture(texture, filename)

    made = 0
    for texture, kind, columns, rows, tint in mask_assets:
        for number in range(1, columns * rows + 1):
            configure_instance(master, texture, kind, number, columns, rows, tint)
            made += 1

    verified = 0
    for texture, kind, columns, rows, _tint in mask_assets:
        for number in range(1, columns * rows + 1):
            verify_instance(master, texture, kind, number, columns, rows)
            verified += 1

    unreal.EditorAssetLibrary.save_directory(TEXTURE_DESTINATION, recursive=True)
    unreal.EditorAssetLibrary.save_directory(INSTANCE_DESTINATION, recursive=True)
    log("DONE: imported 6 textures, configured {} decal instances, and verified {}".format(
        made, verified))


run()
