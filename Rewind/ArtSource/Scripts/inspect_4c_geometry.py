"""Inventory Apartment 4C's geometry before enlarging it.

The room is authored in the map, not in code, so growing it means knowing
exactly which actors form it and where they sit. Reports every actor whose
bounds intersect the 4C volume, with transform and size, so the enlargement
can move real things rather than guess.

4C's player volume today: X [-245, 370), Y [853, 1883), Z [1180, 1500).

Run with the editor closed:
    UnrealEditor-Cmd.exe <uproject> -ExecutePythonScript="<this file>"
      -unattended -nopause -nosplash -NullRHI
"""
import unreal

MAP = "/Game/Maps/FiveLoops_Stairwell_Blockout"
# Generous bounds: catch anything near 4C, including the corridor wall.
LO = unreal.Vector(-700.0, 600.0, 1100.0)
HI = unreal.Vector(700.0, 2200.0, 1600.0)


def log(m):
    unreal.log("[REW-0030] " + str(m))


def inside(v):
    return (LO.x <= v.x <= HI.x and LO.y <= v.y <= HI.y and LO.z <= v.z <= HI.z)


def run():
    unreal.EditorLoadingAndSavingUtils.load_map(MAP)
    sub = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    actors = sub.get_all_level_actors()
    log(f"map loaded, {len(actors)} actors total")

    hits = []
    for a in actors:
        try:
            loc = a.get_actor_location()
        except Exception:
            continue
        if not inside(loc):
            continue
        try:
            origin, extent = a.get_actor_bounds(False)
        except Exception:
            origin, extent = loc, unreal.Vector(0, 0, 0)
        hits.append((a, loc, origin, extent))

    log(f"{len(hits)} actors inside the 4C region")
    log("-" * 100)
    for a, loc, origin, extent in sorted(hits, key=lambda h: (h[1].y, h[1].x)):
        cls = a.get_class().get_name()
        label = a.get_actor_label()
        scale = a.get_actor_scale3d()
        log(f"{label:<34} {cls:<26} "
            f"loc=({loc.x:8.1f},{loc.y:8.1f},{loc.z:7.1f}) "
            f"scale=({scale.x:6.2f},{scale.y:6.2f},{scale.z:6.2f}) "
            f"size=({extent.x*2:7.1f},{extent.y*2:7.1f},{extent.z*2:7.1f})")

    log("-" * 100)
    # Camera regions carry the player volume that must grow with the room.
    for a in actors:
        if "CameraRegion" not in a.get_class().get_name():
            continue
        try:
            name = a.get_editor_property("RegionName")
            ext = a.get_editor_property("PlayerVolumeExtent")
            off = a.get_editor_property("CameraOffset")
            fov = a.get_editor_property("FieldOfView")
            loc = a.get_actor_location()
            log(f"REGION {str(name):<22} centre=({loc.x:7.1f},{loc.y:7.1f},{loc.z:7.1f}) "
                f"extent=({ext.x:6.1f},{ext.y:6.1f},{ext.z:6.1f}) "
                f"offset=({off.x:7.1f},{off.y:7.1f},{off.z:6.1f}) fov={fov:.3f}")
        except Exception as e:
            log(f"  region read failed: {e}")

    log("DONE")


run()
