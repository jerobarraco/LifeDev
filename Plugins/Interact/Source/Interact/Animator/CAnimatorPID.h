// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once
#include "CAnimatorPID.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAPIDUpdate, const float, Output, const float, NewValue);
DECLARE_DYNAMIC_DELEGATE_RetVal(float, FAPIDGetVal);
DECLARE_DYNAMIC_DELEGATE_RetVal(float, FAPIDGetTarget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAPIDStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAPIDStop);

// Animator for a pid controller. It will activate/deactivate automatically
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Interact), meta=(BlueprintSpawnableComponent))
class INTERACT_API UCAnimatorPID: public UActorComponent { // UCAnimator {
	GENERATED_BODY()

public:

	UCAnimatorPID();
	// call manually if this component is instantiated independently
	UFUNCTION(BlueprintCallable)
	void TickManual(float DT);

	// the target to aim for. Set this when the target moves.
	// preferably using GetTarget
	UFUNCTION(BlueprintCallable)
	void SetTarget(float NewTgt);
	// the current value of the object. Set this when the current value moves from outside the PID.
	// preferrably using GetVal
	UFUNCTION(BlueprintCallable)
	void SetVal(float NewVal);
	// the current output of the system
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetOutput() const { return Output; };
	// the current output of the system
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetError() const { return ErrorPrev; };
	//	start the controller
	virtual void Activate(bool bReset) override;
	// stops the controller
	virtual void Deactivate() override;

	// the proportional gain. normalized.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float Kp = .1;
	// the integral gain. normalized.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float Ki = .1;
	// the derivative gain. normalized.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float Kd = .1;
	// Saturation value for the integral. Happens on [-Max, Max]. no saturation is performed is set if ==0
	// this value is absolute, not normalized.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float IntegralMax = 0;
	// Clamps the output. no clamping happens if output min and max are equals. or min>=max.
	// Not normalized. See OnUpdate.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float OutputMin = 0;
	// Clamps the output. no clamping happens if output min and max are equals. or min>=max.
	// Not normalized. See OnUpdate.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float OutputMax = 0;
	
	// how many seconds to wait before auto-stopping when error is ==0. <=0 will disable it.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float StopTime = 1;
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
	// will attempt to update the value automatically by integrating the output
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	bool AutoUpdateValue = true;

	UPROPERTY(BlueprintAssignable, Category=SetUp)
	FAPIDStart OnStart;
	UPROPERTY(BlueprintAssignable, Category=SetUp)
	FAPIDStop OnStop;
	// triggered every time it processes. Outputs the output.
	// This is NOT the new value, but instead a different unit. a force, a delta.
	// You NEED to integrate it into your value, after which, you need to call SetVal.
	UPROPERTY(BlueprintAssignable, Category=SetUp)
	FAPIDUpdate OnUpdate;
	/// This one gets called every time it needs to measure the value,
	/// this is recommended since it will only be called when about to do the process. with tick interval it might be different. 
	UPROPERTY(BlueprintReadWrite, Category=SetUp)
	FAPIDGetVal OnGetVal;
	/// This one gets called every time it needs to measure the target.
	/// this is recommended since it will only be called when about to do the process. with tick interval it might be different. 
	UPROPERTY(BlueprintReadWrite, Category=SetUp)
	FAPIDGetTarget OnGetTarget;

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void DoTick(float DT);
	void Reset();

	// must return the measured value of the system to control. override and get the appropriate value here.
	UFUNCTION(BlueprintNativeEvent)
	float GetVal();

	// must return the measured value of the system to control. override and get the appropriate value here.
	UFUNCTION(BlueprintNativeEvent)
	float GetTarget();
	
	/// internal values
	float Target = 0.0;
	float Value = 0.0;
	float Output = 0.0;
	float ErrorPrev = 0.0;
	float ValuePrev = 0.0;
	float Integral = 0.0;
	float Proportional = 0.0;
	float Derivative = 0.0;
	float CoolDown = 0.0;
	float DTAcum = 0.0;
	bool HasDerivative = false;
};
