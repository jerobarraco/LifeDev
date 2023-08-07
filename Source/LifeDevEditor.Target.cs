// Copyright Jeronimo Barraco-Marmol

using UnrealBuildTool;
using System.Collections.Generic;

public class LifeDevEditorTarget : TargetRules
{
	public LifeDevEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_2;
		ExtraModuleNames.Add("LifeDev");
	}
}
