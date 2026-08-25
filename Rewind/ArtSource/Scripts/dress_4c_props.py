"""Dress the 4C props with instances of the shared surface master.

The props import without materials on purpose (see restore_props_untextured),
so they arrive grey. Rather than give each one its own baked texture set, each
gets a material instance of M_REW_Surface: the same eight kit textures, tinted
and roughened per prop. Twelve props cost twelve small instance assets instead
of twelve 4K sets, and they read as one building rather than twelve downloads.

Parameter names are discovered from the master rather than hard-coded, because
the master was authored by a separate task and its names are its own business.

Run with the editor closed:
    UnrealEditor-Cmd.exe <uproject> -ExecutePythonScript="<this file>"
      -unattended -nopause -nosplash -NullRHI
"""
import unreal

MASTER = "/Game/Art/Materials/Surfaces/M_REW_Surface"
TEX = "/Game/Art/Textures/Surfaces/T_REW_Surface_{}_BC"
MI_DIR = "/Game/Props/Chapter1/Materials"

# (asset path, surface, tint RGB, roughness, world tiling size in cm)
# Tints are muted on purpose. This game renders at low exposure; saturated
# props turn to mud, and near-black ones lose their silhouette entirely.
PLAN = [
    ("kitchentable/Meshy_AI_kitchen_table_3d_0825162406_image-to-3d-texture",
     "WoodDark_Worn", (0.42, 0.29, 0.19), 0.62, 160.0),
    ("shelf/Meshy_AI_shelf_3d_0825175810_image-to-3d-texture",
     "WoodDark_Worn", (0.34, 0.24, 0.16), 0.68, 140.0),
    ("door/Meshy_AI_door_3d_0825162626_image-to-3d-texture",
     "WoodDark_Worn", (0.30, 0.34, 0.28), 0.55, 200.0),
    ("coathanger/Meshy_AI_coat_3d_0825162710_image-to-3d-texture",
     "MetalBlack_Painted_Aged", (0.26, 0.24, 0.26), 0.70, 90.0),
    ("Radio/Meshy_AI_radio_3d_0825162448_image-to-3d-texture",
     "MetalBlack_Painted_Aged", (0.38, 0.30, 0.22), 0.45, 40.0),
    ("sofatable/Meshy_AI_footstool_3d_0825175833_image-to-3d-texture",
     "WoodDark_Worn", (0.38, 0.27, 0.18), 0.65, 110.0),
    ("sofa/Meshy_AI_sofa_3d_0825162856_image-to-3d-texture",
     "PlasterGreen_Aged", (0.30, 0.33, 0.26), 0.88, 180.0),
    ("windowrail/Meshy_AI_railing_plant_3d_0825162751_image-to-3d-texture",
     "MetalBlack_Painted_Aged", (0.22, 0.23, 0.22), 0.72, 80.0),
    ("Lamp/Meshy_AI_pendant_lamp_3d_0825174536_image-to-3d-texture",
     "MetalBlack_Painted_Aged", (0.24, 0.23, 0.21), 0.50, 60.0),
    ("chair/SM_REW_Chair",
     "WoodDark_Worn", (0.36, 0.26, 0.17), 0.66, 120.0),
    ("drapes/SM_REW_Drapes",
     "PlasterGray_Aged", (0.34, 0.33, 0.31), 0.92, 220.0),
]

# The sign is lit, not surfaced. It already has an authored emissive master.
EMISSIVE = [("signs/Meshy_AI_neon_sign_3d_0825162358_image-to-3d-texture",
             "/Game/Art/Materials/BuildingSlice/M_4CSignEmissive")]


def log(m):
    unreal.log("[REW-dress] " + str(m))


def pick(names, *wanted):
    """First parameter whose name contains any of the wanted substrings."""
    for w in wanted:
        for n in names:
            if w.lower() in str(n).lower():
                return n
    return None


