// Copyright Jeronimo Barraco-Marmol

using UnrealBuildTool;

public class LifeDevTarget : TargetRules {
	public LifeDevTarget(TargetInfo Target) : base(Target) {
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;
		// OptimizationLevel = OptimizationMode.SizeAndSpeed;
		ExtraModuleNames.Add("LifeDev");
		
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
