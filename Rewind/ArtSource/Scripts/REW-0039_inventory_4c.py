"""Write a read-only Apartment 4C actor/asset/material baseline for REW-0039.

The script loads the authored default map, inspects it and exits without any
save call. Its JSON is evidence for the task's placement/collision/provenance
baseline, not a level-authoring input.
"""
import json
import os
import unreal


MAP = "/Game/Maps/FiveLoops_Stairwell_Blockout"
OUTPUT = os.path.join(
    unreal.Paths.project_dir(), "ArtSource", "Apartment4C", "REW-0039",
    "REW-0039_baseline_inventory.json")
ROOM_MIN = unreal.Vector(-400.0, 750.0, 1120.0)
ROOM_MAX = unreal.Vector(1200.0, 2300.0, 1650.0)
ASSET_ROOTS = (
    "/Game/Props/Chapter1",
    "/Game/Art/Materials/BuildingSlice",
    "/Game/Art/Materials/Surfaces",
    "/Game/Art/Textures/Surfaces",
    "/Game/Art/Materials/Decals/Instances",
)
MATERIAL_PARAMETER_NAMES = (
    "BaseColorTex", "NormalTex", "RoughnessTex", "Roughness", "TileSize",
    "Tint")


def log(message):
    unreal.log("[REW-0039-inventory] " + str(message))


def vector(value):
    return [round(float(value.x), 3), round(float(value.y), 3),
            round(float(value.z), 3)]


def rotator(value):
    return [round(float(value.pitch), 3), round(float(value.yaw), 3),
            round(float(value.roll), 3)]


def path(value):
    if not value:
        return None
    try:
        return value.get_path_name()
    except Exception:
        return str(value)


def enum_value(value):
    try:
        return value.name
    except Exception:
        return str(value)


def get_property(obj, name, default=None):
    try:
        return obj.get_editor_property(name)
    except Exception:
        return default


def overlaps(origin, extent):
    return not (
        origin.x + extent.x < ROOM_MIN.x or
        origin.x - extent.x > ROOM_MAX.x or
        origin.y + extent.y < ROOM_MIN.y or
        origin.y - extent.y > ROOM_MAX.y or
        origin.z + extent.z < ROOM_MIN.z or
        origin.z - extent.z > ROOM_MAX.z)


def location_inside(location):
    return (ROOM_MIN.x <= location.x <= ROOM_MAX.x and
            ROOM_MIN.y <= location.y <= ROOM_MAX.y and
            ROOM_MIN.z <= location.z <= ROOM_MAX.z)


def component_record(component):
    record = {
        "name": component.get_name(),
        "class": component.get_class().get_name(),
        "visible": bool(get_property(component, "visible", True)),
    }
    if isinstance(component, unreal.StaticMeshComponent):
        mesh = get_property(component, "static_mesh")
        try:
            collision_enabled = enum_value(component.get_collision_enabled())
        except Exception:
            collision_enabled = enum_value(
                get_property(component, "collision_enabled"))
        try:
            collision_profile = str(component.get_collision_profile_name())
        except Exception:
            collision_profile = get_property(
                component, "collision_profile_name", "")
        record.update({
            "static_mesh": path(mesh),
            "materials": [path(component.get_material(index)) for index in
                          range(component.get_num_materials())],
            "collision_enabled": collision_enabled,
            "collision_profile": collision_profile,
            "mobility": enum_value(get_property(component, "mobility")),
            "cast_shadow": bool(get_property(component, "cast_shadow", True)),
        })
    elif isinstance(component, unreal.SkeletalMeshComponent):
        record.update({
            "skeletal_mesh": path(get_property(component, "skeletal_mesh")),
            "materials": [path(component.get_material(index)) for index in
                          range(component.get_num_materials())],
            "collision_enabled": enum_value(
                get_property(component, "collision_enabled")),
        })
    elif isinstance(component, unreal.DecalComponent):
        record.update({
            "decal_material": path(get_property(component, "decal_material")),
            "decal_size": vector(get_property(
                component, "decal_size", unreal.Vector())),
        })
    elif isinstance(component, unreal.LightComponent):
        color = get_property(component, "light_color")
        record.update({
            "intensity": float(get_property(component, "intensity", 0.0)),
            "attenuation_radius": float(get_property(
                component, "attenuation_radius", 0.0)),
            "color": [int(color.r), int(color.g), int(color.b)] if color else None,
            "cast_shadows": bool(get_property(component, "cast_shadows", True)),
        })
    return record


