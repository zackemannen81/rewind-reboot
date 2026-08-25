"""REW-0030: a lighting pass for Apartment 4C.

The level runs on MANUAL exposure at -0.7 EV -- an unbound PostProcessVolume,
no auto-exposure, no adaptation. So the intensities are absolute, and the two
pendant lamps meant to light the apartment were authored at 1.4 and 2.3 while
their neighbours sit at 110 and 160. The room rendered black because it was
black, not because the capture was wrong.

Values are candelas. A domestic bulb is roughly 800 lumens, and an isotropic
source at 800 lm is about 64 cd, so the pendants land near 70 rather than near
1. The cold and magenta accents stay accents: bright enough to colour a wall,
never bright enough to light the room, because the room should be lit by the
lamps hanging in it.

Absolute values throughout, so re-running converges instead of compounding.

Run with the editor closed:
    UnrealEditor-Cmd.exe <uproject> -ExecutePythonScript="<this file>"
      -unattended -nopause -nosplash -NullRHI
"""
import unreal

MAP = "/Game/Maps/FiveLoops_Stairwell_Blockout"

# label -> intensity (cd), colour or None to keep, attenuation or None to keep
LIGHTS = {
    # The two practicals that actually light the apartment.
    "PointLight4": (130.0, (255, 206, 152), 760.0),
    "PointLight5": (130.0, (255, 206, 152), 760.0),
    # Warm bounce near the radio desk, and the near-door fill.
    # Was 40 cm off the back wall, so it burned a white disc into the middle
    # of the shot and read as blue only because the scene's cool fill coloured
    # the falloff around the clipped core. Pulled into the room and dimmed.
    "PointLight2": (12.0, (255, 186, 130), 420.0),
    "PointLight":  (16.0, None, 320.0),
    # Cold accent. It was the brightest thing in the room and sat 30 cm under
    # the ceiling, so it painted a blue ellipse across the slab like a UFO.
    # 48 cd with a 4.2 m radius, 3.6 m from the back wall, still painted a
    # blue disc across the middle of the shot and a wash over the ceiling. An
    # accent has to lose to the lamps or it is not an accent.
    "4C_Radio_Cool": (14.0, None, 260.0),
    # City spill through the balcony, and the warm wash behind it.
    "SpotLight2":   (46.0, None, None),
    "4C_Rear_Warm": (60.0, None, None),
    # Teal wash on the apartment door -- the shot's one cool key.
    "SpotLight": (30.0, None, None),
    # A pure-white 110 cd spot with a 10 m radius, washing the middle of the
    # room flat. Kept as a soft warm fill instead of a blowout.
    "SpotLight3": (34.0, (255, 226, 196), 780.0),
}

# The cold accent also moves off the ceiling.
MOVE = {"4C_Radio_Cool": 1350.0}

# label -> full world position, for lights that were badly placed rather than
# badly set.
MOVE_XY = {"PointLight2": (-120.0, 1354.0, 1300.0)}

# The pendant meshes are 190 cm tall and 83 wide, which reads as a factory
# shade in a domestic room.
LAMP_SCALE = 0.62
LAMPS = ["Meshy_AI_pendant_lamp_3d_0825174536_image-to-3d-texture",
         "Meshy_AI_pendant_lamp_3d_0825174536_image-to-3d-texture2"]

# A little sky fill so shadows read as dark rather than as holes.
# A cool sky fill is the owner's intent, but at 0.34 it was tinting every
# surface in the apartment and the warm practicals could not read against it.
SKY = ("Authored_CoolSkyFill", 0.16)


def log(m):
    unreal.log("[REW-light] " + str(m))


def run():
    unreal.EditorLoadingAndSavingUtils.load_map(MAP)
    sub = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    by_label = {}
    for a in sub.get_all_level_actors():
        by_label.setdefault(a.get_actor_label(), a)

    for label, (intensity, colour, atten) in LIGHTS.items():
        a = by_label.get(label)
        if not a:
            log(f"  no actor {label}")
            continue
        comps = a.get_components_by_class(unreal.LightComponent)
        if not comps:
            log(f"  {label} has no light component")
            continue
        c = comps[0]
        # Mark the actor modified BEFORE touching the component. Setting a
        # component property from Python does not dirty the owning map
        # package, so save_dirty_packages finds nothing to write, returns True,
        # and the change is silently lost. A colour-only run of this script did
        # exactly that: the log said it worked and the render was byte-for-byte
        # identical to the previous one.
        a.modify()
        before = c.get_editor_property("intensity")
        units = "?"
        try:
            c.set_editor_property(
                "intensity_units", unreal.LightUnits.CANDELAS)
            units = "cd"
        except Exception:
            pass
        c.set_editor_property("intensity", intensity)
        if colour:
            # set_light_color, not the raw property. Assigning light_color
            # directly does not push the value into the component's render
            # state, so the light keeps rendering its old colour.
            try:
                c.set_light_color(
                    unreal.LinearColor(colour[0] / 255.0, colour[1] / 255.0,
                                       colour[2] / 255.0, 1.0), True)
            except Exception:
                c.set_editor_property(
                    "light_color",
                    unreal.Color(colour[0], colour[1], colour[2], 255))
        if atten is not None:
            try:
                c.set_editor_property("attenuation_radius", atten)
            except Exception:
                pass
        got = c.get_editor_property("light_color")
        log(f"  {label:<16} {before:7.1f} -> {intensity:6.1f} {units}  "
            f"colour now ({got.r},{got.g},{got.b})")

    for label, z in MOVE.items():
        a = by_label.get(label)
        if a:
            loc = a.get_actor_location()
            a.set_actor_location(unreal.Vector(loc.x, loc.y, z), False, False)
            log(f"  {label} Z {loc.z:.0f} -> {z:.0f}")

    for label, xyz in MOVE_XY.items():
        a = by_label.get(label)
        if a:
            a.set_actor_location(unreal.Vector(*xyz), False, False)
            log(f"  {label} -> ({xyz[0]:.0f},{xyz[1]:.0f},{xyz[2]:.0f})")

    for label in LAMPS:
        a = by_label.get(label)
        if not a:
            log(f"  no lamp {label}")
            continue
        # Scale about the ceiling: a pendant is hung from the top, so shrinking
        # it around its pivot would leave it floating.
        _, e0 = a.get_actor_bounds(False)
        top0 = a.get_actor_bounds(False)[0].z + e0.z
        a.set_actor_scale3d(unreal.Vector(LAMP_SCALE, LAMP_SCALE, LAMP_SCALE))
        o1, e1 = a.get_actor_bounds(False)
        loc = a.get_actor_location()
        a.set_actor_location(
            unreal.Vector(loc.x, loc.y, loc.z + (top0 - (o1.z + e1.z))),
            False, False)
        log(f"  {label[-28:]} scaled to {LAMP_SCALE}")

    sky = by_label.get(SKY[0])
    if sky:
        # A SkyLight carries a SkyLightComponent, which derives from
        # LightComponentBase and NOT from LightComponent, so a LightComponent
        # query silently returns nothing and the sky is skipped without a word.
        comps = sky.get_components_by_class(unreal.SkyLightComponent)
        if comps:
            sky.modify()
            b = comps[0].get_editor_property("intensity")
            comps[0].set_editor_property("intensity", SKY[1])
            log(f"  {SKY[0]:<16} {b:7.2f} -> {SKY[1]:6.2f}")
    else:
        log(f"  no sky light {SKY[0]}")

    log(f"save -> {unreal.EditorLoadingAndSavingUtils.save_dirty_packages(True, True)}")
    log("DONE")


run()
