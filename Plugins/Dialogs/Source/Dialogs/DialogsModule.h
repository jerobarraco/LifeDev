
#pragma once

#include "CoreMinimal.h"

class DIALOGS_API FDialogsModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
