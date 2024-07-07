// Copyright Jeronimo Barraco-Marmol

using UnrealBuildTool;
using System.Collections.Generic;

public class LifeDevEditorTarget : TargetRules
{
	public LifeDevEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		ExtraModuleNames.Add("LifeDev");
	}
}
