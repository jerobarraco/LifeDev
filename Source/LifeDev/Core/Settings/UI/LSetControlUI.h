// Copyright (C) 2024 - Jeronimo Barraco-Marmol. All rights reserved.
#pragma once

#include "CoreMinimal.h"

#include "LSetBaseUI.h"

#include "LSetControlUI.generated.h"

// baseclass for the control settings
UCLASS(Blueprintable, BlueprintType)
class LIFEDEV_API ULSetControlUI : public ULSetBaseUI {
	GENERATED_BODY()

public:
	virtual void Apply_Implementation() override {};
	virtual void Load_Implementation() override {};
};
