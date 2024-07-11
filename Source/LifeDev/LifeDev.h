// Copyright Jeronimo Barraco-Marmol

#pragma once

#include "CoreMinimal.h"

class LIFEDEV_API FLifeDevModule : public IModuleInterface {
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;	
};
