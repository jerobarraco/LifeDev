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
	// call manually if this component is instantiated independently
	UFUNCTION(BlueprintCallable)
	void TickManual(float DT);

	// the target we want to get to
	UFUNCTION(BlueprintCallable)
	void SetTarget(float NewTgt);
	// the current value of the object. you need to set this every frame. alternative set OnGetVal.
	UFUNCTION(BlueprintCallable)
	void SetVal(float NewVal);

	virtual void Activate(bool bReset) override;
	virtual void Deactivate() override;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Kp = 1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Ki = 1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Kd = 1;
	// how many seconds to wait before auto-stopping when error is ==0. <=0 will disable it.
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float StopTime = 1;

	UPROPERTY(BlueprintAssignable, EditDefaultsOnly)
	FAPIDStart OnStart;
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly)
	FAPIDStop OnStop;
	// triggered every time it changes the value, and outputs the .. output of the pid
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly)
	FAPIDUpdate OnUpdate;
	/// This one gets called every time it needs to measure the value,
	/// this is recommended since it will only be called when about to do the process. with tick interval it might be different. 
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FAPIDGetVal OnGetVal;

protected:
	void DoTick(float DT);
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void Reset();

	// internal values
	float Target = 0.0;
	float Value = 0.0;
	float Output = 0.0;
	float ErrorPrev = 0.0;
	float Integral = 0.0;
	float Proportional = 0.0;
	float Derivative = 0.0;
	float CoolDown = 0.0;
	float DTAcum = 0.0;
};
