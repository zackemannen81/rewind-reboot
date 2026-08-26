"""Apply REW-0031 surface details without replacing the proven base-colour graph.

The REW-0031 PNG sources are retained verbatim under `Derived_REW-0031`; this
script only imports/configures them, binds instances, and rebuilds the material
around the existing WorldAlignedTexture contract. It never opens or saves a map.

Set REW0032_MODE=bind_only to bind detail parameters against the old master for
the forced-recompile bisect. The default mode builds the corrected master.
"""

import os
import unreal


TASK = "[REW-0032]"
TEXTURE_DIR = "/Game/Art/Textures/Surfaces"
MATERIAL_DIR = "/Game/Art/Materials/Surfaces"
MASTER_PATH = MATERIAL_DIR + "/M_REW_Surface"
DERIVED_DIR = os.path.join(
    unreal.Paths.convert_relative_path_to_full(unreal.Paths.project_dir()),
    "ArtSource", "Textures", "EnvironmentTextureKit-v1", "Derived_REW-0031",
).replace("\\", "/")
WORLD_ALIGNED_TEXTURE = (
    "/Engine/Functions/Engine_MaterialFunctions01/Texturing/WorldAlignedTexture.WorldAlignedTexture")
WORLD_ALIGNED_NORMAL = (
    "/Engine/Functions/Engine_MaterialFunctions01/Texturing/WorldAlignedNormal.WorldAlignedNormal")
SURFACES = (
    "BrickDark_Aged", "CeramicTile_Ivory_Aged", "FloorTile_Interior",
    "MetalBlack_Painted_Aged", "PavingStone_Courtyard", "PlasterGray_Aged",
    "PlasterGreen_Aged", "WoodDark_Worn",
)


def log(message):
    unreal.log(f"{TASK} {message}")


def load(path):
    value = unreal.EditorAssetLibrary.load_asset(path)
    if not value:
        raise RuntimeError("missing required asset: " + path)
    return value


def detail_path(surface, suffix):
    return f"{TEXTURE_DIR}/T_REW_Surface_{surface}_{suffix}"


def import_and_configure_details():
    tasks = []
    for surface in SURFACES:
        for suffix in ("N", "R"):
            name = f"T_REW_Surface_{surface}_{suffix}"
            source = f"{DERIVED_DIR}/{name}.png"
            if not os.path.isfile(source):
                raise RuntimeError("retained REW-0031 source is missing: " + source)
            task = unreal.AssetImportTask()
            task.filename = source
            task.destination_path = TEXTURE_DIR
            task.destination_name = name
            task.automated = True
            task.replace_existing = True
            task.replace_existing_settings = True
            task.save = False
            task.options = unreal.TextureFactory()
            tasks.append(task)
    unreal.AssetToolsHelpers.get_asset_tools().import_asset_tasks(tasks)
    for task in tasks:
        if not task.imported_object_paths:
            raise RuntimeError("import returned no asset for " + task.filename)
    for surface in SURFACES:
        for suffix in ("N", "R"):
            texture = load(detail_path(surface, suffix))
            texture.set_editor_property("srgb", False)
            texture.set_editor_property("address_x", unreal.TextureAddress.TA_WRAP)
            texture.set_editor_property("address_y", unreal.TextureAddress.TA_WRAP)
            texture.set_editor_property("lod_group", unreal.TextureGroup.TEXTUREGROUP_WORLD)
            texture.set_editor_property("max_texture_size", 2048)
            texture.set_editor_property(
                "compression_settings",
                unreal.TextureCompressionSettings.TC_NORMALMAP
                if suffix == "N" else unreal.TextureCompressionSettings.TC_MASKS,
            )
            unreal.EditorAssetLibrary.save_loaded_asset(texture)
    log("imported and saved 16 retained REW-0031 detail textures")


def make(material, expression_class, x, y):
    return unreal.MaterialEditingLibrary.create_material_expression(
        material, expression_class, x, y)


