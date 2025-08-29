// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once

#include "CRandomizer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTrigger);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTriggerVal, float, Val);

class UCAnimator;

// Class that allows for timed randomization. OPTIONALLY works with an animator (or subclass)
// Doesn't use tick, uses timers.
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Interact), meta=(BlueprintSpawnableComponent))
class INTERACT_API UCRandomizer: public UActorComponent {
	GENERATED_BODY()

public:
	UCRandomizer();

	virtual void Activate(const bool bReset = false) override;
	virtual void Deactivate() override;
	
	// Test only. use Activate and deactivate. used for binding.
	UFUNCTION(BlueprintCallable, CallInEditor, meta=(AdvancedDisplay))
	FORCEINLINE void Start() {Activate (true);}

	// Test only. use Activate and deactivate.
	UFUNCTION(BlueprintCallable, CallInEditor, meta=(DeprecatedFunction))
	FORCEINLINE void Stop() {Deactivate();}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	float DelayMin = 0.5;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	float DelayMax = 10.0;

	// the minimum value to get as random. Will set that as duration to the Anim
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	float ValueMin = 0.0;
	// the max value to get as random. Will set that as duration to the Anim
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	float ValueMax = 1.0f;

	// whether the randomizer will loop 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp")
	bool IsLooping = false;
	// Whether to randomize the reverse for the anim.
	// This takes precedence over UseAnimMirror.
	// And will force reset the animation.
	// If the randomizer triggers mid-animation, this will jump.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Anim")
	bool UseAnimRandReverse = false;
	// whether to mirror the anim on trigger.
	// Won't reset the anim progress but will keep playing.
	// UseAnimRandReverse takes precedence, they are exclusive.
	// If the randomizer triggers mid-animation, this will make a smooth change of direction.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Anim")
	bool UseAnimMirror = false;
	// whether to set the anim duration to the random value on trigger. (ValueMin, ValueMax)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Anim")
	bool UseAnimValue = true;

	// optional animator, if set will be triggered. It will be reset if UseAnimreversed || !UseAnimMirror
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="SetUp|Anim")
	TObjectPtr<UCAnimator> Anim = nullptr;

	// triggers on each trigger
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Transient, Category="SetUp")
	FOnTrigger OnTrigger;

	// trigger on each trigger, passes the random value as parameter
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Transient, Category="SetUp")
	FOnTriggerVal OnTriggerVal;
	
protected:
	// triggers the random, called by timer
	UFUNCTION(BlueprintCallable)
	void Trigger();

	void Reset();

	// the current timer if any.
	FTimerHandle Timer;
};
