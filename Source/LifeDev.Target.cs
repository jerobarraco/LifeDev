// Copyright Jeronimo Barraco-Marmol

using UnrealBuildTool;

public class LifeDevTarget : TargetRules {
	public LifeDevTarget(TargetInfo Target) : base(Target) {
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;
		// OptimizationLevel = OptimizationMode.SizeAndSpeed;
		ExtraModuleNames.Add("LifeDev");

		// https://forums.unrealengine.com/t/how-to-compile-in-non-unity-mode/94863/3?u=nande
		// https://dev.epicgames.com/documentation/en-us/unreal-engine/build-configuration-for-unreal-engine
		// bWarningsAsErrors = false;
		// bAllCores = true;
		// bUsePCHFiles = false;
		bUsePDBFiles = false;
		//bEnableAddressSanitizer
		CppStandardEngine = CppStandardVersion.Cpp20;
		CppStandard = CppStandardVersion.Cpp20;
		
		// ---- speed iteration
		bUseIncrementalLinking = true; // speeds up iteration
		bUseUnityBuild = false;  // with 32 cores this is faster to iterate changes. it might be slower when making a build though.
		bUseAdaptiveUnityBuild = true;
		bAdaptiveUnityDisablesOptimizations = true;
		bAdaptiveUnityCreatesDedicatedPCH = true;
		// bAdaptiveUnityDisablesPCH = true;
		bUseSharedPCHs = true; // speed up compilation

		// to enable logs on shipping https://dev.epicgames.com/community/learning/knowledge-base/vzvZ/unreal-engine-enabling-logging-in-shipping-builds
		if (Target.Platform == UnrealTargetPlatform.Linux) {
			BuildEnvironment = TargetBuildEnvironment.UniqueIfNeeded;
			bUseLoggingInShipping = true;
			// bUseExecCommnadInShipping
		} else if (Target.Platform == UnrealTargetPlatform.Win64) {
			// can't do on windows because i have an "Installed" build. which can only do shared.
			// https://forums.unrealengine.com/t/targets-with-a-unique-build-environment-cannot-be-built-with-an-installed-engine/1353217/2
			BuildEnvironment = TargetBuildEnvironment.Shared;
		}
	}
}
