import unreal

MAP_PATH = "/Game/Maps"
MAP_NAME = "FiveLoops"
MAP = MAP_PATH + "/" + MAP_NAME

if unreal.EditorAssetLibrary.does_asset_exist(MAP):
    unreal.EditorAssetLibrary.delete_asset(MAP)

factory = unreal.WorldFactory()
asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
world = asset_tools.create_asset(MAP_NAME, MAP_PATH, unreal.World, factory)
if world is None:
    unreal.log_error("Rewind: failed to create " + MAP)
else:
    unreal.EditorAssetLibrary.save_asset(MAP)
    unreal.log("Rewind: created basic (non-partition) map " + MAP)
