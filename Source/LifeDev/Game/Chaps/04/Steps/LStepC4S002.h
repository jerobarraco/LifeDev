// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LifeDev/Core/Story/LStep.h"

#include "LStepC4S002.generated.h"

class ALInteract;

UCLASS(Blueprintable, BlueprintType)
class ALStepC4S002 : public ALStep {
	GENERATED_BODY()

public:
	ALStepC4S002();

protected:
	virtual void BeginPlay() override;
	virtual void DoDebug_Implementation() override;
	virtual void DoStart_Implementation() override;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	TArray<ALInteract*> Chars;
};
