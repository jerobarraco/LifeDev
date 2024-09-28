// Copyright Jeronimo Barraco-Marmol

#include "LifeDev.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FLifeDevModule, LifeDev, "LifeDev" );

// IMPLEMENT_MODULE(FLifeDevModule, LifeDev)
void FLifeDevModule::StartupModule()
{
	IModuleInterface::StartupModule();
	
	const FString& ShaderDirectory = FPaths::Combine(FPaths::ProjectDir(), TEXT("Shaders"));
	UE_LOG(LogTemp, Warning, TEXT("LD Shaders dir %s"), *ShaderDirectory);
	
	// Construct the virtual path shorthand.
	// FString VirtualShaderDirectory = FString::Printf(TEXT("/Project/LD"), *FMyShaderDevPluginModule::Name);
	// Create a mapping to the virtual shader directory shorthand.
	AddShaderSourceDirectoryMapping(TEXT("/Project/Shaders/LD"),
		ShaderDirectory);
}

void FLifeDevModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}
