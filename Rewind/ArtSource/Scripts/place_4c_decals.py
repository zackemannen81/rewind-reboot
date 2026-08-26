"""REW-0030: dress Apartment 4C with the grime decal kit.

REW-0028 authored eighteen decal instances -- rust leaks, peeling plaster,
wall cracks -- and not one of them was ever placed. The room is a clean box
with a worn story, which is most of why it reads as new construction rather
than a flat someone has lived in badly.

Placement follows the building's own logic rather than scattering: water
stains under the balcony header where rain gets in, peeling plaster around
the door where hands and weather reach it, cracks radiating from the fusebox
and the bay columns where the structure works, grime pooled on the floor along
the walking line.

A DecalActor projects along its own -X axis. With no rotation it therefore
projects toward -X, which is exactly what the back wall needs; a floor decal
needs pitch +90 so that -X points down.

Idempotent: every actor is labelled 4C_Decal_* and cleared on re-run.

Run with the editor closed:
    UnrealEditor-Cmd.exe <uproject> -ExecutePythonScript="<this file>"
      -unattended -nopause -nosplash -NullRHI
"""
import unreal

MAP = "/Game/Maps/FiveLoops_Stairwell_Blockout"
DEC = "/Game/Art/Materials/Decals/Instances/MI_REW_Decal_{}"
PREFIX = "4C_Decal_"

WALL_X = -255.0      # just in front of the back wall face at -260
COL_X = -158.0       # just in front of the bay columns' room-facing face
FLOOR_Z = 1204.0     # just above the floor at 1200

WALL_ROT = (0.0, 0.0, 0.0)      # -X projection, straight into the back wall
FLOOR_ROT = (90.0, 0.0, 0.0)    # pitch up so -X points down

# (material, location, rotation, half-size (depth, width, height), roll)
DECALS = [
    # Water gets in around the balcony opening and runs down the wall.
    ("LeaksRust1", (WALL_X, 1880.0, 1440.0), WALL_ROT, (40, 60, 130), 0.0),
    ("LeaksRust3", (WALL_X, 2100.0, 1420.0), WALL_ROT, (40, 55, 150), 0.0),
    ("LeaksRust5", (WALL_X, 1990.0, 1500.0), WALL_ROT, (40, 130, 80), 0.0),
    # The kitchen corner: damp behind the counter, rust round the pipework.
    ("LeaksRust2", (WALL_X, 900.0, 1300.0), WALL_ROT, (40, 70, 110), 0.0),
    ("LeaksRust6", (WALL_X, 862.0, 1450.0), WALL_ROT, (40, 45, 120), 0.0),
    # Peeling plaster where hands and weather reach: around the door.
    ("PeelingPlaster1", (WALL_X, 1000.0, 1250.0), WALL_ROT, (40, 55, 90), 0.0),
    ("PeelingPlaster2", (WALL_X, 1120.0, 1380.0), WALL_ROT, (40, 60, 120), 0.0),
    ("PeelingPlaster3", (WALL_X, 1140.0, 1230.0), WALL_ROT, (40, 50, 70), 0.0),
    ("PeelingPlaster4", (WALL_X, 1700.0, 1330.0), WALL_ROT, (40, 90, 140), 0.0),
    # Cracks radiate from the fusebox and along the long wall run.
    ("WallCracks1", (WALL_X, 1230.0, 1400.0), WALL_ROT, (40, 70, 130), 0.0),
    ("WallCracks2", (WALL_X, 1330.0, 1300.0), WALL_ROT, (40, 80, 150), 0.0),
    ("WallCracks4", (WALL_X, 1560.0, 1450.0), WALL_ROT, (40, 90, 120), 0.0),
    ("WallCracks6", (WALL_X, 1450.0, 1240.0), WALL_ROT, (40, 60, 90), 0.0),
    # The columns carry the load, so they carry the cracking.
    ("WallCracks3", (COL_X, 1178.0, 1350.0), WALL_ROT, (30, 18, 150), 0.0),
    ("WallCracks5", (COL_X, 1503.0, 1300.0), WALL_ROT, (30, 18, 170), 0.0),
    ("WallCracks3", (COL_X, 1828.0, 1400.0), WALL_ROT, (30, 18, 140), 0.0),
    # Floor grime: the walking line, the door threshold, under the balcony.
    ("LeaksRust4", (-60.0, 1048.0, FLOOR_Z), FLOOR_ROT, (40, 110, 150), 0.0),
    ("LeaksRust2", (-120.0, 1990.0, FLOOR_Z), FLOOR_ROT, (40, 130, 170), 30.0),
    ("PeelingPlaster2", (-40.0, 1500.0, FLOOR_Z), FLOOR_ROT, (40, 160, 200), 65.0),
    ("LeaksRust6", (20.0, 1240.0, FLOOR_Z), FLOOR_ROT, (40, 140, 180), 15.0),
]

SORT_ORDER = 0

# A DecalComponent's DecalSize is read-only from Python, so size is expressed
# through the actor's scale instead -- scale multiplies the default box.
DEFAULT_DECAL_SIZE = (128.0, 256.0, 256.0)


def log(m):
    unreal.log("[REW-decal] " + str(m))


def run():
    unreal.EditorLoadingAndSavingUtils.load_map(MAP)
    sub = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)

    gone = 0
    for a in sub.get_all_level_actors():
        if a.get_actor_label().startswith(PREFIX):
            sub.destroy_actor(a)
            gone += 1
    if gone:
        log(f"cleared {gone} decals from a previous run")

    # Preload and hold references. Loading inside the loop is how a missing
    # path turns into a silently untextured result instead of an error.
    cache = {}
    for name in {d[0] for d in DECALS}:
        m = unreal.EditorAssetLibrary.load_asset(DEC.format(name))
        if m:
            cache[name] = m
        else:
            log(f"  MISSING decal material {DEC.format(name)}")
    log(f"loaded {len(cache)}/{len({d[0] for d in DECALS})} decal materials")
    if not cache:
        log("FATAL: no decal materials, nothing to place")
        return

    made = 0
    for i, (name, loc, rot, size, roll) in enumerate(DECALS):
        mat = cache.get(name)
        if not mat:
            continue
        a = sub.spawn_actor_from_class(
            unreal.DecalActor, unreal.Vector(*loc),
            unreal.Rotator(rot[0], rot[1], rot[2] + roll))
        if not a:
            log(f"  FAILED to spawn decal {i} ({name})")
            continue
        a.set_actor_label(f"{PREFIX}{i:02d}_{name}")
        d = a.decal
        d.set_decal_material(mat)
        a.set_actor_scale3d(unreal.Vector(
            size[0] / DEFAULT_DECAL_SIZE[0],
            size[1] / DEFAULT_DECAL_SIZE[1],
            size[2] / DEFAULT_DECAL_SIZE[2]))
        try:
            d.set_editor_property("sort_order", SORT_ORDER + i)
        except Exception:
            pass
        made += 1

    log(f"placed {made} of {len(DECALS)} decals")

    # Read back: a decal with no material still exists as an actor and still
    # looks like success in a spawn count.
    bad = 0
    for a in sub.get_all_level_actors():
        if not a.get_actor_label().startswith(PREFIX):
            continue
        m = a.decal.get_decal_material()
        if not m:
            log(f"  NO MATERIAL on {a.get_actor_label()}")
            bad += 1
    log(f"verify: {bad} decals without a material")

    log(f"save -> {unreal.EditorLoadingAndSavingUtils.save_dirty_packages(True, True)}")
    log("DONE")


run()
