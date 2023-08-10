// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once

#include "CRandomizer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTrigger);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTriggerVal, float, Val);

class UCAnimator;

// Class that allows for timed randomization. OPTIONALLY works with an animator (or subclass)
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Interact), meta=(BlueprintSpawnableComponent))
class INTERACT_API UCRandomizer: public UActorComponent {
	GENERATED_BODY()

public:
	UCRandomizer();

	UFUNCTION(BlueprintCallable)
	void Start();
	
	UFUNCTION(BlueprintCallable)
	void Stop();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	float DelayMin = 0.5;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	float DelayMax = 10.0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	float ValueMin = 0.0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	float ValueMax = 1.0f;

	// whether the randomizer will loop 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	bool IsLooping = false;
	// whether to randomize the reverse for the anim
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	bool UseRandReverse = false;

	// optional animator, if set will be triggered, and have its duration randomizer via the "value" variables.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	UCAnimator* Anim = nullptr;

	// triggers on each trigger
	UPROPERTY(BlueprintAssignable, EditAnywhere, Category="SetUp")
	FOnTrigger OnTrigger;

	// trigger on each trigger, passes the random value as parameter
	UPROPERTY(BlueprintAssignable, EditAnywhere, Category="SetUp")
	FOnTriggerVal OnTriggerVal;
	
protected:
	// triggers the random, called by timer
	UFUNCTION(BlueprintCallable)
	void Trigger();

	// the current timer if any. 
	UPROPERTY(BlueprintReadOnly, Transient)
	FTimerHandle Timer;
};
