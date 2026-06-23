// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "CRandomizer.h"

#include "Engine/World.h"
#include "TimerManager.h"

#include "CAnimator.h"

UCRandomizer::UCRandomizer(): Super() {
	PrimaryComponentTick.SetTickFunctionEnable(false);
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
	UCRandomizer::SetAutoActivate(false);
}

void UCRandomizer::Activate(const bool bReset) {
	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;
	
	if (bReset)
		Reset();
	else if (IsActive()) // on else because reset will override active
		// before super::activate since it will make it active.
		return;

	Super::Activate(bReset);

	const float Delay = FMath::FRandRange(DelayMin, DelayMax);
	FTimerManager& Manager = World->GetTimerManager();
	Manager.SetTimer(Timer, this, &UCRandomizer::Trigger, Delay);
}

void UCRandomizer::Reset() {
	const UWorld* const World = GetWorld();
	if (UNLIKELY(!World)) return;
	
	FTimerManager& Manager = World->GetTimerManager();
	Manager.ClearTimer(Timer);
	Timer.Invalidate();
}

void UCRandomizer::Deactivate() {
	// if (!IsActive()) return;
	Super::Deactivate();
	Reset();
}

void UCRandomizer::Trigger() {
	const float Val = FMath::FRandRange(ValueMin, ValueMax);

	if (IsValid(Anim)) {
		if (UseAnimValue) Anim->Duration = Val;
		if (UseAnimRandReverse)
			Anim->IsReversed = FMath::RandBool();
		else if (UseAnimMirror)
			Anim->Mirror();
		const bool Reset = UseAnimRandReverse || !UseAnimMirror;
		Anim->Activate(Reset);
	}

	OnTrigger.Broadcast();
	OnTriggerVal.Broadcast(Val);
	
	if (!IsLooping) return;

	Activate(true);
}
