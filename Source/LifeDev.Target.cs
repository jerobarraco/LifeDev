// Copyright Jeronimo Barraco-Marmol

using UnrealBuildTool;
using System.Collections.Generic;

public class LifeDevTarget : TargetRules
{
	public LifeDevTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_2;
		ExtraModuleNames.Add("LifeDev");
	}
}
