// Copyright Jeronimo Barraco-Marmol 2023. All rights reserved.

using UnrealBuildTool;

public class LifeDev : ModuleRules {
	public LifeDev(ReadOnlyTargetRules Target) : base(Target) {
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput",
			"UMG", "DeveloperSettings",
		});

		PrivateDependencyModuleNames.AddRange(new string[] {
			// general
			"ChaosNiagara", "Niagara", "GameplayCameras",
			"MoviePlayer",  // loading screen yes.
			"SlateCore", "Slate", // settings
			"AudioMixer", // for musicman fx fade
			// "MetasoundEngine", // for the music
			"RenderCore", // to set the shaders' path
			// my stuff
			"JUtils", "Interact", "Diags", "Inventory", "Story",
			"Sounds", "JSig",
			"Sentry"
		});
		// force user dir. this saves headaches when managing demo/playtest/release builds through steam.
		// it also saves a lot of headaches to me when developing multiple versions
		// https://forums.unrealengine.com/t/setting-global-preprocessor-definitions-for-a-module/339593/2?u=nande
		PublicDefinitions.Add("UE_SAVED_DIR_OVERRIDE=./MyData");
		PrivateDefinitions.Add("UE_SAVED_DIR_OVERRIDE=./MyData");
		PrivateDefinitions.Add("LD_DEMO=1");
	}
}