def actor_record(actor):
    location = actor.get_actor_location()
    try:
        origin, extent = actor.get_actor_bounds(False)
    except Exception:
        origin, extent = location, unreal.Vector()
    components = []
    for component in actor.get_components_by_class(unreal.ActorComponent):
        if isinstance(component, (unreal.StaticMeshComponent,
                                  unreal.SkeletalMeshComponent,
                                  unreal.DecalComponent,
                                  unreal.LightComponent)):
            components.append(component_record(component))
    return {
        "label": actor.get_actor_label(),
        "name": actor.get_name(),
        "class": actor.get_class().get_name(),
        "location": vector(location),
        "rotation": rotator(actor.get_actor_rotation()),
        "scale": vector(actor.get_actor_scale3d()),
        "bounds_origin": vector(origin),
        "bounds_extent": vector(extent),
        "bounds_min": vector(origin - extent),
        "bounds_max": vector(origin + extent),
        "components": components,
    }


def material_parameters(asset):
    if not isinstance(asset, unreal.MaterialInstanceConstant):
        return {}
    output = {"parent": path(get_property(asset, "parent"))}
    for parameter in ("BaseColorTex", "NormalTex", "RoughnessTex"):
        value = unreal.MaterialEditingLibrary.get_material_instance_texture_parameter_value(
            asset, parameter)
        output[parameter] = path(value)
    output["Roughness"] = round(float(
        unreal.MaterialEditingLibrary.get_material_instance_scalar_parameter_value(
            asset, "Roughness")), 6)
    for parameter in ("TileSize", "Tint"):
        value = unreal.MaterialEditingLibrary.get_material_instance_vector_parameter_value(
            asset, parameter)
        output[parameter] = [round(float(value.r), 6),
                             round(float(value.g), 6),
                             round(float(value.b), 6),
                             round(float(value.a), 6)]
    return output


def asset_inventory():
    records = []
    for root in ASSET_ROOTS:
        for asset_path in unreal.EditorAssetLibrary.list_assets(
                root, recursive=True, include_folder=False):
            asset = unreal.EditorAssetLibrary.load_asset(asset_path)
            if not asset:
                records.append({"path": asset_path, "load_failed": True})
                continue
            record = {
                "path": asset_path,
                "class": asset.get_class().get_name(),
            }
            if isinstance(asset, unreal.StaticMesh):
                bounds = asset.get_bounds()
                record["mesh_bounds_origin"] = vector(bounds.origin)
                record["mesh_bounds_extent"] = vector(bounds.box_extent)
                record["material_slots"] = [
                    path(slot.material_interface) for slot in
                    get_property(asset, "static_materials", [])]
            record.update(material_parameters(asset))
            records.append(record)
    return sorted(records, key=lambda item: item["path"].lower())


def camera_inventory(actors):
    output = {
        "required_comparison": {
            "location": [1969.78, 1300.0, 1390.0],
            "rotation": [0.0, 180.0, 0.0],
            "horizontal_fov": 37.5,
        }
    }
    for actor in actors:
        label = actor.get_actor_label()
        if label == "Apartment4C_Region":
            output["region"] = {
                "location": vector(actor.get_actor_location()),
                "camera_offset": vector(get_property(
                    actor, "CameraOffset", unreal.Vector())),
                "field_of_view": float(get_property(
                    actor, "FieldOfView", 0.0)),
                "player_volume_extent": vector(get_property(
                    actor, "PlayerVolumeExtent", unreal.Vector())),
                "travel_axis": enum_value(get_property(actor, "TravelAxis")),
            }
        elif label == "Stairwell_PlayerStart":
            output["player_start"] = {
                "location": vector(actor.get_actor_location()),
                "rotation": rotator(actor.get_actor_rotation()),
            }
    return output


def run():
    unreal.EditorLoadingAndSavingUtils.load_map(MAP)
    subsystem = unreal.get_editor_subsystem(unreal.EditorActorSubsystem)
    all_actors = subsystem.get_all_level_actors()
    room_actors = []
    for actor in all_actors:
        try:
            location = actor.get_actor_location()
            origin, extent = actor.get_actor_bounds(False)
        except Exception:
            continue
        if overlaps(origin, extent) or location_inside(location):
            room_actors.append(actor_record(actor))

    report = {
        "task": "REW-0039",
        "map": MAP,
        "read_only": True,
        "room_query_min": vector(ROOM_MIN),
        "room_query_max": vector(ROOM_MAX),
        "map_actor_count": len(all_actors),
        "camera": camera_inventory(all_actors),
        "room_actors": sorted(room_actors, key=lambda item: (
            item["location"][1], item["location"][0], item["label"])),
        "assets": asset_inventory(),
    }
    os.makedirs(os.path.dirname(OUTPUT), exist_ok=True)
    with open(OUTPUT, "w", encoding="utf-8", newline="\n") as handle:
        json.dump(report, handle, indent=2, sort_keys=True)
        handle.write("\n")
    log(f"map actors={len(all_actors)} room records={len(room_actors)} "
        f"assets={len(report['assets'])}")
    log(f"wrote {OUTPUT} ({os.path.getsize(OUTPUT)} bytes)")
    log("DONE; no save call was made")


run()
