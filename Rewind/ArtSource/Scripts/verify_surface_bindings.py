"""Read back what the eight surface instances actually reference.

A material instance with an unset texture parameter falls back silently to the
engine's DefaultTexture, so "assigned" and "reports as assigned" are different
claims. This makes the first claim checkable: it prints the resolved texture
behind every texture parameter on every MI_REW_* surface instance, plus the
compression settings and sRGB flag, because a normal map imported as sRGB
colour data lights wrong in a way nothing else reports.

Run with the editor closed, against any clone:
    UnrealEditor-Cmd.exe <uproject> -ExecutePythonScript="<this file>"
      -unattended -nopause -nosplash -NullRHI
"""
import unreal

MI_DIR = "/Game/Art/Materials/Surfaces"
NAMES = ("BrickDark_Aged", "CeramicTile_Ivory_Aged", "FloorTile_Interior",
         "MetalBlack_Painted_Aged", "PavingStone_Courtyard",
         "PlasterGray_Aged", "PlasterGreen_Aged", "WoodDark_Worn")


def log(m):
    unreal.log("[REW-verify] " + str(m))


def run():
    lib = unreal.MaterialEditingLibrary
    master = unreal.EditorAssetLibrary.load_asset(f"{MI_DIR}/M_REW_Surface")
    if not master:
        log("FATAL: no master M_REW_Surface")
        return
    params = [str(p) for p in lib.get_texture_parameter_names(master)]
    log(f"master texture params: {params}")
    log(f"master scalar params:  "
        f"{[str(p) for p in lib.get_scalar_parameter_names(master)]}")
    log(f"master vector params:  "
        f"{[str(p) for p in lib.get_vector_parameter_names(master)]}")
    log("-" * 96)

    bad = 0
    for n in NAMES:
        mi = unreal.EditorAssetLibrary.load_asset(f"{MI_DIR}/MI_REW_{n}")
        if not mi:
            log(f"{n:<26} MISSING INSTANCE")
            bad += 1
            continue
        parts = []
        for p in params:
            t = lib.get_material_instance_texture_parameter_value(mi, p)
            if not t:
                parts.append(f"{p}=NONE")
                bad += 1
                continue
            tn = t.get_name()
            if "Default" in tn or "WhiteSquare" in tn:
                parts.append(f"{p}={tn}<-FALLBACK")
                bad += 1
                continue
            # A normal map must be TC_Normalmap and NOT sRGB; a roughness mask
            # must not be sRGB either. Both are invisible until lit.
            flags = ""
            try:
                srgb = t.get_editor_property("srgb")
                comp = str(t.get_editor_property("compression_settings"))
                short = comp.split(".")[-1]
                if tn.endswith("_N"):
                    ok = (not srgb) and "NORMALMAP" in short.upper()
                    flags = f"[{short},sRGB={srgb}]{'' if ok else ' <-WRONG'}"
                    if not ok:
                        bad += 1
                elif tn.endswith("_R"):
                    ok = not srgb
                    flags = f"[{short},sRGB={srgb}]{'' if ok else ' <-WRONG'}"
                    if not ok:
                        bad += 1
            except Exception as e:
                flags = f"[flags unreadable: {e}]"
            parts.append(f"{p}={tn}{flags}")
        # Scalar and vector overrides too. Rebuilding a master can drop an
        # instance's overrides, and a lost Tint is invisible in a texture
        # read-back while changing every surface in the game.
        ov = []
        for sp in lib.get_scalar_parameter_names(master):
            v = lib.get_material_instance_scalar_parameter_value(mi, sp)
            ov.append(f"{sp}={v:g}")
        for vp in lib.get_vector_parameter_names(master):
            c = lib.get_material_instance_vector_parameter_value(mi, vp)
            ov.append(f"{vp}=({c.r:.2f},{c.g:.2f},{c.b:.2f})")
        log(f"{n:<26} " + "  ".join(parts))
        log(f"{'':<26}   " + "  ".join(ov))

    log("-" * 96)
    log(f"{bad} problems across {len(NAMES)} instances")
    log("DONE")


run()
