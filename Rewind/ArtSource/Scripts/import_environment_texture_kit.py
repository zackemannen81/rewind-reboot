"""Import and verify RE:WIND Environment Texture Kit v1 surface assets.

Run headless from the repository's UE 5.8 install:
  UnrealEditor-Cmd.exe Rewind.uproject -ExecutePythonScript="<this file>"
  -unattended -nopause -nosplash -NullRHI

The retained source JPEGs, the names, intended use, and import settings are
recorded in ArtSource/Textures/EnvironmentTextureKit-v1/README.md and the
external kit README. This script deliberately creates no map assets and never
edits the M_REW_Surface master.
"""

import unreal


TASK = "[REW-0027]"
SOURCE_DIR = "C:/code/rewind-multiagents/rewind-reboot/Rewind/ArtSource/Textures/EnvironmentTextureKit-v1"
TEXTURE_DEST = "/Game/Art/Textures/Surfaces"
MATERIAL_DEST = "/Game/Art/Materials/Surfaces"
MASTER_PATH = MATERIAL_DEST + "/M_REW_Surface"

# filename, instance suffix, tileable, texture group, dry roughness, tile size cm
TEXTURES = [
    ("T_REW_Surface_PlasterGray_Aged_BC.jpg", "PlasterGray_Aged", True, "world", 0.78, 200.0),
    ("T_REW_Surface_PlasterGreen_Aged_BC.jpg", "PlasterGreen_Aged", True, "world", 0.70, 200.0),
    ("T_REW_Surface_BrickDark_Aged_BC.jpg", "BrickDark_Aged", True, "world", 0.82, 120.0),
    ("T_REW_Surface_PavingStone_Courtyard_BC.jpg", "PavingStone_Courtyard", True, "world", 0.76, 200.0),
    ("T_REW_Surface_FloorTile_Interior_BC.jpg", "FloorTile_Interior", True, "world", 0.68, 100.0),
    ("T_REW_Surface_CeramicTile_Ivory_Aged_BC.jpg", "CeramicTile_Ivory_Aged", True, "world", 0.48, 40.0),
    ("T_REW_Surface_MetalBlack_Painted_Aged_BC.jpg", "MetalBlack_Painted_Aged", True, "world", 0.64, 100.0),
    ("T_REW_Surface_WoodDark_Worn_BC.jpg", "WoodDark_Worn", True, "world", 0.62, 150.0),
    ("T_REW_Backdrop_CityRain_Night_BC.jpg", None, False, "world", None, None),
    ("T_REW_Poster_TransitMap_BC.jpg", None, False, "ui", None, None),
    ("T_REW_Signs_FloorCourtyard_Atlas_BC.jpg", None, False, "ui", None, None),
    ("T_REW_Label_FuseBoxInstructions_BC.jpg", None, False, "ui", None, None),
]


def log(message):
    unreal.log(f"{TASK} {message}")


def asset_name(filename):
    return filename.rsplit(".", 1)[0]


def texture_path(filename):
    return f"{TEXTURE_DEST}/{asset_name(filename)}"


def import_textures():
    tasks = []
    for filename, _, _, _, _, _ in TEXTURES:
        task = unreal.AssetImportTask()
        task.filename = f"{SOURCE_DIR}/{filename}"
        task.destination_path = TEXTURE_DEST
        task.destination_name = asset_name(filename)
        task.automated = True
        task.replace_existing = True
        task.replace_existing_settings = True
        task.save = False
        task.options = unreal.TextureFactory()
        tasks.append(task)
    unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks(tasks)
    for task in tasks:
        log(f"imported {task.filename}: {list(task.imported_object_paths or [])}")


def configure_texture(filename, tileable, group):
    path = texture_path(filename)
    texture = unreal.EditorAssetLibrary.load_asset(path)
    if not texture:
        raise RuntimeError(f"texture missing after import: {path}")
    texture.set_editor_property("srgb", True)
    texture.set_editor_property("compression_settings", unreal.TextureCompressionSettings.TC_DEFAULT)
    texture.set_editor_property(
        "lod_group",
        unreal.TextureGroup.TEXTUREGROUP_WORLD if group == "world" else unreal.TextureGroup.TEXTUREGROUP_UI,
    )
    address = unreal.TextureAddress.TA_WRAP if tileable else unreal.TextureAddress.TA_CLAMP
    texture.set_editor_property("address_x", address)
    texture.set_editor_property("address_y", address)
    texture.set_editor_property("max_texture_size", 1024)
    unreal.EditorAssetLibrary.save_loaded_asset(texture)
    return texture


