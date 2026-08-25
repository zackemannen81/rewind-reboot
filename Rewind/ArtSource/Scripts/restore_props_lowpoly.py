"""Restore the owner's 4C props as low-poly meshes at their original paths.

The map still holds every placement the owner authored; the actors reference
assets that were lost when an editor holding unsaved imports was killed. The
placements are the irreplaceable part, and they survive as long as the assets
come back at the SAME package paths. Importing under new names would orphan
them.

Sources are Meshy remeshes of the owner's own models, 1,821 to 5,921 triangles
each, downloaded outside the repository because the exports embed 4K textures.
"""
import os
import unreal

SRC = os.environ.get(
    "REW_PROP_SRC",
    "C:/Users/zakri/AppData/Local/Temp/claude/C--code-rewind-reboot/"
    "b0aefe8e-554e-4e6a-9865-5d8a7c0b5983/scratchpad/props_lowpoly/")

# (fbx, destination folder, asset name exactly as the map references it)
PROPS = [
    ("kitchentable.fbx", "/Game/Props/Chapter1/kitchentable",
     "Meshy_AI_kitchen_table_3d_0825162406_image-to-3d-texture"),
    ("shelf.fbx", "/Game/Props/Chapter1/shelf",
     "Meshy_AI_shelf_3d_0825175810_image-to-3d-texture"),
    ("door.fbx", "/Game/Props/Chapter1/door",
     "Meshy_AI_door_3d_0825162626_image-to-3d-texture"),
    ("coat.fbx", "/Game/Props/Chapter1/coathanger",
     "Meshy_AI_coat_3d_0825162710_image-to-3d-texture"),
    ("radio.fbx", "/Game/Props/Chapter1/Radio",
     "Meshy_AI_radio_3d_0825162448_image-to-3d-texture"),
    ("footstool.fbx", "/Game/Props/Chapter1/sofatable",
     "Meshy_AI_footstool_3d_0825175833_image-to-3d-texture"),
    ("sofa.fbx", "/Game/Props/Chapter1/sofa",
     "Meshy_AI_sofa_3d_0825162856_image-to-3d-texture"),
    ("railingplant.fbx", "/Game/Props/Chapter1/windowrail",
     "Meshy_AI_railing_plant_3d_0825162751_image-to-3d-texture"),
    ("lamp.fbx", "/Game/Props/Chapter1/Lamp",
     "Meshy_AI_pendant_lamp_3d_0825174536_image-to-3d-texture"),
    ("sign.fbx", "/Game/Props/Chapter1/signs",
     "Meshy_AI_neon_sign_3d_0825162358_image-to-3d-texture"),
    # Not currently placed, imported so they are available for dressing.
    ("chair.fbx", "/Game/Props/Chapter1/chair", "SM_REW_Chair"),
    ("drapes.fbx", "/Game/Props/Chapter1/drapes", "SM_REW_Drapes"),
]

# The lamp and sign were remeshed in an earlier pass and live beside the repo.
ALT_SRC = "C:/code/rewind-reboot/Rewind/ArtSource/Props/Chapter1/"
ALT = {"lamp.fbx": "lamp_lowpoly.fbx", "sign.fbx": "sign_lowpoly.fbx"}


def log(m):
    unreal.log("[REW-restore] " + str(m))


def tri_count(mesh):
    try:
        return mesh.get_num_triangles(0)
    except Exception:
        return "?"


def run():
    tools = unreal.AssetToolsHelpers.get_asset_tools()
    made, missing = 0, []

    for filename, folder, name in PROPS:
        path = SRC + filename
        if not os.path.exists(path):
            alt = ALT_SRC + ALT.get(filename, "")
            if ALT.get(filename) and os.path.exists(alt):
                path = alt
            else:
                missing.append(filename)
                continue

        task = unreal.AssetImportTask()
        task.filename = path
        task.destination_path = folder
        task.destination_name = name
        task.automated = True
        task.replace_existing = True
        task.save = True

        opts = unreal.FbxImportUI()
        opts.import_mesh = True
        opts.import_as_skeletal = False
        # Materials and textures come with the FBX so the props are not grey,
        # but every texture is capped below; 4K maps are invisible at this
        # game's exposure and are not free.
        opts.import_materials = True
        opts.import_textures = True
        opts.import_animations = False
        opts.static_mesh_import_data.set_editor_property("combine_meshes", True)
        task.options = opts

        tools.import_asset_tasks([task])

        asset = unreal.EditorAssetLibrary.load_asset(f"{folder}/{name}")
        if asset:
            try:
                ns = asset.get_editor_property("nanite_settings")
                ns.enabled = False
                asset.set_editor_property("nanite_settings", ns)
            except Exception:
                pass
            unreal.EditorAssetLibrary.save_asset(f"{folder}/{name}")
            log(f"{name.split('_3d_')[0][:34]:<34} {tri_count(asset)} tris")
            made += 1
        else:
            missing.append(name)

    # Cap every texture the imports brought in. Roughness and metallic arrive
    # flagged sRGB, which is wrong for linear mask data, not merely heavy.
    reg = unreal.AssetRegistryHelpers.get_asset_registry()
    capped = 0
    for a in reg.get_assets_by_path("/Game/Props", recursive=True):
        if str(a.asset_class_path.asset_name) != "Texture2D":
            continue
        t = unreal.EditorAssetLibrary.load_asset(str(a.package_name))
        if not t:
            continue
        n = str(a.asset_name).lower()
        linear = ("rough" in n or "metal" in n or "normal" in n)
        try:
            t.set_editor_property("max_texture_size", 512 if linear else 1024)
            if "rough" in n or "metal" in n:
                t.set_editor_property("srgb", False)
                t.set_editor_property(
                    "compression_settings",
                    unreal.TextureCompressionSettings.TC_MASKS)
            unreal.EditorAssetLibrary.save_asset(str(a.package_name))
            capped += 1
        except Exception as e:
            log(f"  cap failed on {a.asset_name}: {e}")

    log(f"imported {made} props, capped {capped} textures")
    if missing:
        log(f"MISSING: {', '.join(missing)}")
    log("DONE")


run()
