#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

DEFINE_LOG_CATEGORY_STATIC(LogSounds, Log, Log)

class SOUNDS_API FSoundsModule : public IModuleInterface {
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
