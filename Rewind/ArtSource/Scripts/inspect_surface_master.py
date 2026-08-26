"""Print the saved RE:WIND surface-master graph and instance parameter contract.

Run headlessly with UnrealEditor-Cmd.exe and `-ExecutePythonScript`. The script
only reads `/Game/Art/Materials/Surfaces`; it never opens or saves a map.
"""

import unreal


MASTER_PATH = "/Game/Art/Materials/Surfaces/M_REW_Surface"
MATERIAL_DIR = "/Game/Art/Materials/Surfaces"
SURFACES = (
    "BrickDark_Aged", "CeramicTile_Ivory_Aged", "FloorTile_Interior",
    "MetalBlack_Painted_Aged", "PavingStone_Courtyard", "PlasterGray_Aged",
    "PlasterGreen_Aged", "WoodDark_Worn",
)


def log(message):
    unreal.log("[REW-0032 inspect] " + str(message))


def path_of(value):
    return value.get_path_name() if value else "None"


def input_source(node, name):
    try:
        source = node.get_editor_property(name)
        expression = source.get_editor_property("expression")
        output_index = source.get_editor_property("output_index")
        return f"{expression.get_name() if expression else 'None'}:{output_index}"
    except Exception:
        return "unavailable"


def run():
    material = unreal.EditorAssetLibrary.load_asset(MASTER_PATH)
    if not material:
        raise RuntimeError("missing " + MASTER_PATH)
    log("master=" + path_of(material))
    library = unreal.MaterialEditingLibrary
    log("params textures=" + str(list(library.get_texture_parameter_names(material))))
    log("params scalars=" + str(list(library.get_scalar_parameter_names(material))))
    log("params vectors=" + str(list(library.get_vector_parameter_names(material))))
    log("material-library methods=" + str([
        name for name in dir(library)
        if any(word in name.lower() for word in ("expression", "function", "input", "output"))
    ]))
    # `Material.Expressions` is protected in UE 5.8 Python; the editing library
    # exposes the same collection for a read-only graph walk.
    nodes = library.get_material_expressions(material)
    for index, node in enumerate(nodes):
        if not node.get_class().get_name().startswith("MaterialExpression"):
            continue
        cls = node.get_class().get_name()
        fields = []
        for prop in ("parameter_name", "sampler_type", "texture", "material_function",
                     "default_value", "const_a", "const_b"):
            try:
                value = node.get_editor_property(prop)
                fields.append(f"{prop}={path_of(value) if prop in ('texture', 'material_function') else value}")
            except Exception:
                pass
        inputs = []
        for name in ("input", "a", "b", "coordinates", "texture_size", "world_position",
                     "normal", "projection_transition_contrast"):
            value = input_source(node, name)
            if value != "unavailable":
                inputs.append(f"{name}<-{value}")
        try:
            fields.append("input_names=" + str(list(library.get_material_expression_input_names(node))))
            fields.append("output_names=" + str(list(library.get_material_expression_output_names(node))))
        except Exception as error:
            fields.append("pin_names=" + str(error))
        log(f"node[{index}] name={node.get_name()} class={cls} " +
            " ".join(fields + inputs))
    for surface in SURFACES:
        instance = unreal.EditorAssetLibrary.load_asset(f"{MATERIAL_DIR}/MI_REW_{surface}")
        if not instance:
            log(f"instance={surface} MISSING")
            continue
        parent = instance.get_editor_property("parent")
        texture_values = []
        for parameter in library.get_texture_parameter_names(material):
            value = library.get_material_instance_texture_parameter_value(instance, parameter)
            texture_values.append(f"{parameter}={path_of(value)}")
        scalar_values = []
        for parameter in library.get_scalar_parameter_names(material):
            scalar_values.append(
                f"{parameter}={library.get_material_instance_scalar_parameter_value(instance, parameter)}")
        vector_values = []
        for parameter in library.get_vector_parameter_names(material):
            vector_values.append(
                f"{parameter}={library.get_material_instance_vector_parameter_value(instance, parameter)}")
        log(f"instance={surface} parent={path_of(parent)} " +
            " ".join(texture_values + scalar_values + vector_values))
    for function_path in (
            "/Engine/Functions/Engine_MaterialFunctions01/Texturing/WorldAlignedTexture.WorldAlignedTexture",
            "/Engine/Functions/Engine_MaterialFunctions01/Texturing/WorldAlignedNormal.WorldAlignedNormal"):
        function = unreal.EditorAssetLibrary.load_asset(function_path)
        if not function:
            log("function MISSING=" + function_path)
            continue
        pins = []
        for node in library.get_material_function_expressions(function):
            if node.get_class().get_name() not in (
                    "MaterialExpressionFunctionInput", "MaterialExpressionFunctionOutput"):
                continue
            try:
                name = node.get_editor_property("input_name")
            except Exception:
                name = node.get_editor_property("output_name")
            pins.append(f"{node.get_class().get_name()}:{name}")
        log(f"function={function_path} pins={pins}")
    log("DONE")


run()
