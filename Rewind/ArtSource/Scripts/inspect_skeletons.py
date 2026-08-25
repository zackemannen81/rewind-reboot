"""Report what the retarget actually needs, before building anything.

Two questions decide the approach and neither can be answered from disk,
because a .uasset is compressed:

1. Do the Quaternius and Returner skeletons share a bone naming convention?
   If they do, UE's compatible-skeleton mechanism can play the existing clips
   directly and no IK Retargeter is needed at all.
2. Which IK Rig and IK Retargeter Python entry points does this engine build
   actually expose? The API moved between 5.x releases and guessing wastes a
   headless run per attempt.

Run headless:
    UnrealEditor-Cmd.exe <uproject> -ExecutePythonScript="<this file>"
      -unattended -nopause -nosplash
"""
import unreal

QUAT_SKEL = "/Game/Characters/Tier1/UAL1/Tier1_UAL1/SkeletalMeshes/Tier1_UAL1_Skeleton"
QUAT_MESH = "/Game/Characters/Tier1/UAL1/Tier1_UAL1/SkeletalMeshes/Tier1_UAL1"
RET_SKEL = "/Game/Characters/Returner/Returner_Skeleton"
RET_MESH = "/Game/Characters/Returner/Returner"


def log(msg):
    unreal.log("[REW-0026] " + str(msg))


def bones_of(skeleton_path):
    sk = unreal.EditorAssetLibrary.load_asset(skeleton_path)
    if not sk:
        return None
    # Skeleton exposes its reference hierarchy through the bone tree.
    try:
        return [str(n) for n in sk.get_editor_property("bone_tree")]
    except Exception:
        pass
    # Fall back to the mesh, which reliably reports its reference skeleton.
    return None


def bones_from_mesh(mesh_path):
    mesh = unreal.EditorAssetLibrary.load_asset(mesh_path)
    if not mesh:
        return None
    try:
        return [str(b) for b in mesh.get_editor_property("skeleton").get_editor_property("bone_tree")]
    except Exception:
        pass
    try:
        return [str(b) for b in unreal.SkeletalMeshEditorSubsystem().get_bone_names(mesh)]
    except Exception:
        pass
    return None


def run():
    log("=" * 60)
    for label, skel, mesh in (
        ("QUATERNIUS", QUAT_SKEL, QUAT_MESH),
        ("RETURNER", RET_SKEL, RET_MESH),
    ):
        exists = unreal.EditorAssetLibrary.does_asset_exist(skel)
        log(f"{label} skeleton {skel} exists={exists}")
        names = bones_of(skel) or bones_from_mesh(mesh)
        if names:
            log(f"{label} bones ({len(names)}): {names}")
        else:
            log(f"{label} bones: could not read through the tried properties")

    log("=" * 60)
    # Which retarget APIs this build exposes. Names have moved between
    # releases, so report rather than assume.
    for name in (
        "IKRigDefinition", "IKRigDefinitionFactory", "IKRigController",
        "IKRetargeter", "IKRetargetFactory", "IKRetargeterFactory",
        "IKRetargeterController", "IKRetargetBatchOperation",
        "IKRetargetBatchOperationContext", "RetargetChainSettings",
    ):
        log(f"api {name}: {'yes' if hasattr(unreal, name) else 'no'}")

    log("=" * 60)
    # Animation sequences available to retarget.
    ar = unreal.AssetRegistryHelpers.get_asset_registry()
    found = ar.get_assets_by_path("/Game/Characters/Tier1", recursive=True)
    anims = [str(a.package_name) for a in found
             if str(a.asset_class_path.asset_name) == "AnimSequence"]
    log(f"Quaternius AnimSequences ({len(anims)}):")
    for a in sorted(anims):
        log("   " + a)

    found_r = ar.get_assets_by_path("/Game/Characters/Returner", recursive=True)
    log("Returner assets:")
    for a in found_r:
        log(f"   {a.package_name}  [{a.asset_class_path.asset_name}]")
    log("=" * 60)
    log("DONE")


run()
