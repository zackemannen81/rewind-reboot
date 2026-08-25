"""REW-0030: build Apartment 4C's shell at the concept's dimensions.

Apartment 4C had no shell. inspect_4c_shell found no floor, no walls and no
ceiling anywhere near it -- every cube in range is a small fixture (counter,
fusebox plate, balcony lintel, four thin posts). The only enclosure was Cube8,
the 3291x3895x5000 building envelope the whole level sits inside. So the room
read wrong for a reason that no amount of lighting or texturing would fix:
there was no room, only a camera volume and props standing against an imagined
wall line.

Concept topology, using the owner's 1.8 m player as the metric:
  interior  X [-260, 240]   500 deep, open toward the camera (no fourth wall)
            Y [853, 2153]  1300 wide, four bays of 325
            Z [1200, 1580]  380 tall
  four bays with structural columns on the interior boundaries
  the door stays where it is, at Y 1048 in bay 1
  the balcony opening moves to bay 4, centred at Y 1990

The camera moves back to frame the wider room. Distance follows the rule the
stairwell regions already use -- width / (2 tan(fov/2)) plus a little margin --
so 1300 wide on the existing 37.5 degree lens needs about 1970, not 687.5.
That, not the lens, is why the shot felt too zoomed in.

Idempotent: everything it creates is labelled 4C_Shell_* and deleted on re-run.

Run with the editor closed:
    UnrealEditor-Cmd.exe <uproject> -ExecutePythonScript="<this file>"
      -unattended -nopause -nosplash -NullRHI
"""
import unreal

MAP = "/Game/Maps/FiveLoops_Stairwell_Blockout"
CUBE = "/Engine/BasicShapes/Cube"          # 100 cm on a side, pivot centred
PREFIX = "4C_Shell_"

MAT = {
    "floor":  "/Game/Art/Materials/Surfaces/MI_REW_FloorTile_Interior",
    "wall":   "/Game/Art/Materials/Surfaces/MI_REW_PlasterGreen_Aged",
    "ceil":   "/Game/Art/Materials/Surfaces/MI_REW_PlasterGray_Aged",
    "column": "/Game/Art/Materials/Surfaces/MI_REW_BrickDark_Aged",
}

# Interior extents.
X0, X1 = -260.0, 240.0
Y0, Y1 = 853.0, 2153.0
Z0, Z1 = 1200.0, 1580.0
T = 20.0                                    # wall / slab thickness
BAY = (Y1 - Y0) / 4.0                       # 325

DOOR_Y0, DOOR_Y1 = 988.0, 1108.0            # keeps the authored door at 1048
DOOR_TOP = 1422.0                           # door is 222 tall once floor-snapped
BALC_Y0, BALC_Y1 = 1850.0, 2130.0           # centred in bay 4
BALC_TOP = 1480.0

# (label, centre, size, material key)
def shell():
    cx, sx = (X0 + X1) / 2.0, (X1 - X0)
    wx, wsx = (X0 - T + X1) / 2.0, (X1 - X0 + T)   # slabs run under the walls
    cy, sy = (Y0 + Y1) / 2.0, (Y1 - Y0)
    sy_full = (Y1 - Y0) + 2 * T
    cz = (Z0 + Z1) / 2.0
    sz = (Z1 - Z0)
    bwx = X0 - T / 2.0                              # back wall centre X

    out = [
        ("Floor",   (wx, cy, Z0 - T / 2.0), (wsx, sy_full, T), "floor"),
        ("Ceiling", (wx, cy, Z1 + T / 2.0), (wsx, sy_full, T), "ceil"),

        # Back wall, broken around the door and the balcony opening.
        ("Wall_Back_A", (bwx, (Y0 + DOOR_Y0) / 2.0, cz),
         (T, DOOR_Y0 - Y0, sz), "wall"),
        ("Wall_Back_DoorHead", (bwx, (DOOR_Y0 + DOOR_Y1) / 2.0,
                                (DOOR_TOP + Z1) / 2.0),
         (T, DOOR_Y1 - DOOR_Y0, Z1 - DOOR_TOP), "wall"),
        ("Wall_Back_B", (bwx, (DOOR_Y1 + BALC_Y0) / 2.0, cz),
         (T, BALC_Y0 - DOOR_Y1, sz), "wall"),
        ("Wall_Back_BalcHead", (bwx, (BALC_Y0 + BALC_Y1) / 2.0,
                                (BALC_TOP + Z1) / 2.0),
         (T, BALC_Y1 - BALC_Y0, Z1 - BALC_TOP), "wall"),
        ("Wall_Back_C", (bwx, (BALC_Y1 + Y1) / 2.0, cz),
         (T, Y1 - BALC_Y1, sz), "wall"),

        # End walls.
        ("Wall_End_Near", (wx, Y0 - T / 2.0, cz), (wsx, T, sz), "wall"),
        ("Wall_End_Far",  (wx, Y1 + T / 2.0, cz), (wsx, T, sz), "wall"),
    ]
    # Structural columns on the three interior bay boundaries, set 80 cm off
    # the back wall so they clear the fixtures mounted on it.
    for i in (1, 2, 3):
        out.append((f"Bay_Column_{i}", (-180.0, Y0 + BAY * i, cz),
                    (40.0, 30.0, sz), "column"))
    return out


