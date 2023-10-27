// Copyright Jeronimo Barraco-Marmol 2023. All rights reserved.

using UnrealBuildTool;

public class LifeDev : ModuleRules
{
	public LifeDev(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput",
			"UMG", "DeveloperSettings",
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			// general
			"ChaosNiagara", "Niagara", "GameplayCameras",
			"MoviePlayer",  // loading screen yes.
			// my stuff
			"JUtils", "Interact",  "Diags", "Inventory", "Story",
			"Sounds", "JSig"
			
        });
	}
}
