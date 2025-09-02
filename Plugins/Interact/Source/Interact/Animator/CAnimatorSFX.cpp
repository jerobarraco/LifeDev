// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "CAnimatorSFX.h"

#include "AudioMixerBlueprintLibrary.h"

void UCAnimatorSFX::Activate(const bool bReset) {
	if (UNLIKELY(bool(!Submix) | (!FX))) return;

	if (!IsReversed)
		UAudioMixerBlueprintLibrary::AddSubmixEffect(
			this, Submix, FX);

	Super::Activate(bReset);
}

void UCAnimatorSFX::Update_Implementation(const float Alpha) {
	Super::Update_Implementation(Alpha);
	if (UNLIKELY(!bool(Submix))) return;

	Submix->SetSubmixWetLevel(this, Alpha);
	Submix->SetSubmixDryLevel(this, 1.0-Alpha);
}
