// Copyright Jeronimo Barraco-Marmol

using UnrealBuildTool;

public class LifeDevEditorTarget : TargetRules {
	public LifeDevEditorTarget(TargetInfo Target) : base(Target) {
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_6;
		ExtraModuleNames.Add("LifeDev");
		bAllowHotReload = true;

		CppStandardEngine = CppStandardVersion.Cpp20;
		CppStandard = CppStandardVersion.Cpp20;
		// https://forums.unrealengine.com/t/ue-5-5-source-linux-compilation-errors/2148769/2?u=nande
		// Disable Intel Implicit SPMD Program Compiler forced by UE 5.5 on Linux.
		// bCompileISPC = true; // setting to false could fix some errors on linux, but disabling uba fixes it too. and also disabling this will require an engine recompilation.

		bUseIncrementalLinking = true; // speeds up iteration
		bUsePCHFiles = true;
		bUseSharedPCHs = true; // speed up compilation
		bUseUnityBuild = true;
		bUseAdaptiveUnityBuild = true;

		// https://forums.unrealengine.com/t/disabling-pdb-debug-gen-in-development-configuration-engine-source/617448/4
		if(Configuration == UnrealTargetConfiguration.Shipping) { 
			// disable pdb. it takes time and space. and i test builds. if someone has an issue ask to re-test with a debug build with logs and all.
			bOmitPCDebugInfoInDevelopment = true;
		}
	}
}
