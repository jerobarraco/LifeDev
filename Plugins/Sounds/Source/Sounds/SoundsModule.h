#pragma once

#include "Modules/ModuleInterface.h"
#include "Logging/LogCategory.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSounds, Log, All)

class SOUNDS_API FSoundsModule : public IModuleInterface {
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
