// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

using UnrealBuildTool;

public class Interact: ModuleRules
{
	public Interact(ReadOnlyTargetRules Target) : base(Target)
	{
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_6;
		DefaultBuildSettings = BuildSettingsVersion.V5;

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
			"CoreUObject", "Engine",
			"Slate", // for AnimMat should be created
			"Niagara", "AudioMixer", // for the CAnimatorSFX
			"JUtils",
		});
		
		DynamicallyLoadedModuleNames.AddRange(new string[] {
			// ... add any modules that your module loads dynamically here ...
		});
		
		if (Target.bBuildEditor) {
			// PrivateDependencyModuleNames.AddRange(new string[]{});
		}
	}
}
