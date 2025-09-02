// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "CAnimatorSFX.h"

#include "AudioMixerBlueprintLibrary.h"


void UCAnimatorSFX::FXAdd() const {
	UAudioMixerBlueprintLibrary::AddSubmixEffect(
		this, Submix, FX);
}

void UCAnimatorSFX::FXRem() const {
	UAudioMixerBlueprintLibrary::RemoveSubmixEffect(
			this, Submix, FX);
}

void UCAnimatorSFX::Activate(const bool bReset) {
	if (UNLIKELY(bool(!Submix) | (!FX))) return;

	if (UseAutoManage & !IsReversed) FXAdd();

	Super::Activate(bReset);
}

void UCAnimatorSFX::Update_Implementation(const float Alpha) {
	Super::Update_Implementation(Alpha);
	if (UNLIKELY(!bool(Submix))) return;

	Submix->SetSubmixWetLevel(this, Alpha);
	Submix->SetSubmixDryLevel(this, 1.0-Alpha);
}

void UCAnimatorSFX::End_Implementation() {
	Super::End_Implementation();
	
	if (UNLIKELY(!bool(FX) | !bool(Submix))) return;

	const bool Remove = IsReversed;
	if (UseAutoManage & Remove) FXRem();
}