def load_or_create_instance(name):
    path = f"{MATERIAL_DEST}/{name}"
    if unreal.EditorAssetLibrary.does_asset_exist(path):
        return unreal.EditorAssetLibrary.load_asset(path)
    factory = unreal.MaterialInstanceConstantFactoryNew()
    instance = unreal.AssetToolsHelpers.get_asset_tools().create_asset(
        name, MATERIAL_DEST, unreal.MaterialInstanceConstant, factory
    )
    if not instance:
        raise RuntimeError(f"could not create material instance: {path}")
    return instance


def configure_instances():
    master = unreal.EditorAssetLibrary.load_asset(MASTER_PATH)
    if not master:
        raise RuntimeError(f"master material not found: {MASTER_PATH}")
    for filename, suffix, tileable, _, roughness, tile_size in TEXTURES:
        if not tileable:
            continue
        instance = load_or_create_instance(f"MI_REW_{suffix}")
        texture = unreal.EditorAssetLibrary.load_asset(texture_path(filename))
        instance.set_editor_property("parent", master)
        unreal.MaterialEditingLibrary.set_material_instance_texture_parameter_value(
            instance, "BaseColorTex", texture
        )
        unreal.MaterialEditingLibrary.set_material_instance_scalar_parameter_value(
            instance, "TileSize", tile_size
        )
        unreal.MaterialEditingLibrary.set_material_instance_vector_parameter_value(
            instance, "Tint", unreal.LinearColor(1.0, 1.0, 1.0, 1.0)
        )
        unreal.MaterialEditingLibrary.set_material_instance_scalar_parameter_value(
            instance, "Roughness", roughness
        )
        unreal.EditorAssetLibrary.save_loaded_asset(instance)
        log(f"configured {instance.get_path_name()} BaseColorTex={texture.get_path_name()} TileSize={tile_size}")


def verify():
    errors = []
    for filename, suffix, tileable, group, _, tile_size in TEXTURES:
        path = texture_path(filename)
        exists = unreal.EditorAssetLibrary.does_asset_exist(path)
        texture = unreal.EditorAssetLibrary.load_asset(path) if exists else None
        expected_address = unreal.TextureAddress.TA_WRAP if tileable else unreal.TextureAddress.TA_CLAMP
        if not texture:
            errors.append(f"missing texture {path}")
            continue
        max_size = texture.get_editor_property("max_texture_size")
        srgb = texture.get_editor_property("srgb")
        address_x = texture.get_editor_property("address_x")
        address_y = texture.get_editor_property("address_y")
        lod_group = texture.get_editor_property("lod_group")
        log(f"VERIFY texture={path} exists={exists} sRGB={srgb} MaxTextureSize={max_size} address=({address_x},{address_y}) group={lod_group}")
        if not srgb or max_size != 1024 or address_x != expected_address or address_y != expected_address:
            errors.append(f"incorrect texture settings: {path}")
        expected_group = unreal.TextureGroup.TEXTUREGROUP_WORLD if group == "world" else unreal.TextureGroup.TEXTUREGROUP_UI
        if lod_group != expected_group:
            errors.append(f"incorrect texture group: {path}")
        if tileable:
            instance_path = f"{MATERIAL_DEST}/MI_REW_{suffix}"
            instance = unreal.EditorAssetLibrary.load_asset(instance_path)
            if not instance:
                errors.append(f"missing instance {instance_path}")
                continue
            base_color = unreal.MaterialEditingLibrary.get_material_instance_texture_parameter_value(instance, "BaseColorTex")
            actual_tile_size = unreal.MaterialEditingLibrary.get_material_instance_scalar_parameter_value(instance, "TileSize")
            base_path = base_color.get_path_name() if base_color else "None"
            log(f"VERIFY instance={instance_path} parent={instance.get_editor_property('parent').get_path_name()} BaseColorTex={base_path} TileSize={actual_tile_size}")
            if base_color != texture or abs(actual_tile_size - tile_size) > 0.001:
                errors.append(f"incorrect instance values: {instance_path}")
    if errors:
        raise RuntimeError("; ".join(errors))
    log("VERIFY PASS: 12 textures and 8 material instances read back from saved assets")


def main():
    log("starting import; no map asset is opened or modified")
    import_textures()
    for filename, _, tileable, group, _, _ in TEXTURES:
        configure_texture(filename, tileable, group)
    configure_instances()
    verify()
    log("DONE")


main()
