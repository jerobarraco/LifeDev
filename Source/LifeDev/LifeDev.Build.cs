// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LifeDev : ModuleRules
{
	public LifeDev(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput",
			"UMG", "DeveloperSettings"
		});
		PrivateDependencyModuleNames.AddRange(new string[] { "Interact", "Dialogs", "JUtils" });
	}
}
