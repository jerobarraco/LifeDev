// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#pragma once

#include "CoreMinimal.h"
#include "CAnimator.h"
#include "CAnimatorSFX.generated.h"

// Animator that handles submixes effects
UCLASS(ClassGroup=(Interact), meta=(BlueprintSpawnableComponent))
class INTERACT_API UCAnimatorSFX : public UCAnimator {
	GENERATED_BODY()

public:
	virtual void Activate(const bool bReset = false) override;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp")
	TObjectPtr<USoundSubmix> Submix = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp")
	TObjectPtr<USoundEffectSubmixPreset> FX = nullptr;
};
