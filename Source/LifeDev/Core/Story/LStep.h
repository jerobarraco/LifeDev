// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Story/Step.h"

#include "LStep.generated.h"

UCLASS(Blueprintable, BlueprintType)
class ALStep : public AStep {
	GENERATED_BODY()

public:
	virtual void Start_Implementation() override;
	virtual void Stop_Implementation() override;
	
	UPROPERTY(BlueprintReadWrite, Category=SetUp)
	bool DisablePlayer = false;
};
