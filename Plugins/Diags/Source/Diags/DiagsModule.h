
#pragma once

#include "CoreMinimal.h"

class DIAGS_API FDiagsModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
