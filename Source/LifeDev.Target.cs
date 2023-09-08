// Copyright Jeronimo Barraco-Marmol

using UnrealBuildTool;
using System.Collections.Generic;

public class LifeDevTarget : TargetRules
{
	public LifeDevTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V4;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_3;
		ExtraModuleNames.Add("LifeDev");
	}
}
