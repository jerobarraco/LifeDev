// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

using UnrealBuildTool;

public class JSig: ModuleRules
{
	public JSig(ReadOnlyTargetRules Target) : base(Target)
	{
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;
		DefaultBuildSettings = BuildSettingsVersion.V7;

		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(new string[] {
			// ... add public include paths required here ...
			//ModuleDirectory + "/Math"
		});
				
		PrivateIncludePaths.AddRange(new string[] {
				// ... add other private include paths required here ...
		});
			
		// ... add other public dependencies that you statically link with here ...
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			// public since it ends up being included indirectly due to the csignificance
			"SignificanceManager",
		});
		
		// ... add private dependencies that you statically link with here ...	
		PrivateDependencyModuleNames.AddRange(new string[]{
			"CoreUObject", "Engine",
		});
		
		DynamicallyLoadedModuleNames.AddRange(new string[] {
			// ... add any modules that your module loads dynamically here ...
		});
		
		if (Target.bBuildEditor) {
			// needed for GEditor
			PrivateDependencyModuleNames.AddRange(new string[]{ "UnrealEd"});
		}
	}
}