def run():
    master = unreal.EditorAssetLibrary.load_asset(MASTER)
    if not master:
        log(f"FATAL: no master at {MASTER}")
        return

    lib = unreal.MaterialEditingLibrary
    tex_names = list(lib.get_texture_parameter_names(master))
    vec_names = list(lib.get_vector_parameter_names(master))
    sca_names = list(lib.get_scalar_parameter_names(master))
    log(f"master params: tex={tex_names} vec={vec_names} scalar={sca_names}")

    p_tex = pick(tex_names, "base", "color", "albedo", "diffuse") or (
        tex_names[0] if tex_names else None)
    p_tint = pick(vec_names, "tint", "color")
    p_size = pick(vec_names, "size", "tiling", "scale")
    p_rough = pick(sca_names, "rough")
    log(f"using tex={p_tex} tint={p_tint} size={p_size} rough={p_rough}")

    # Preload once, and fail loudly here rather than per prop: a wrong texture
    # path is otherwise invisible, because a material instance with an unset
    # texture parameter silently falls back to the engine's DefaultTexture and
    # every prop still reports as dressed. The verify pass below reads the
    # parameters back for the same reason.
    surfaces = sorted({surface for _, surface, _, _, _ in PLAN})
    cache = {}
    for s_name in surfaces:
        t = unreal.EditorAssetLibrary.load_asset(TEX.format(s_name))
        if t:
            cache[s_name] = t
        else:
            log(f"  FATAL: no texture asset {TEX.format(s_name)}")
    log(f"preloaded {len(cache)}/{len(surfaces)} surface textures")

    tools = unreal.AssetToolsHelpers.get_asset_tools()
    unreal.EditorAssetLibrary.make_directory(MI_DIR)
    done = 0

    for rel, surface, tint, rough, size in PLAN:
        mesh_path = f"/Game/Props/Chapter1/{rel}"
        mesh = unreal.EditorAssetLibrary.load_asset(mesh_path)
        if not mesh:
            log(f"  MISSING mesh {mesh_path}")
            continue

        short = rel.split("/")[0]
        mi_path = f"{MI_DIR}/MI_REW_Prop_{short}"
        if unreal.EditorAssetLibrary.does_asset_exist(mi_path):
            unreal.EditorAssetLibrary.delete_asset(mi_path)
        mi = tools.create_asset(
            f"MI_REW_Prop_{short}", MI_DIR,
            unreal.MaterialInstanceConstant,
            unreal.MaterialInstanceConstantFactoryNew())
        if not mi:
            log(f"  could not create {mi_path}")
            continue
        lib.set_material_instance_parent(mi, master)

        if p_tex and surface in cache:
            lib.set_material_instance_texture_parameter_value(
                mi, p_tex, cache[surface])
        if p_tint:
            lib.set_material_instance_vector_parameter_value(
                mi, p_tint, unreal.LinearColor(tint[0], tint[1], tint[2], 1.0))
        if p_size:
            lib.set_material_instance_vector_parameter_value(
                mi, p_size, unreal.LinearColor(size, size, size, 1.0))
        if p_rough:
            lib.set_material_instance_scalar_parameter_value(mi, p_rough, rough)

        unreal.EditorAssetLibrary.save_asset(mi_path)

        slots = mesh.get_editor_property("static_materials")
        for sm in slots:
            sm.material_interface = mi
        mesh.set_editor_property("static_materials", slots)
        unreal.EditorAssetLibrary.save_asset(mesh_path)
        log(f"{short:<16} {surface:<26} {len(slots)} slot(s)")
        done += 1

    for rel, mat_path in EMISSIVE:
        mesh_path = f"/Game/Props/Chapter1/{rel}"
        mesh = unreal.EditorAssetLibrary.load_asset(mesh_path)
        mat = unreal.EditorAssetLibrary.load_asset(mat_path)
        if not mesh or not mat:
            log(f"  emissive skip {rel}: mesh={bool(mesh)} mat={bool(mat)}")
            continue
        slots = mesh.get_editor_property("static_materials")
        for sm in slots:
            sm.material_interface = mat
        mesh.set_editor_property("static_materials", slots)
        unreal.EditorAssetLibrary.save_asset(mesh_path)
        log(f"{rel.split('/')[0]:<16} emissive                   {len(slots)} slot(s)")
        done += 1

    log("-" * 62)
    bad = 0
    for rel, surface, _, _, _ in PLAN:
        short = rel.split("/")[0]
        mi = unreal.EditorAssetLibrary.load_asset(
            f"{MI_DIR}/MI_REW_Prop_{short}")
        got = lib.get_material_instance_texture_parameter_value(
            mi, p_tex) if (mi and p_tex) else None
        name = got.get_name() if got else "NONE"
        ok = surface in name
        if not ok:
            bad += 1
        log(f"  verify {short:<14} {'ok ' if ok else 'BAD'} {name}")
    log(f"dressed {done} props, {bad} with the wrong or no texture")
    log("DONE")


run()
