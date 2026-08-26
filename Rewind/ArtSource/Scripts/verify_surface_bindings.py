"""Read back REW-0032 saved surface-detail assets without modifying a map."""

import unreal


TEXTURE_DIR = "/Game/Art/Textures/Surfaces"
MATERIAL_DIR = "/Game/Art/Materials/Surfaces"
MASTER_PATH = MATERIAL_DIR + "/M_REW_Surface"
WORLD_ALIGNED_TEXTURE = (
    "/Engine/Functions/Engine_MaterialFunctions01/Texturing/WorldAlignedTexture.WorldAlignedTexture")
SURFACES = (
    "BrickDark_Aged", "CeramicTile_Ivory_Aged", "FloorTile_Interior",
    "MetalBlack_Painted_Aged", "PavingStone_Courtyard", "PlasterGray_Aged",
    "PlasterGreen_Aged", "WoodDark_Worn",
)


def log(message):
    unreal.log("[REW-0032 verify] " + str(message))


def load(path):
    value = unreal.EditorAssetLibrary.load_asset(path)
    if not value:
        raise RuntimeError("missing: " + path)
    return value


def detail_path(surface, suffix):
    return f"{TEXTURE_DIR}/T_REW_Surface_{surface}_{suffix}"


def run():
    lib = unreal.MaterialEditingLibrary
    master = load(MASTER_PATH)
    texture_names = {str(name) for name in lib.get_texture_parameter_names(master)}
    vector_names = {str(name) for name in lib.get_vector_parameter_names(master)}
    if texture_names != {"BaseColorTex", "NormalTex", "RoughnessTex"}:
        raise RuntimeError("master texture parameter contract changed: " + str(texture_names))
    if vector_names != {"TileSize", "Tint"}:
        raise RuntimeError("TileSize must remain a vector: " + str(vector_names))
    nodes = lib.get_material_expressions(master)
    texture_calls = [node for node in nodes
                     if node.get_class().get_name() == "MaterialExpressionMaterialFunctionCall"
                     and node.get_editor_property("material_function").get_path_name() == WORLD_ALIGNED_TEXTURE]
    normal_nodes = [node for node in nodes
                    if node.get_class().get_name() == "MaterialExpressionTextureSampleParameter2D"
                    and str(node.get_editor_property("parameter_name")) == "NormalTex"]
    if len(texture_calls) != 2 or len(normal_nodes) != 1:
        raise RuntimeError("expected base/roughness world alignment plus one tangent normal sample")
    if normal_nodes[0].get_editor_property("sampler_type") != unreal.MaterialSamplerType.SAMPLERTYPE_NORMAL:
        raise RuntimeError("NormalTex sampler is not SAMPLERTYPE_NORMAL")
    for surface in SURFACES:
        instance = load(f"{MATERIAL_DIR}/MI_REW_{surface}")
        for suffix, compression in (("N", unreal.TextureCompressionSettings.TC_NORMALMAP),
                                    ("R", unreal.TextureCompressionSettings.TC_MASKS)):
            texture = load(detail_path(surface, suffix))
            if (texture.get_editor_property("srgb") or
                    texture.get_editor_property("compression_settings") != compression or
                    texture.get_editor_property("address_x") != unreal.TextureAddress.TA_WRAP or
                    texture.get_editor_property("address_y") != unreal.TextureAddress.TA_WRAP):
                raise RuntimeError("bad saved settings: " + texture.get_path_name())
        normal = lib.get_material_instance_texture_parameter_value(instance, "NormalTex")
        roughness = lib.get_material_instance_texture_parameter_value(instance, "RoughnessTex")
        if normal != load(detail_path(surface, "N")) or roughness != load(detail_path(surface, "R")):
            raise RuntimeError("bad instance binding: " + instance.get_path_name())
        log(f"{surface}: N={normal.get_path_name()} R={roughness.get_path_name()}")
    log("PASS: vector TileSize, decal-safe normal sampler, 16 texture settings and 8 bindings")


run()
