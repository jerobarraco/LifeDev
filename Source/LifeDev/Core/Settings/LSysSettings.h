#pragma once

#include "CoreMinimal.h"

#include "LSysSettings.generated.h"
UCLASS(Config=LifeDevSysSettings, DefaultConfig, meta =(DisplayName="LifeDev System Settings"))
class LIFEDEV_API ULSysSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	ULSysSettings* Get();
};