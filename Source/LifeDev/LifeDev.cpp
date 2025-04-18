// Copyright Jeronimo Barraco-Marmol

#include "LifeDev.h"

#include "Modules/ModuleManager.h"
#include "ShaderCore.h"

IMPLEMENT_PRIMARY_GAME_MODULE(FLifeDevModule, LifeDev, "LifeDev");

void FLifeDevModule::StartupModule() {
	IModuleInterface::StartupModule();
	
	// https://biq.medium.com/configuring-unreal-engine-for-custom-shader-development-biq-cf79f72e7137
	const FString& ShaderDirectory = FPaths::Combine(FPaths::ProjectDir(), TEXT("Shaders"));
	const FString VirtualShaderDirectory = TEXT("/Project/Shaders/LD");
	// avoid crash on hotreload. yes. hotreload. and yes. a crash.
	if (AllShaderSourceDirectoryMappings().Contains(VirtualShaderDirectory)) {
		UE_LOG(LogTemp, Warning, TEXT("%hs Shader directory already exists. dir=%s"),
			__func__, *VirtualShaderDirectory);
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("%hs LifeDev Shaders path '%s'"), __func__, *ShaderDirectory);
	// disabled manually so that i don't need to copy it for building.
	// AddShaderSourceDirectoryMapping(VirtualShaderDirectory, ShaderDirectory);

	// this won't work. probably is overriden by the engine. just go to Editor settings > Appearance > Application Scale
	// FSlateApplication::Get().SetApplicationScale(1.3);
}

void FLifeDevModule::ShutdownModule() {
	// attempt to open the feedback url if any

	// this somehow got the packager crashing. TODO fix.
	// const ULSysSettings* const SSettings = ULSysSettings::Get();
	// if (LIKELY(IsValid(SSettings) && !SSettings->CloseURL.IsEmpty()))
		// FPlatformProcess::LaunchURL(*SSettings->CloseURL, NULL, NULL);
	// FPlatformProcess::LaunchURL(TEXT("https://forms.gle/aYCr8zRR3wWTomyu8"), NULL, NULL);
	IModuleInterface::ShutdownModule();
}

void FLifeDevModule::PreUnloadCallback() {
	IModuleInterface::PreUnloadCallback();
}
