#include "SoundsModule.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_MODULE(FSoundsModule, Sounds)
void FSoundsModule::StartupModule()
{
	IModuleInterface::StartupModule();
}

void FSoundsModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}
