import unreal

MAP = "/Game/Maps/FiveLoops"

if unreal.EditorAssetLibrary.does_asset_exist(MAP):
    unreal.EditorLevelLibrary.load_level(MAP)
else:
    unreal.EditorLevelLibrary.new_level(MAP)

world_settings = unreal.get_editor_world().get_world_settings()
if world_settings:
    world_settings.set_editor_property("enable_world_composition", False)

unreal.EditorLevelLibrary.save_current_level()
unreal.log("Rewind: saved " + MAP)
