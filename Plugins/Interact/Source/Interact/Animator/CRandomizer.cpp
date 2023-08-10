// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#include "CRandomizer.h"

#include "CAnimator.h"

UCRandomizer::UCRandomizer(): Super() {
	PrimaryComponentTick.SetTickFunctionEnable(false);
	PrimaryComponentTick.bStartWithTickEnabled = false;
	Super::SetComponentTickEnabled(false);
	Super::SetActive(false); // not sure this does much. but...
}

void UCRandomizer::Start() {
	Stop(); // clear timer and such

	UWorld* const World = GetWorld();
	if (!World) {
		return;
	}
	
	Super::SetActive(true);
	const float Delay = FMath::FRandRange(DelayMin, DelayMax);
	FTimerManager& Manager = World->GetTimerManager();
	Manager.SetTimer(Timer, this,&UCRandomizer::Trigger, Delay);
}

void UCRandomizer::Stop() {
	Super::SetActive(false);
	UWorld* const World = GetWorld();
	if (!World) {
		return;
	}
	
	FTimerManager& Manager = World->GetTimerManager();
	Manager.ClearTimer(Timer);
	Timer.Invalidate();
}

void UCRandomizer::Trigger() {
	const float Val = FMath::FRandRange(ValueMin, ValueMax);

	if (IsValid(Anim)) {
		Anim->Duration = Val;
		if (UseRandReverse) {
			Anim->IsReversed = FMath::RandBool();
		}
		Anim->Play();
	}

	OnTrigger.Broadcast();
	OnTriggerVal.Broadcast(Val);
	
	if (!IsLooping) return;
	Start();
}
