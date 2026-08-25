"""Find the geometry that actually forms Apartment 4C's shell.

inspect_4c_geometry tested actor PIVOTS against the 4C box, which finds the
props but misses the shell: a floor or wall mesh large enough to enclose the
room usually pivots outside it. This tests bounding-box OVERLAP instead, and
reports the mesh asset behind each actor so a "Cube" is identifiable as floor,
wall, column or lintel.

Run with the editor closed:
    UnrealEditor-Cmd.exe <uproject> -ExecutePythonScript="<this file>"
      -unattended -nopause -nosplash -NullRHI
"""
import unreal

MAP = "/Game/Maps/FiveLoops_Stairwell_Blockout"

# The 4C player volume padded by PAD on every side. Unpadded it finds no
# walls at all, and that is not because there are none: a wall sits just
# OUTSIDE the volume the player may stand in, so an exact-volume test excludes
# the very geometry it is looking for.
PAD = 320.0
LO = unreal.Vector(-245.0 - PAD, 853.0 - PAD, 1180.0 - PAD)
HI = unreal.Vector(370.0 + PAD, 1883.0 + PAD, 1500.0 + PAD)

# Anything bigger than this is the building envelope or a backdrop, not a
# wall of 4C; listing it drowns the result.
MAX_SIZE = 2000.0


def log(m):
    unreal.log("[REW-shell] " + str(m))


def overlaps(origin, extent):
    return not (origin.x + extent.x < LO.x or origin.x - extent.x > HI.x or
                origin.y + extent.y < LO.y or origin.y - extent.y > HI.y or
                origin.z + extent.z < LO.z or origin.z - extent.z > HI.z)


def mesh_of(actor):
    for comp in actor.get_components_by_class(unreal.StaticMeshComponent):
        m = comp.get_editor_property("static_mesh")
        if m:
            return m.get_name()
    return ""


def run():
    unreal.EditorLoadingAndSavingUtils.load_map(MAP)
    sub = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    actors = sub.get_all_level_actors()
    log(f"{len(actors)} actors in map")

    hits = []
    for a in actors:
        if not isinstance(a, unreal.StaticMeshActor):
            continue
        try:
            origin, extent = a.get_actor_bounds(False)
        except Exception:
            continue
        if extent.x <= 0 and extent.y <= 0 and extent.z <= 0:
            continue
        if max(extent.x, extent.y, extent.z) * 2 > MAX_SIZE:
            continue
        if overlaps(origin, extent):
            hits.append((a, origin, extent))

    log(f"{len(hits)} static meshes overlap the 4C volume")
    log(f"{'label':<30} {'mesh':<20} {'bounds min':>24} {'bounds max':>24}")
    log("-" * 104)
    for a, o, e in sorted(hits, key=lambda h: -(h[2].x * h[2].y * h[2].z)):
        lo = (o.x - e.x, o.y - e.y, o.z - e.z)
        hi = (o.x + e.x, o.y + e.y, o.z + e.z)
        log(f"{a.get_actor_label()[:29]:<30} {mesh_of(a)[:19]:<20} "
            f"({lo[0]:7.0f},{lo[1]:7.0f},{lo[2]:7.0f}) "
            f"({hi[0]:7.0f},{hi[1]:7.0f},{hi[2]:7.0f}) "
            f"size=({e.x*2:6.0f},{e.y*2:6.0f},{e.z*2:6.0f})")

    log("DONE")


run()
