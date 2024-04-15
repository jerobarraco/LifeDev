// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#include "CRandomizer.h"

#include "CAnimator.h"

UCRandomizer::UCRandomizer(): Super() {
	PrimaryComponentTick.SetTickFunctionEnable(false);
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
	Super::SetAutoActivate(false);
	Super::SetActive(false); // not sure if this does much. but...
}

void UCRandomizer::Activate(bool bReset) {
	if (bReset) {
		Reset();
	} else if (IsActive()) { // on else because reset will override active
		return;
	}

	UWorld* const World = GetWorld();
	if (!World) return;

	Super::Activate(bReset);
	
	const float Delay = FMath::FRandRange(DelayMin, DelayMax);
	FTimerManager& Manager = World->GetTimerManager();
	Manager.SetTimer(Timer, this, &UCRandomizer::Trigger, Delay);
}

void UCRandomizer::Reset() {
	UWorld* const World = GetWorld();
	if (!World) return;
	
	FTimerManager& Manager = World->GetTimerManager();
	Manager.ClearTimer(Timer);
	Timer.Invalidate();
}

void UCRandomizer::Deactivate() {
	if (!IsActive()) return;
	Super::Deactivate();
	Reset();
}

void UCRandomizer::Trigger() {
	const float Val = FMath::FRandRange(ValueMin, ValueMax);

	if (IsValid(Anim)) {
		Anim->Duration = Val;
		if (UseRandReverse) {
			Anim->IsReversed = FMath::RandBool();
		}
		Anim->Activate(true);
	}

	OnTrigger.Broadcast();
	OnTriggerVal.Broadcast(Val);
	
	if (!IsLooping) return;
	Activate(true);
}
