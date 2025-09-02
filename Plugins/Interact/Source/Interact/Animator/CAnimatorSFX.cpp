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
