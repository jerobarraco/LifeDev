// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#pragma once
#include "Components/ActorComponent.h"
#include "CAnimatorPID.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAPIDUpdate, const float, Output, const float, Value);
DECLARE_DYNAMIC_DELEGATE_RetVal(float, FAPIDGetVal);
DECLARE_DYNAMIC_DELEGATE_RetVal(float, FAPIDGetTarget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAPIDStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAPIDStop);

// Animator for a pid controller. It will activate/deactivate automatically.
// This is useful for matching two values (Target and Value)
// via a potentially undefined and indirect control variable/force (the output).
// Potentially when the target or the owner is affected by external forces or constraints.
// See note about Output on OnUpdate.
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Interact), meta=(BlueprintSpawnableComponent))
class INTERACT_API UCAnimatorPID: public UActorComponent {
	GENERATED_BODY()

public:
	UCAnimatorPID();
	// call manually if this component is instantiated independently
	UFUNCTION(BlueprintCallable)
	void TickManual(float DT);

	// The target value to aim for.
	// Set this when the target moves. (on tick or however often you need).
	// preferably using GetTarget.
	UFUNCTION(BlueprintCallable)
	void SetTarget(float NewTgt);
	// the current value of the object.
	// Set this when the current value moves from outside the PID (on tick or whenever you need).
	// preferably using GetVal.
	UFUNCTION(BlueprintCallable)
	void SetVal(float NewVal);
	// the current output of the system
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetOutput() const { return Output; };
	// the current output of the system
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetError() const { return ErrorPrev; };
	// start the controller
	virtual void Activate(bool bReset) override;
	// stop the controller
	virtual void Deactivate() override;

	// the proportional gain. it's a factor.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float Kp = .1f;
	// the integral gain. it's a factor. Bigger values could lead to error (NaN overflow).
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float Ki = .01f;
	// the derivative gain. it's a factor. Bigger values could lead to error (NaN overflow).
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float Kd = .01f;
	// Saturation value for the integral. Happens on [-Max, Max]. no saturation is performed is set if ==0
	// this value is absolute, not normalized.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float IntegralMax = 0.f;
	// Clamps the output. no clamping happens if OutputMin and OutputMax are equals. or min>=max.
	// Not normalized. See OnUpdate.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float OutputMin = 0.f;
	// Clamps the output. no clamping happens if OutputMin and OutputMax are equals. or min>=max.
	// Not normalized. See OnUpdate.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float OutputMax = 0.f;
	
	// how many seconds to wait before auto-stopping when error is ==0. <=0 will disable it.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float StopTime = 1.f;
	// The tolerance for the stop time
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float StopTolerance = UE_SMALL_NUMBER;
	// calculate derivative using velocity instead of error, to avoid a derivative kick.
	// The first frame it will use the error anyway.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	bool UseVelocityElseError = true;
	// when set the "value" and output is in degrees and does some angle wrapping (360==0) 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	bool UseAngles = false;
	// Will attempt to update the value automatically by integrating the output.
	// Will skip calling GetValue on each update.
	// This is only useful when the owner can't be affected by outside forces.
	// Otherwise, the PID's value and the owner's value will differ.
	// You can always call SetVal manually though.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	bool AutoUpdateValue = false;

	UPROPERTY(BlueprintAssignable, Category=SetUp)
	FAPIDStart OnStart;
	UPROPERTY(BlueprintAssignable, Category=SetUp)
	FAPIDStop OnStop;
	// triggered every time it processes. Outputs the output.
	// This is NOT the new value, but instead a different unit. a force, a delta.
	// You NEED to integrate it into your value, after which, you need to call SetVal.
	// otherwise you can set AutoUpdateValue.
	UPROPERTY(BlueprintAssignable, Category=SetUp)
	FAPIDUpdate OnUpdate;
	// This one gets called every time it needs to measure the value,
	// this is recommended since it will only be called when about to do the process.
	// with tick interval it might more efficient than uptading it every frame.
	// It's skipped when using AutoUpdateValue.
	UPROPERTY(BlueprintReadWrite, Category=SetUp)
	FAPIDGetVal OnGetVal;
	// This one gets called every time it needs to measure the target.
	// this is recommended since it will only be called when about to do the process.
	// with tick interval it might more efficient than uptading it every frame. 
	UPROPERTY(BlueprintReadWrite, Category=SetUp)
	FAPIDGetTarget OnGetTarget;

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void DoTick(float DT);
	void Reset();

	// must return the measured value of the system to control. override and get the appropriate value here.
	float GetVal() const;

	// must return the measured value of the system to control. override and get the appropriate value here.
	float GetTarget() const;
	
	/// internal values
	float Target = 0.0f;
	float Value = 0.0f;
	float Output = 0.0f;
	float ErrorPrev = 0.0f;
	float ValuePrev = 0.0f;
	float Integral = 0.0f;
	float Proportional = 0.0f;
	float Derivative = 0.0f;
	float CoolDown = 0.0f;
	float DTAcum = 0.0f;
	bool HasDerivative = false;
};