def connect(material, source, output, target, input_name):
    unreal.MaterialEditingLibrary.connect_material_expressions(
        source, output, target, input_name)


def texture_object_parameter(material, name, texture, sampler_type, x, y):
    node = make(material, unreal.MaterialExpressionTextureObjectParameter, x, y)
    node.set_editor_property("parameter_name", name)
    node.set_editor_property("texture", texture)
    node.set_editor_property("sampler_type", sampler_type)
    return node


def texture_sample_parameter(material, name, texture, sampler_type, x, y):
    node = make(material, unreal.MaterialExpressionTextureSampleParameter2D, x, y)
    node.set_editor_property("parameter_name", name)
    node.set_editor_property("texture", texture)
    node.set_editor_property("sampler_type", sampler_type)
    return node


def world_aligned_call(material, function_path, x, y):
    node = make(material, unreal.MaterialExpressionMaterialFunctionCall, x, y)
    node.set_editor_property("material_function", load(function_path))
    return node


def extend_master():
    material = load(MASTER_PATH)
    lib = unreal.MaterialEditingLibrary
    texture_names = {str(name) for name in lib.get_texture_parameter_names(material)}
    vector_names = {str(name) for name in lib.get_vector_parameter_names(material)}
    scalar_names = {str(name) for name in lib.get_scalar_parameter_names(material)}
    if texture_names != {"BaseColorTex"} or vector_names != {"TileSize", "Tint"} or scalar_names != {"Roughness"}:
        raise RuntimeError("master is not the untouched REW-0027 contract")
    nodes = lib.get_material_expressions(material)
    tile_size = next(
        node for node in nodes
        if node.get_class().get_name() == "MaterialExpressionVectorParameter"
        and str(node.get_editor_property("parameter_name")) == "TileSize")
    roughness_scalar = next(
        node for node in nodes
        if node.get_class().get_name() == "MaterialExpressionScalarParameter"
        and str(node.get_editor_property("parameter_name")) == "Roughness")

    # Preserve every pre-existing expression and connection. In particular,
    # BaseColorTex -> WorldAlignedTexture -> Tint remains exactly as REW-0027
    # authored it; this task only appends detail branches.
    normal = texture_sample_parameter(
        material, "NormalTex", load("/Engine/EngineMaterials/DefaultNormal"),
        unreal.MaterialSamplerType.SAMPLERTYPE_NORMAL, -1000, -40)
    roughness_tex = texture_object_parameter(
        material, "RoughnessTex", load("/Engine/EngineResources/WhiteSquareTexture"),
        unreal.MaterialSamplerType.SAMPLERTYPE_LINEAR_GRAYSCALE, -1000, 300)
    roughness_world = world_aligned_call(material, WORLD_ALIGNED_TEXTURE, -650, 300)
    roughness_mask = make(material, unreal.MaterialExpressionComponentMask, -250, 330)
    roughness_mask.set_editor_property("r", True)
    roughness = make(material, unreal.MaterialExpressionMultiply, 0, 330)

    for texture_node, world_node in ((roughness_tex, roughness_world),):
        connect(material, texture_node, "", world_node, "TextureObject")
        connect(material, tile_size, "RGB", world_node, "TextureSize")
    connect(material, roughness_world, "XYZ Texture", roughness_mask, "Input")
    connect(material, roughness_mask, "R", roughness, "A")
    connect(material, roughness_scalar, "", roughness, "B")
    # WorldAlignedNormal and a WorldAlignedTexture normal output both regressed
    # 4C decals in matched captures. Keep this normal tangent-space, using the
    # mesh's authored UVs; the base-colour and roughness paths retain the
    # vector world-aligned projection contract.
    lib.connect_material_property(normal, "RGB", unreal.MaterialProperty.MP_NORMAL)
    lib.connect_material_property(roughness, "", unreal.MaterialProperty.MP_ROUGHNESS)
    lib.recompile_material(material)
    unreal.EditorAssetLibrary.save_loaded_asset(material)
    log("extended the untouched vector WorldAlignedTexture base graph with world-aligned detail branches")
    return material


