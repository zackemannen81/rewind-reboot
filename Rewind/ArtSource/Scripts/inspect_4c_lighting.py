"""Inventory what is lighting Apartment 4C, and what is lighting the level.

The first render of the finished room came back almost entirely black. That is
either the lighting or the capture's missing eye adaptation, and the way to
tell them apart is to look at what light actually exists: intensity,
attenuation and colour per light, plus whether the level has a sky light,
directional light or post-process volume setting global exposure at all.

Run with the editor closed:
    UnrealEditor-Cmd.exe <uproject> -ExecutePythonScript="<this file>"
      -unattended -nopause -nosplash -NullRHI
"""
import unreal

MAP = "/Game/Maps/FiveLoops_Stairwell_Blockout"
Y0, Y1 = 700.0, 2300.0


def log(m):
    unreal.log("[REW-light] " + str(m))


def prop(comp, name, default=None):
    try:
        return comp.get_editor_property(name)
    except Exception:
        return default


def run():
    unreal.EditorLoadingAndSavingUtils.load_map(MAP)
    sub = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    actors = sub.get_all_level_actors()

    log(f"{'label':<26} {'class':<18} {'intensity':>10} {'atten':>8} "
        f"{'colour':>18} {'Y':>7}")
    log("-" * 92)
    n = 0
    for a in actors:
        comps = a.get_components_by_class(unreal.LightComponent)
        if not comps:
            continue
        loc = a.get_actor_location()
        if not (Y0 <= loc.y <= Y1):
            continue
        c = comps[0]
        col = prop(c, "light_color")
        colstr = f"({col.r},{col.g},{col.b})" if col else "-"
        log(f"{a.get_actor_label()[:25]:<26} {a.get_class().get_name()[:17]:<18} "
            f"{prop(c, 'intensity', 0.0):>10.1f} "
            f"{prop(c, 'attenuation_radius', 0.0):>8.0f} "
            f"{colstr:>18} {loc.y:>7.0f}")
        n += 1
    log(f"{n} lights in the 4C band (Y {Y0:.0f}-{Y1:.0f})")

    log("-" * 92)
    for cls, label in ((unreal.DirectionalLight, "DirectionalLight"),
                       (unreal.SkyLight, "SkyLight"),
                       (unreal.PostProcessVolume, "PostProcessVolume"),
                       (unreal.ExponentialHeightFog, "HeightFog")):
        found = [a for a in actors if isinstance(a, cls)]
        log(f"{label:<20} {len(found)} in level"
            + (f"  -> {', '.join(x.get_actor_label() for x in found[:4])}"
               if found else ""))

    for a in actors:
        if not isinstance(a, unreal.PostProcessVolume):
            continue
        s = a.get_editor_property("settings")
        log(f"PPV {a.get_actor_label()}: unbound="
            f"{a.get_editor_property('unbound')}")
        for f in ("override_auto_exposure_method", "auto_exposure_method",
                  "override_auto_exposure_bias", "auto_exposure_bias",
                  "override_auto_exposure_min_brightness",
                  "auto_exposure_min_brightness",
                  "override_auto_exposure_max_brightness",
                  "auto_exposure_max_brightness"):
            try:
                log(f"    {f:<40} {s.get_editor_property(f)}")
            except Exception:
                pass

    log("DONE")


run()
