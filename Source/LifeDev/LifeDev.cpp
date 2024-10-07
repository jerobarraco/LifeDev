// Copyright Jeronimo Barraco-Marmol

#include "LifeDev.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FLifeDevModule, LifeDev, "LifeDev" );

// IMPLEMENT_MODULE(FLifeDevModule, LifeDev)
void FLifeDevModule::StartupModule()
{
	IModuleInterface::StartupModule();
	
	const FString& ShaderDirectory = FPaths::Combine(FPaths::ProjectDir(), TEXT("Shaders"));
	UE_LOG(LogTemp, Log, TEXT("LifeDev Shaders path '%s'"), *ShaderDirectory);
	// https://biq.medium.com/configuring-unreal-engine-for-custom-shader-development-biq-cf79f72e7137
	AddShaderSourceDirectoryMapping(TEXT("/Project/Shaders/LD"),
		ShaderDirectory);
	
	FSlateApplication::Get().SetApplicationScale(1.3);
}

void FLifeDevModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}
