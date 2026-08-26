"""Import and bind the REW-0031 derived surface normal and roughness maps.

First generate sources with generate_surface_detail_maps.py, then run headless:
  UnrealEditor-Cmd.exe <absolute Rewind.uproject path>
    -ExecutePythonScript=<absolute script path> -unattended -nopause -nosplash

This script edits only /Game/Art/Textures/Surfaces and
/Game/Art/Materials/Surfaces.  It creates no map assets and never saves a map.
"""

import os
import unreal


TASK = "[REW-0031]"
TEXTURE_DEST = "/Game/Art/Textures/Surfaces"
MATERIAL_DEST = "/Game/Art/Materials/Surfaces"
MASTER_PATH = MATERIAL_DEST + "/M_REW_Surface"
SOURCE_DIR = os.path.join(
    unreal.Paths.convert_relative_path_to_full(unreal.Paths.project_dir()),
    "ArtSource", "Textures", "EnvironmentTextureKit-v1", "Derived_REW-0031",
).replace(chr(92), "/")
SURFACES = (
    "BrickDark_Aged", "CeramicTile_Ivory_Aged", "FloorTile_Interior",
    "MetalBlack_Painted_Aged", "PavingStone_Courtyard", "PlasterGray_Aged",
    "PlasterGreen_Aged", "WoodDark_Worn",
)


def log(message):
    unreal.log(f"{TASK} {message}")


def texture_path(surface, suffix):
    return f"{TEXTURE_DEST}/T_REW_Surface_{surface}_{suffix}"


def load(path):
    asset = unreal.EditorAssetLibrary.load_asset(path)
    if not asset:
        raise RuntimeError(f"missing required asset: {path}")
    return asset


def import_derived_textures():
    tasks = []
    for surface in SURFACES:
        for suffix in ("N", "R"):
            name = f"T_REW_Surface_{surface}_{suffix}"
            filename = f"{SOURCE_DIR}/{name}.png"
            if not os.path.isfile(filename):
                raise RuntimeError(f"generate source before importing: {filename}")
            task = unreal.AssetImportTask()
            task.filename = filename
            task.destination_path = TEXTURE_DEST
            task.destination_name = name
            task.automated = True
            task.replace_existing = True
            task.replace_existing_settings = True
            task.save = False
            task.options = unreal.TextureFactory()
            tasks.append(task)
    unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks(tasks)
    for task in tasks:
        log(f"imported {task.filename}: {list(task.imported_object_paths or [])}")


def configure_derived_textures():
    for surface in SURFACES:
        for suffix in ("N", "R"):
            texture = load(texture_path(surface, suffix))
            texture.set_editor_property("srgb", False)
            texture.set_editor_property("address_x", unreal.TextureAddress.TA_WRAP)
            texture.set_editor_property("address_y", unreal.TextureAddress.TA_WRAP)
            texture.set_editor_property("lod_group", unreal.TextureGroup.TEXTUREGROUP_WORLD)
            texture.set_editor_property("max_texture_size", 2048)
            compression = (unreal.TextureCompressionSettings.TC_NORMALMAP
                           if suffix == "N" else unreal.TextureCompressionSettings.TC_MASKS)
            texture.set_editor_property("compression_settings", compression)
            unreal.EditorAssetLibrary.save_loaded_asset(texture)


def expression(material, expression_class, x, y):
    return unreal.MaterialEditingLibrary.create_material_expression(material, expression_class, x, y)


def parameter_texture(material, name, texture, sampler_type, x, y):
    node = expression(material, unreal.MaterialExpressionTextureSampleParameter2D, x, y)
    node.set_editor_property("parameter_name", name)
    node.set_editor_property("texture", texture)
    node.set_editor_property("sampler_type", sampler_type)
    return node


def wire(material, source, source_output, target, target_input):
    unreal.MaterialEditingLibrary.connect_material_expressions(source, source_output, target, target_input)


