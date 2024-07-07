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
	}
}
