"""Create the Chapter 1 radio attenuation asset without opening a map.

Digit credit is limited by ARewindRadio::RadioRange (320 cm). The attenuation
falloff ends at that same distance so no clearly audible digit can be outside
the gameplay hearing range.
"""

import unreal

ASSET_PATH = "/Game/Audio/Chapter1/A_REW_Radio_Attenuation"
FALLOFF_DISTANCE_CM = 320.0

asset_lib = unreal.EditorAssetLibrary
if asset_lib.does_asset_exist(ASSET_PATH):
    attenuation = asset_lib.load_asset(ASSET_PATH)
else:
    attenuation = unreal.AssetToolsHelpers.get_asset_tools().create_asset(
        "A_REW_Radio_Attenuation",
        "/Game/Audio/Chapter1",
        unreal.SoundAttenuation,
        unreal.SoundAttenuationFactory(),
    )

settings = attenuation.get_editor_property("attenuation")
settings.set_editor_property("attenuate", True)
settings.set_editor_property("spatialize", True)
settings.set_editor_property("attenuation_shape", unreal.AttenuationShape.SPHERE)
settings.set_editor_property("distance_algorithm", unreal.AttenuationDistanceModel.LINEAR)
settings.set_editor_property("attenuation_shape_extents", unreal.Vector(0.0, 0.0, 0.0))
settings.set_editor_property("falloff_distance", FALLOFF_DISTANCE_CM)
attenuation.set_editor_property("attenuation", settings)
asset_lib.save_asset(ASSET_PATH, only_if_is_dirty=False)
saved_settings = attenuation.get_editor_property("attenuation")
saved_falloff = saved_settings.get_editor_property("falloff_distance")
if saved_falloff != FALLOFF_DISTANCE_CM:
    raise RuntimeError("Expected {} cm falloff, got {}".format(FALLOFF_DISTANCE_CM, saved_falloff))
unreal.log("REW-0035: saved {} with {} cm falloff".format(ASSET_PATH, saved_falloff))
