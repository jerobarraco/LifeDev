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
	virtual void Update_Implementation(const float Alpha) override;
	virtual void End_Implementation() override;

	UFUNCTION(BlueprintCallable, CallInEditor, BlueprintPure=false)
	void FXAdd() const;
	UFUNCTION(BlueprintCallable, CallInEditor, BlueprintPure=false)
	void FXRem() const;
	
	// will add the effect on start, and remove on end.
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp")
	bool UseAutoManage = true;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp")
	TObjectPtr<USoundSubmix> Submix = nullptr;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="SetUp")
	TObjectPtr<USoundEffectSubmixPreset> FX = nullptr;
};
