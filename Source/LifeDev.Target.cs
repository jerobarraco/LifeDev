// Copyright Jeronimo Barraco-Marmol

using UnrealBuildTool;

public class LifeDevTarget : TargetRules {
	public LifeDevTarget(TargetInfo Target) : base(Target) {
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_6;
		// OptimizationLevel = OptimizationMode.SizeAndSpeed;
		ExtraModuleNames.Add("LifeDev");
		bAllowHotReload = true;

		// https://forums.unrealengine.com/t/how-to-compile-in-non-unity-mode/94863/3?u=nande
		// https://dev.epicgames.com/documentation/en-us/unreal-engine/build-configuration-for-unreal-engine
		// bWarningsAsErrors = false;
		// bUseIris = false; // can't be used, since windows will fail and request a "unique" build environment. which is not supported on binaries.
		// bAllCores = true;
		bUsePDBFiles = false;
		//bEnableAddressSanitizer
		CppStandardEngine = CppStandardVersion.Cpp20;
		CppStandard = CppStandardVersion.Cpp20;

		// https://forums.unrealengine.com/t/disabling-pdb-debug-gen-in-development-configuration-engine-source/617448/4
		if(Configuration == UnrealTargetConfiguration.Shipping) { 
			// disable pdb. it takes time and space. and i test builds. if someone has an issue ask to re-test with a debug build with logs and all.
			bOmitPCDebugInfoInDevelopment = true; // actually sentry re-enabled it somewhere. even if i remove it.
		} else {
			// disable warning as error
			DefaultWarningLevel = WarningLevel.Warning;
			bWarningsAsErrors = false;
		}

		// https://forums.unrealengine.com/t/ue-5-5-source-linux-compilation-errors/2148769/2?u=nande
		// Disable Intel Implicit SPMD Program Compiler forced by UE 5.5 on Linux.
		// bCompileISPC = true; // setting to false could fix some errors on linux, but disabling uba fixes it too. and also disabling this will require an engine recompilation.
		
		// ---- speed iteration
		bUseIncrementalLinking = true; // speeds up iteration
		bUsePCHFiles = true;
		bUseSharedPCHs = true; // speed up compilation
		bUseUnityBuild = true;
		bUseAdaptiveUnityBuild = true;
		/*
		bUseUnityBuild = false;  // with 32 cores this is faster to iterate changes. it might be slower when making a build though.
		bUseAdaptiveUnityBuild = false;
		bAdaptiveUnityDisablesOptimizations = true;
		bAdaptiveUnityCreatesDedicatedPCH = true;
		// bAdaptiveUnityDisablesPCH = true;
		*/

		// to enable logs on shipping https://dev.epicgames.com/community/learning/knowledge-base/vzvZ/unreal-engine-enabling-logging-in-shipping-builds
/*		if (Target.Platform == UnrealTargetPlatform.Linux) {
			BuildEnvironment = TargetBuildEnvironment.UniqueIfNeeded;
			bUseLoggingInShipping = true;
			// bUseExecCommnadInShipping
		} else if (Target.Platform == UnrealTargetPlatform.Win64) {
			// can't do on windows because i have an "Installed" build. which can only do shared.
			// https://forums.unrealengine.com/t/targets-with-a-unique-build-environment-cannot-be-built-with-an-installed-engine/1353217/2
			BuildEnvironment = TargetBuildEnvironment.Shared;
		}
		*/
	}
}
