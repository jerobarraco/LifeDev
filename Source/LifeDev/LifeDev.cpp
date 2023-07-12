// Copyright Jeronimo Barraco-Marmol


#include "LifeDev.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FLifeDevModule, LifeDev, "LifeDev" );

// IMPLEMENT_MODULE(FLifeDevModule, LifeDev)
void FLifeDevModule::StartupModule()
{
	IModuleInterface::StartupModule();
}

void FLifeDevModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}
