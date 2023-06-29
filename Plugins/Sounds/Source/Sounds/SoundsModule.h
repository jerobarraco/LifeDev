#pragma once

#include "CoreMinimal.h"

class SOUNDS_API FSoundsModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
