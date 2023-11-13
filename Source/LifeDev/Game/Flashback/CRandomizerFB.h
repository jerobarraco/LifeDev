// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#pragma once
#include "Interact/Animator/CRandomizer.h"

#include "CRandomizerFB.generated.h"

// a class that will set the flashback system randomly
UCLASS(Blueprintable,BlueprintType)
class UCRandomizerFB : public UCRandomizer {
	GENERATED_BODY()

public:
	UCRandomizerFB();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	UFUNCTION()
	void TriggerFB(float Val);
};
