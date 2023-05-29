// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LifeDev/Core/Story/LStep.h"

#include "LStep0000.generated.h"

UCLASS(Blueprintable, BlueprintType)
class ALStep0000 : public ALStep {
	GENERATED_BODY()

public:
	ALStep0000();
	
	virtual void Start_Implementation() override;
	virtual void Stop_Implementation() override;
};