def bind_instances(master):
    lib = unreal.MaterialEditingLibrary
    for surface in SURFACES:
        instance = load(f"{MATERIAL_DIR}/MI_REW_{surface}")
        instance.modify()
        instance.set_editor_property("parent", master)
        lib.set_material_instance_texture_parameter_value(
            instance, "NormalTex", load(detail_path(surface, "N")))
        lib.set_material_instance_texture_parameter_value(
            instance, "RoughnessTex", load(detail_path(surface, "R")))
        lib.update_material_instance(instance)
        unreal.EditorAssetLibrary.save_loaded_asset(instance)
    log("bound and saved normal/roughness values on 8 surface instances")


def verify(master):
    lib = unreal.MaterialEditingLibrary
    expected_texture_parameters = {"BaseColorTex", "NormalTex", "RoughnessTex"}
    expected_vector_parameters = {"TileSize", "Tint"}
    texture_parameters = set(str(name) for name in lib.get_texture_parameter_names(master))
    vector_parameters = set(str(name) for name in lib.get_vector_parameter_names(master))
    if texture_parameters != expected_texture_parameters:
        raise RuntimeError("unexpected master texture parameters: " + str(texture_parameters))
    if vector_parameters != expected_vector_parameters:
        raise RuntimeError("unexpected master vector parameters: " + str(vector_parameters))
    calls = [node for node in lib.get_material_expressions(master)
             if node.get_class().get_name() == "MaterialExpressionMaterialFunctionCall"]
    function_paths = [node.get_editor_property("material_function").get_path_name() for node in calls]
    if function_paths.count(WORLD_ALIGNED_TEXTURE) != 2 or WORLD_ALIGNED_NORMAL in function_paths:
        raise RuntimeError("world-aligned graph call count is wrong: " + str(function_paths))
    for surface in SURFACES:
        for suffix, compression in (("N", unreal.TextureCompressionSettings.TC_NORMALMAP),
                                    ("R", unreal.TextureCompressionSettings.TC_MASKS)):
            texture = load(detail_path(surface, suffix))
            if (texture.get_editor_property("srgb") or
                    texture.get_editor_property("compression_settings") != compression or
                    texture.get_editor_property("address_x") != unreal.TextureAddress.TA_WRAP or
                    texture.get_editor_property("address_y") != unreal.TextureAddress.TA_WRAP):
                raise RuntimeError("bad saved texture settings: " + texture.get_path_name())
        instance = load(f"{MATERIAL_DIR}/MI_REW_{surface}")
        normal = lib.get_material_instance_texture_parameter_value(instance, "NormalTex")
        roughness = lib.get_material_instance_texture_parameter_value(instance, "RoughnessTex")
        if normal != load(detail_path(surface, "N")) or roughness != load(detail_path(surface, "R")):
            raise RuntimeError("incorrect instance detail binding: " + instance.get_path_name())
        log(f"VERIFY {surface}: NormalTex={normal.get_path_name()} RoughnessTex={roughness.get_path_name()}")
    log("VERIFY PASS: vector world-aligned master plus 16 saved detail textures and 8 explicit bindings")


def main():
    log("starting; no map asset will be opened or modified")
    import_and_configure_details()
    master = load(MASTER_PATH)
    if os.environ.get("REW0032_MODE") == "bind_only":
        bind_instances(master)
        unreal.MaterialEditingLibrary.recompile_material(master)
        unreal.EditorAssetLibrary.save_loaded_asset(master)
        log("bind-only state saved after forced master recompilation")
        return
    master = extend_master()
    bind_instances(master)
    verify(master)
    log("DONE")


main()
