// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "CAnimatorSound.h"

#include "Components/AudioComponent.h"

UCAnimatorSound::UCAnimatorSound() {
	SetComponentTickInterval(1/30); // opt. doesn't need 60fps.
}

void UCAnimatorSound::Update_Implementation(const float Alpha) {
	Super::Update_Implementation(Alpha);

	if (UNLIKELY(!IsValid(Snd))) return;
	if (UNLIKELY(!Snd->IsPlaying())) return; // this could lead to a crash

	if (!IntName.IsNone())
		Snd->SetIntParameter(IntName, FMath::LerpStable(IntMin, IntMax, Alpha));

	if (!FloatName.IsNone())
		Snd->SetFloatParameter(FloatName, FMath::LerpStable(FloatMin, FloatMax, Alpha));

	if (!TriggerName.IsNone())
		Snd->SetTriggerParameter(TriggerName);
	
	if (VolUse)
		Snd->SetVolumeMultiplier(FMath::LerpStable(VolMin, VolMax, Alpha));
	
	if (PitchUse)
		Snd->SetPitchMultiplier(FMath::LerpStable(PitchMin, PitchMax, Alpha));
}