# Where each prop belongs, expressed as TARGET BOUNDS rather than a delta.
# A delta script is not idempotent -- run it twice and everything moves twice.
# Converging on a target means a re-run is a no-op, which matters because this
# is the script the room gets rebuilt from.
#
# (label, centre X, centre Y, min Z, max Z); None means leave that axis alone.
PLACE_BOUNDS = [
    # Floor contact. These floated 20-110 cm, invisible until there was a
    # floor under them.
    ("Meshy_AI_sofa_3d_0825162856_image-to-3d-texture", None, 1665.0, Z0, None),
    ("Meshy_AI_kitchen_table_3d_0825162406_image-to-3d-texture",
     None, 928.0, Z0, None),
    ("Meshy_AI_door_3d_0825162626_image-to-3d-texture", None, 1048.0, Z0, None),
    ("Meshy_AI_railing_plant_3d_0825162751_image-to-3d-texture",
     None, 1990.0, Z0, None),
    # The footstool sat inside bay column 2 and beside the sofa rather than in
    # front of it. +X is toward the camera, so "in front" is +X, not +Y.
    ("Meshy_AI_footstool_3d_0825175833_image-to-3d-texture",
     90.0, 1665.0, Z0, None),
    # The shelf, table and near lamp overhung the end wall at Y 853.
    ("Meshy_AI_shelf_3d_0825175810_image-to-3d-texture", None, 955.0, None, None),
    # Pendant lamps hung from Z 1740, straight through the new ceiling.
    ("Meshy_AI_pendant_lamp_3d_0825174536_image-to-3d-texture",
     None, 910.0, None, Z1),
    ("Meshy_AI_pendant_lamp_3d_0825174536_image-to-3d-texture2",
     None, 1655.0, None, Z1),
    # Kitchen counter and its splashback, both overhanging the end wall and
    # the counter floating 60 cm up.
    ("Cube", None, 913.0, Z0, None),
    ("Cube4", None, 913.0, None, None),
]

# Actors placed outright, in world space.
PLACE_ACTOR = {
    # The two loose posts became the balcony reveals; the wall now carries the
    # opening they used to imply.
    "Cube10": ((-250.0, BALC_Y0, (Z0 + Z1) / 2.0),
               (0.20, 0.20, (Z1 - Z0) / 100.0)),
    "Cube9":  ((-250.0, BALC_Y1, (Z0 + Z1) / 2.0),
               (0.20, 0.20, (Z1 - Z0) / 100.0)),
    "Cube7":  ((-260.0, (BALC_Y0 + BALC_Y1) / 2.0, 1454.0), None),
    # Bulbs follow their lamps down.
    "PointLight4": ((-60.0, 920.0, 1320.0), None),
    "PointLight5": ((-190.0, 1655.0, 1320.0), None),
    # Balcony wash follows the opening into bay 4.
    "SpotLight2":    ((-320.0, 1990.0, 1320.0), None),
    "4C_Rear_Warm":  ((-300.0, 1950.0, 1375.0), None),
    # The spawn point sat inside the sofa once the sofa moved into bay 3.
    "Stairwell_PlayerStart": ((50.0, 1300.0, 1296.0), None),
}

# Redundant once the shell exists: both sit entirely inside a bay column.
DELETE = ["Cube5", "Cube6"]

REGION = "Apartment4C_Region"
FOV = 37.5
MARGIN = 1.03           # the stairwell regions frame ~3% wider than the volume


def log(m):
    unreal.log("[REW-0030] " + str(m))


