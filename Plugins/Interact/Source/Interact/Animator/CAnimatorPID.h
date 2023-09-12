// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#pragma once
#include "CAnimatorPID.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAPIDUpdate, float, Output);
DECLARE_DYNAMIC_DELEGATE_RetVal(float, FAPIDGetVal);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAPIDStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAPIDStop);

// Animator for a pid controller. It will activate/deactivate automatically
UCLASS(Blueprintable, BlueprintType, Placeable, ClassGroup=(Interact), meta=(BlueprintSpawnableComponent))
class INTERACT_API UCAnimatorPID: public UActorComponent { // UCAnimator {
	GENERATED_BODY()

public:

	UCAnimatorPID();
	// call manually if this component is instantiated independently
	UFUNCTION(BlueprintCallable)
	void TickManual(float DT);

	// the target to aim for
	UFUNCTION(BlueprintCallable)
	void SetTarget(float NewTgt);
	// the current value of the object. you need to set this every frame. preferably set OnGetVal.
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

	// the proportional gain
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float Kp = 1;
	// the integral gain
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float Ki = 1;
	// the derivative gain
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float Kd = 1;
	// Saturation value for the integral. Happens on [-Max, Max]. no saturation is performed is set if ==0
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float IntegralMax = 0;
	// Clamps the output. no clamping happens if output min and max both are 0. or min>=max.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=SetUp)
	float OutputMin = 0;
	// Clamps the output. no clamping happens if output min and max both are 0. or min>=max.
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

	UPROPERTY(BlueprintAssignable, Category=SetUp)
	FAPIDStart OnStart;
	UPROPERTY(BlueprintAssignable, Category=SetUp)
	FAPIDStop OnStop;
	// triggered every time it changes the value, and outputs the .. output of the pid
	UPROPERTY(BlueprintAssignable, Category=SetUp)
	FAPIDUpdate OnUpdate;
	/// This one gets called every time it needs to measure the value,
	/// this is recommended since it will only be called when about to do the process. with tick interval it might be different. 
	UPROPERTY(BlueprintReadWrite, Category=SetUp)
	FAPIDGetVal OnGetVal;

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void DoTick(float DT);
	void Reset();

	static float AngleDiff(float A, float B);
	
	// must return the measured value of the system to control. override and get the appropriate value here.
	UFUNCTION(BlueprintNativeEvent)
	float GetVal();
	virtual float GetVal_Implementation();
	
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
