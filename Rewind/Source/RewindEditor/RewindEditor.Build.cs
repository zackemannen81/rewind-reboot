// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class RewindEditor : ModuleRules
{
	public RewindEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"CoreUObject",
			"Engine",
			"InputCore",
			"ToolsetRegistry",
			"UnrealEd"
		});
	}
}
