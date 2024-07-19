// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#include "CAnimatorSound.h"

#include "Components/AudioComponent.h"

void UCAnimatorSound::Update_Implementation(float Alpha) {
	Super::Update_Implementation(Alpha);

	if (!IsValid(Snd)) return;
	if (!Snd->IsPlaying()) return; // this could lead to a crash

	// TODO test all this stuff
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
