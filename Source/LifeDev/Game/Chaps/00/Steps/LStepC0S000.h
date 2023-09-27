// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LifeDev/Core/Story/LStep.h"

#include "LStepC0S000.generated.h"

class UCRandomizer;

UCLASS(Blueprintable, BlueprintType)
class ALStepC0S000 : public ALStep {
	GENERATED_BODY()

public:
	ALStepC0S000();
	virtual void Start_Implementation() override;
	virtual void Stop_Implementation() override;
	
protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void TriggerFB(float Val);
	
	UPROPERTY(BlueprintReadOnly)
	UCRandomizer* RandFB = nullptr;
};
