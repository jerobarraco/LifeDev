// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LifeDev/Core/Story/LStep.h"

#include "LStepC1S002.generated.h"

class UNiagaraComponent;
// when the chap 1 boss enters
UCLASS(Blueprintable, BlueprintType)
class ALStepC1S002 : public ALStep {
	GENERATED_BODY()

public:
	ALStepC1S002();

	virtual void Start_Implementation() override;

protected:
	void SpawnGhosts();
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	UNiagaraComponent* Ghosts = nullptr;
};
