// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include "CoreMinimal.h"

#include "Sounds/CSounder.h"

#include "CLSounder.generated.h"

// Base class for sounds on LD. use this instead of the CSounder
// uses the sfx class and attenuation.
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LIFEDEV_API UCLSounder : public UCSounder {
	GENERATED_BODY()

public:
	UCLSounder();
};
