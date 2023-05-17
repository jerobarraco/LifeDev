
#pragma once

#include "CoreMinimal.h"

class INTERACT_API FInteractModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
