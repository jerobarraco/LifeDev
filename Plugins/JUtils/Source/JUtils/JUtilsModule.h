#pragma once

#include "CoreMinimal.h"

class JUTILS_API FJUtilsModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
