// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#include "CAnimatorPID.h"

UCAnimatorPID::UCAnimatorPID():Super() {
	// these are critical for the component to work correctly
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	// if this were true, it will start with tick enabled.
	PrimaryComponentTick.SetTickFunctionEnable(false);
	Super::SetAutoActivate(false);
	Super::Deactivate();
}

void UCAnimatorPID::TickManual(float DT) {
	if (!IsActive()) return;
	// basic tick interval for manual ticks
	DTAcum += DT;
	// UE_LOG(LogTemp, Log, TEXT("TickManual DTAcum=%3.3f DT=%3.3f"), DTAcum, DeltaSeconds);
	if (DTAcum < GetComponentTickInterval()) return;

	DoTick(DTAcum);
	DTAcum = 0.0;
}

void UCAnimatorPID::SetTarget(float NewTgt) {
	Target = NewTgt;
	Activate(false);
}

void UCAnimatorPID::SetVal(float NewVal) {
	Value = NewVal;
	Activate(false);
}

void UCAnimatorPID::Reset() {
	Integral = 0;
	ErrorPrev = 0;
	ValuePrev = 0;
	HasDerivative = false;
}

float UCAnimatorPID::GetTarget() const {
	return OnGetTarget.IsBound() ? OnGetTarget.Execute() : Target;
}

float UCAnimatorPID::GetVal() const {
	return OnGetVal.IsBound() ? OnGetVal.Execute() : Value;
}

void UCAnimatorPID::DoTick(float DT) {
	if (!AutoUpdateValue) Value = GetVal(); // update
	Target = GetTarget(); // update

	/// do calculations
	float Error = 0;
	if (UseAngles) {
		Error = FMath::FindDeltaAngleDegrees(Value, Target); // NOTE THE ORDER IS REVERSED
		Derivative = UseVelocityElseError && HasDerivative ?
			FMath::FindDeltaAngleDegrees(ValuePrev, Value) / -DT: // NOTE THE ORDER IS REVERSED!!!! // note -Dt
			FMath::FindDeltaAngleDegrees(ErrorPrev, Error) / DT; // NOTE REVERSED
	} else {
		Error = Target - Value;
		Derivative = UseVelocityElseError && HasDerivative ?
			(Value - ValuePrev) / -DT: // note -Dt
			(Error - ErrorPrev) / DT;
	}
	
	Proportional = Error;
	Integral = Integral + (Error * DT);
	if (IntegralMax>0)
		Integral = FMath::Clamp(Integral, -IntegralMax, IntegralMax);
	
	Output = (Kp * Proportional) + (Ki * Integral) + (Kd * Derivative);

	// clamp output
	// if (!FMath::IsNearlyZero(OutputMax) || !FMath::IsNearlyZero(OutputMin)) {
	if (!FMath::IsNearlyEqual(OutputMin, OutputMax) && OutputMin<OutputMax) {
		// we can't trust the < when they are equal
		Output = FMath::Clamp(Output, OutputMin, OutputMax);
	}

	// store prevs
	ValuePrev = Value;
	ErrorPrev = Error;
	HasDerivative = true;

	if (AutoUpdateValue)
		Value += Output;

	OnUpdate.Broadcast(Output, Value);// notify

	// stop check. done after so that deactivate is triggered last
	if (StopTime > 0 && FMath::IsNearlyZero(Error, StopTolerance)) {
		CoolDown+=DT;
		if (CoolDown>=StopTime)
			Deactivate();
	} else
		CoolDown = 0;
}

void UCAnimatorPID::Activate(bool bReset) {
	const bool WasActive = IsActive();
	bReset = bReset || !WasActive; // force reset if wasn't active

	Super::Activate(bReset);
	if (bReset) Reset();

	OnStart.Broadcast();
}

void UCAnimatorPID::Deactivate() {
	Super::Deactivate();
	Reset();
	OnStop.Broadcast();
}

void UCAnimatorPID::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	DoTick(DeltaTime);
}

// some bits taken from https://vazgriz.com/621/pid-controllers/


// float UCAnimatorPID::AngleDiff(float A, float B) {
// while this implementation has 2 branches. the FMod one has 1 and 2 function calls.
// also this is simpler. and KISS.
// return FMath::FindDeltaAngleDegrees(B, A); // NOTE THE ORDER IS REVERSED!!!!
// return FMath::Fmod( (A-B) + 540.0f, 360.0f) -180.f;
// }