def rebuild_surface_master():
    material = load(MASTER_PATH)
    unreal.MaterialEditingLibrary.delete_all_material_expressions(material)

    # World-position XY divided by TileSize is the shared coordinate source for
    # all three map types, preserving REW-0027's world-scaled tiling behaviour.
    world_position = expression(material, unreal.MaterialExpressionWorldPosition, -1100, 0)
    xy = expression(material, unreal.MaterialExpressionComponentMask, -900, 0)
    xy.set_editor_property("r", True)
    xy.set_editor_property("g", True)
    tile_size = expression(material, unreal.MaterialExpressionScalarParameter, -900, 170)
    tile_size.set_editor_property("parameter_name", "TileSize")
    tile_size.set_editor_property("default_value", 100.0)
    uv = expression(material, unreal.MaterialExpressionDivide, -680, 0)
    wire(material, world_position, "", xy, "Input")
    wire(material, xy, "", uv, "A")
    wire(material, tile_size, "", uv, "B")

    base = parameter_texture(
        material, "BaseColorTex", load(texture_path("PlasterGray_Aged", "BC")),
        unreal.MaterialSamplerType.SAMPLERTYPE_COLOR, -420, -260)
    normal = parameter_texture(
        material, "NormalTex", load(texture_path("PlasterGray_Aged", "N")),
        unreal.MaterialSamplerType.SAMPLERTYPE_NORMAL, -420, 0)
    roughness_tex = parameter_texture(
        material, "RoughnessTex", load(texture_path("PlasterGray_Aged", "R")),
        unreal.MaterialSamplerType.SAMPLERTYPE_LINEAR_GRAYSCALE, -420, 250)
    tint = expression(material, unreal.MaterialExpressionVectorParameter, -160, -360)
    tint.set_editor_property("parameter_name", "Tint")
    tint.set_editor_property("default_value", unreal.LinearColor(1.0, 1.0, 1.0, 1.0))
    roughness_scalar = expression(material, unreal.MaterialExpressionScalarParameter, -160, 390)
    roughness_scalar.set_editor_property("parameter_name", "Roughness")
    roughness_scalar.set_editor_property("default_value", 0.7)
    colour = expression(material, unreal.MaterialExpressionMultiply, 60, -250)
    roughness = expression(material, unreal.MaterialExpressionMultiply, 60, 260)

    for sample in (base, normal, roughness_tex):
        wire(material, uv, "", sample, "Coordinates")
    wire(material, base, "RGB", colour, "A")
    wire(material, tint, "", colour, "B")
    wire(material, roughness_tex, "R", roughness, "A")
    wire(material, roughness_scalar, "", roughness, "B")
    unreal.MaterialEditingLibrary.connect_material_property(
        colour, "", unreal.MaterialProperty.MP_BASE_COLOR)
    unreal.MaterialEditingLibrary.connect_material_property(
        normal, "RGB", unreal.MaterialProperty.MP_NORMAL)
    unreal.MaterialEditingLibrary.connect_material_property(
        roughness, "", unreal.MaterialProperty.MP_ROUGHNESS)
    unreal.MaterialEditingLibrary.recompile_material(material)
    unreal.EditorAssetLibrary.save_loaded_asset(material)
    return material


def bind_instances(master):
    for surface in SURFACES:
        instance = load(f"{MATERIAL_DEST}/MI_REW_{surface}")
        instance.set_editor_property("parent", master)
        unreal.MaterialEditingLibrary.set_material_instance_texture_parameter_value(
            instance, "NormalTex", load(texture_path(surface, "N")))
        unreal.MaterialEditingLibrary.set_material_instance_texture_parameter_value(
            instance, "RoughnessTex", load(texture_path(surface, "R")))
        unreal.EditorAssetLibrary.save_loaded_asset(instance)


def verify():
    errors = []
    master = load(MASTER_PATH)
    for parameter in ("BaseColorTex", "NormalTex", "RoughnessTex"):
        value = unreal.MaterialEditingLibrary.get_material_default_texture_parameter_value(master, parameter)
        path = value.get_path_name() if value else "None"
        log(f"VERIFY master parameter={parameter} default={path}")
        if not value:
            errors.append(f"master is missing texture parameter: {parameter}")
    for surface in SURFACES:
        expected = {}
        for suffix, compression in (("N", unreal.TextureCompressionSettings.TC_NORMALMAP),
                                    ("R", unreal.TextureCompressionSettings.TC_MASKS)):
            texture = load(texture_path(surface, suffix))
            srgb = texture.get_editor_property("srgb")
            actual_compression = texture.get_editor_property("compression_settings")
            size = texture.get_editor_property("max_texture_size")
            address_x = texture.get_editor_property("address_x")
            address_y = texture.get_editor_property("address_y")
            path = texture.get_path_name()
            expected[suffix] = texture
            log(f"VERIFY texture={path} sRGB={srgb} compression={actual_compression} max={size} address=({address_x},{address_y})")
            if srgb or actual_compression != compression or size > 2048:
                errors.append(f"texture settings incorrect: {path}")
            if address_x != unreal.TextureAddress.TA_WRAP or address_y != unreal.TextureAddress.TA_WRAP:
                errors.append(f"texture address incorrect: {path}")
        instance = load(f"{MATERIAL_DEST}/MI_REW_{surface}")
        normal = unreal.MaterialEditingLibrary.get_material_instance_texture_parameter_value(instance, "NormalTex")
        roughness = unreal.MaterialEditingLibrary.get_material_instance_texture_parameter_value(instance, "RoughnessTex")
        normal_path = normal.get_path_name() if normal else "None"
        roughness_path = roughness.get_path_name() if roughness else "None"
        log(f"VERIFY instance={instance.get_path_name()} NormalTex={normal_path} RoughnessTex={roughness_path}")
        if normal != expected["N"] or roughness != expected["R"]:
            errors.append(f"instance parameters incorrect: {instance.get_path_name()}")
    if errors:
        raise RuntimeError("; ".join(errors))
    log("VERIFY PASS: 16 linear maps and 8 explicit per-instance parameter bindings read back")


def main():
    log("starting; no map asset will be opened or modified")
    import_derived_textures()
    configure_derived_textures()
    master = rebuild_surface_master()
    bind_instances(master)
    verify()
    log("DONE")


main()
