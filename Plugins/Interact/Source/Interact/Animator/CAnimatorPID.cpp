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

void UCAnimatorPID::Reset() { // TODO call when activate and was not active
	Integral = 0;
	ErrorPrev = 0;
	ValuePrev = 0;
	HasDerivative = false;
}

float UCAnimatorPID::AngleDiff(float A, float B) {
	// while this implementation has 2 branches. the FMod one has 1 and 2 function calls.
	// also this is simpler. and KISS.
	return FMath::FindDeltaAngleDegrees(B, A); // NOTE THE ORDER IS REVERSED!!!!
	// return FMath::Fmod( (A-B) + 540.0f, 360.0f) -180.f;
}

float UCAnimatorPID::GetVal_Implementation() {
	return OnGetVal.IsBound() ? OnGetVal.Execute() : Value;
}

void UCAnimatorPID::DoTick(float DT) {
	Value = GetVal(); // update

	/// do calculations
	float Error = 0;
	if (UseAngles) {
		Error = AngleDiff(Target, Value);
		Derivative = UseVelocityElseError && HasDerivative ?
			AngleDiff(Value, ValuePrev) / -DT: // note -Dt
			AngleDiff(Error, ErrorPrev) / DT;
	} else {
		Error = Target - Value;
		Derivative = UseVelocityElseError && HasDerivative ?
			(Value - ValuePrev) / -DT: // note -Dt
			(Error - ErrorPrev) / DT;
	}
	
	Proportional = Error;
	Integral = Integral + (Error * DT);
	if (IntegralMax>0) {
		Integral = FMath::Clamp(Integral, -IntegralMax, IntegralMax);
	}
	
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
	
	// notify
	OnUpdate.Broadcast(Output);

	// stop check. done after so that deactivate is triggered last
	if (StopTime > 0 && FMath::IsNearlyZero(Error, StopTolerance)) {
		CoolDown+=DT;
		if (CoolDown>=StopTime) {
			Deactivate();
		}
	} else {
		CoolDown = 0;
	}
}

void UCAnimatorPID::Activate(bool bReset) {
	Super::Activate(bReset);
	if (bReset) {
		Reset();
	}
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


// some of the improvements comes from here https://www.youtube.com/watch?v=y3K6FUgrgXw


// todo implement the more complicated version
/*
	A0 := Kp + Ki*dt + Kd/dt
	A1 := -Kp - 2*Kd/dt
	A2 := Kd/dt
	error[2] := 0 // e(t-2)
	error[1] := 0 // e(t-1)
	error[0] := 0 // e(t)
	output := u0  // Usually the current value of the actuator

	loop:
	error[2] := error[1]
	error[1] := error[0]
	error[0] := setpoint − measured_value
	output := output + A0 * error[0] + A1 * error[1] + A2 * error[2]
	wait(dt)
goto loop


maybe this one
A0 := Kp + Ki*dt
A1 := -Kp
error[2] := 0 // e(t-2)
error[1] := 0 // e(t-1)
error[0] := 0 // e(t)
output := u0  // Usually the current value of the actuator
A0d = Kd/dt
A1d = - 2.0*Kd/dt
A2d = Kd/dt
N := 5
tau := Kd / (Kp*N) // IIR filter time constant
alpha = dt / (2*tau)
d0 := 0
d1 := 0
fd0 := 0
fd1 := 0
loop:
	error[2] := error[1]
	error[1] := error[0]
	error[0] := setpoint − measured_value
	// PI
	output := output + A0 * error[0] + A1 * error[1]
	// Filtered D
	d1 = d0
	d0 = A0d * error[0] + A1d * error[1] + A2d * error[2]
	fd1 = fd0
	fd0 = ((alpha) / (alpha + 1)) * (d0 + d1) - ((alpha - 1) / (alpha + 1)) * fd1
	output := output + fd0      
	wait(dt)
	goto loop
*/

// some bits taken from https://vazgriz.com/621/pid-controllers/