def run():
    import math
    unreal.EditorLoadingAndSavingUtils.load_map(MAP)
    sub = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    cube = unreal.EditorAssetLibrary.load_asset(CUBE)
    if not cube:
        log(f"FATAL: no cube at {CUBE}")
        return
    mats = {k: unreal.EditorAssetLibrary.load_asset(v) for k, v in MAT.items()}
    for k, v in mats.items():
        if not v:
            log(f"  WARNING: no material for {k} at {MAT[k]}")

    by_label = {}
    for a in sub.get_all_level_actors():
        by_label.setdefault(a.get_actor_label(), a)

    # Idempotent: clear anything a previous run created.
    gone = 0
    for label, a in list(by_label.items()):
        if label.startswith(PREFIX):
            sub.destroy_actor(a)
            del by_label[label]
            gone += 1
    if gone:
        log(f"removed {gone} actors from a previous run")

    made = 0
    for name, centre, size, key in shell():
        loc = unreal.Vector(*centre)
        a = sub.spawn_actor_from_object(cube, loc, unreal.Rotator(0, 0, 0))
        if not a:
            log(f"  FAILED to spawn {name}")
            continue
        a.set_actor_label(PREFIX + name)
        a.set_actor_scale3d(unreal.Vector(size[0] / 100.0, size[1] / 100.0,
                                          size[2] / 100.0))
        m = mats.get(key)
        if m:
            a.static_mesh_component.set_material(0, m)
        made += 1
    log(f"built {made} shell pieces")

    # Converge each prop on its target bounds.
    for label, tx, ty, tminz, tmaxz in PLACE_BOUNDS:
        a = by_label.get(label)
        if not a:
            log(f"  place: no actor {label}")
            continue
        origin, extent = a.get_actor_bounds(False)
        d = unreal.Vector(0.0, 0.0, 0.0)
        if tx is not None:
            d.x = tx - origin.x
        if ty is not None:
            d.y = ty - origin.y
        if tminz is not None:
            d.z = tminz - (origin.z - extent.z)
        elif tmaxz is not None:
            d.z = tmaxz - (origin.z + extent.z)
        if abs(d.x) + abs(d.y) + abs(d.z) < 0.5:
            continue
        loc = a.get_actor_location()
        a.set_actor_location(
            unreal.Vector(loc.x + d.x, loc.y + d.y, loc.z + d.z), False, False)
        log(f"  moved {label[:40]:<40} ({d.x:+6.0f},{d.y:+6.0f},{d.z:+6.0f})")

    # The counter is 40 cm tall, which is a step, not a worktop.
    counter = by_label.get("Cube")
    if counter:
        sc = counter.get_actor_scale3d()
        counter.set_actor_scale3d(unreal.Vector(sc.x, sc.y, 0.90))
        o, e = counter.get_actor_bounds(False)
        loc = counter.get_actor_location()
        counter.set_actor_location(
            unreal.Vector(loc.x, loc.y, loc.z + (Z0 - (o.z - e.z))), False, False)

    for label, (loc, scale) in PLACE_ACTOR.items():
        a = by_label.get(label)
        if not a:
            log(f"  place: no actor {label}")
            continue
        a.set_actor_location(unreal.Vector(*loc), False, False)
        if scale:
            a.set_actor_scale3d(unreal.Vector(*scale))

    for label in DELETE:
        a = by_label.get(label)
        if a:
            sub.destroy_actor(a)
            log(f"  deleted {label} (inside a bay column)")

    # Widen the camera region and pull the camera back to frame it.
    region = by_label.get(REGION)
    if region:
        half_w = (Y1 - Y0) / 2.0
        dist = (half_w * MARGIN) / math.tan(math.radians(FOV / 2.0))
        # Keep the X boundary with TopHall at -245 so the two volumes still
        # meet exactly; a gap there is what stranded the controller in REW-0015.
        cx = (-245.0 + X1) / 2.0
        region.set_actor_location(
            unreal.Vector(cx, (Y0 + Y1) / 2.0, (Z0 + Z1) / 2.0), False, False)
        region.set_editor_property(
            "PlayerVolumeExtent",
            unreal.Vector((X1 + 245.0) / 2.0, half_w, (Z1 - Z0) / 2.0 + 20.0))
        region.set_editor_property(
            "CameraOffset", unreal.Vector(dist, 0.0, 0.0))
        region.set_editor_property("FieldOfView", FOV)
        log(f"region centre=({cx:.1f},{(Y0+Y1)/2:.1f},{(Z0+Z1)/2:.1f}) "
            f"extent=({(X1+245.0)/2:.1f},{half_w:.1f},{(Z1-Z0)/2+20:.1f}) "
            f"camera offset X={dist:.1f} fov={FOV}")
    else:
        log(f"  FATAL: no {REGION} in the map")

    saved = unreal.EditorLoadingAndSavingUtils.save_dirty_packages(True, True)
    log(f"save_dirty_packages -> {saved}")
    log("DONE")


run()
