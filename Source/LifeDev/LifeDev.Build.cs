// Copyright Jeronimo Barraco-Marmol 2023. All rights reserved.

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
		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Interact", "Dialogs", "JUtils", "Inventory", "Story",
			"ChaosNiagara", "Niagara", "GameplayCameras", "Sounds",
			"MoviePlayer",  // loading screen yes.
        });
	}
}
