#include "SoundsModule.h"

#include "Modules/ModuleManager.h"

DEFINE_LOG_CATEGORY(LogSounds)

IMPLEMENT_MODULE(FSoundsModule, Sounds)
void FSoundsModule::StartupModule()
{
	IModuleInterface::StartupModule();
}

void FSoundsModule::ShutdownModule()
{
	IModuleInterface::ShutdownModule();
}
