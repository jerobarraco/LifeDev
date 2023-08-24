#include "CAnimatorSound.h"

#include "Components/AudioComponent.h"

void UCAnimatorSound::Update_Implementation(float Alpha) {
	Super::Update_Implementation(Alpha);

	if (!IsValid(Snd)) return;
	if (!Snd->IsPlaying()) return; // this could lead to a crash

	// TODO test all this stuff
	if (!IntName.IsNone()) {
		Snd->SetIntParameter(IntName, FMath::Lerp(IntMin, IntMax, Alpha));
	}
	if (!FloatName.IsNone()) {
		Snd->SetFloatParameter(FloatName, FMath::Lerp(FloatMin, FloatMax, Alpha));
	}
	if (!TriggerName.IsNone()) {
		Snd->SetTriggerParameter(TriggerName);
	}
	if (VolUse) {
		Snd->SetVolumeMultiplier(FMath::Lerp(VolMin, VolMax, Alpha));
	}
	if (PitchUse) {
		Snd->SetPitchMultiplier(FMath::Lerp(PitchMin, PitchMax, Alpha));
	}
}
