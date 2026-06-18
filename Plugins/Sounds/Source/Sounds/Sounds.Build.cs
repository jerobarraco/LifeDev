// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: GPL-2.0

using UnrealBuildTool;

public class Sounds: ModuleRules
{
	public Sounds(ReadOnlyTargetRules Target) : base(Target)
	{
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;
		DefaultBuildSettings = BuildSettingsVersion.V7;

		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(new string[] {
				// ... add public include paths required here ...
		});
		
		PrivateIncludePaths.AddRange(new string[] {
				// ... add other private include paths required here ...
		});
		
		// ... add other public dependencies that you statically link with here ...
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
		});
		
		// ... add private dependencies that you statically link with here ...	
		PrivateDependencyModuleNames.AddRange(new string[]{
			"CoreUObject", "Engine"
		});
		
		DynamicallyLoadedModuleNames.AddRange(new string[] {
			// ... add any modules that your module loads dynamically here ...
		});
		
		if (Target.bBuildEditor)
		{
			// PrivateDependencyModuleNames.AddRange(new string[]{ "UnrealEd"});
		}
	}
}
