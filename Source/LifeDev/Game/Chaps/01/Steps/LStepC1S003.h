// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LifeDev/Core/Story/LStep.h"

#include "LStepC1S003.generated.h"

class UNiagaraComponent;
class ALNPC01;

// blending to and spawning npc01
UCLASS(Blueprintable, BlueprintType)
class ALStepC1S003 : public ALStep {
	GENERATED_BODY()

public:
	ALStepC1S003();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	ALNPC01* NPC = nullptr;

protected:
	virtual void BeginPlay() override;
};
