"""Replace the three heaviest props with their remeshed low-poly versions.

Meshy's raw output for these was around 1.9 million triangles each. Nanite was
carrying them, which is what Nanite is for, but it is not a reason to ship a
desk with more geometry than a hero character. They were remeshed through
Meshy's remesh endpoint to between 2,490 and 4,140 triangles.

Re-imports over the SAME asset path on purpose. The level references these
assets by path, so replacing in place keeps every placement the owner authored;
importing under a new name would orphan them.

Material slot assignments are captured before the import and restored after,
because a re-import can reset them.

Run with the editor closed:
    UnrealEditor-Cmd.exe <uproject> -ExecutePythonScript="<this file>"
      -unattended -nopause -nosplash
"""
import unreal

SRC = "C:/code/rewind-reboot/Rewind/ArtSource/Props/Chapter1/"

PROPS = [
    ("desk_lowpoly.fbx", "/Game/Props/Chapter1/Desk",
     "Meshy_AI_desk_3d_0825162439_image-to-3d-texture"),
    ("lamp_lowpoly.fbx", "/Game/Props/Chapter1/Lamp",
     "Meshy_AI_pendant_lamp_3d_0825174536_image-to-3d-texture"),
    ("sign_lowpoly.fbx", "/Game/Props/Chapter1/signs",
     "Meshy_AI_neon_sign_3d_0825162358_image-to-3d-texture"),
]


def log(m):
    unreal.log("[REW-props] " + str(m))


def tri_count(mesh):
    try:
        return mesh.get_num_triangles(0)
    except Exception:
        pass
    try:
        return unreal.EditorStaticMeshLibrary.get_number_triangles(mesh, 0)
    except Exception:
        return "?"


def run():
    tools = unreal.AssetToolsHelpers.get_asset_tools()

    for filename, folder, name in PROPS:
        path = f"{folder}/{name}"
        before = unreal.EditorAssetLibrary.load_asset(path)
        if not before:
            log(f"MISSING {path} — skipped")
            continue

        old_tris = tri_count(before)
        # Capture material assignments; a re-import can reset them.
        mats = []
        try:
            for m in before.get_editor_property("static_materials"):
                mats.append((m.material_slot_name, m.material_interface))
        except Exception as e:
            log(f"  could not read materials on {name}: {e}")

        task = unreal.AssetImportTask()
        task.filename = SRC + filename
        task.destination_path = folder
        task.destination_name = name
        task.automated = True
        task.replace_existing = True
        task.save = True

        opts = unreal.FbxImportUI()
        opts.import_mesh = True
        opts.import_as_skeletal = False
        opts.import_materials = False
        opts.import_textures = False
        opts.import_animations = False
        opts.static_mesh_import_data.set_editor_property("combine_meshes", True)
        task.options = opts

        tools.import_asset_tasks([task])

        after = unreal.EditorAssetLibrary.load_asset(path)
        if not after:
            log(f"FAILED to reload {path} after import")
            continue

        # Restore material assignments if the import cleared them.
        try:
            slots = after.get_editor_property("static_materials")
            changed = False
            for i, sm in enumerate(slots):
                if sm.material_interface is None and i < len(mats):
                    sm.material_interface = mats[i][1]
                    sm.material_slot_name = mats[i][0]
                    changed = True
            if changed:
                after.set_editor_property("static_materials", slots)
                log(f"  restored {len(mats)} material slot(s)")
        except Exception as e:
            log(f"  material restore skipped on {name}: {e}")

        # These are now light enough that Nanite is unnecessary overhead.
        try:
            ns = after.get_editor_property("nanite_settings")
            ns.enabled = False
            after.set_editor_property("nanite_settings", ns)
        except Exception as e:
            log(f"  nanite toggle skipped on {name}: {e}")

        unreal.EditorAssetLibrary.save_asset(path)
        log(f"{name}: {old_tris} -> {tri_count(after)} triangles")

    log("DONE")


run()
