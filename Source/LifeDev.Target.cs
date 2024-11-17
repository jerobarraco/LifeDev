// Copyright Jeronimo Barraco-Marmol

using UnrealBuildTool;

public class LifeDevTarget : TargetRules
{
	public LifeDevTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		// OptimizationLevel = OptimizationMode.SizeAndSpeed;
		// bUseExecCommnadInShipping
		ExtraModuleNames.Add("LifeDev");
		
		// to enable logs on shipping https://dev.epicgames.com/community/learning/knowledge-base/vzvZ/unreal-engine-enabling-logging-in-shipping-builds
		BuildEnvironment = TargetBuildEnvironment.Unique;
		bUseLoggingInShipping = true;
	}
}
