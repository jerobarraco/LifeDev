#include "CAnimatorSound.h"

#include "Components/AudioComponent.h"

void UCAnimatorSound::Update_Implementation(float Alpha) {
	Super::Update_Implementation(Alpha);

	if (!IsValid(Snd)) return;
	if (!IntName.IsNone()) {
		Snd->SetIntParameter(IntName, FMath::Lerp(IntMin, IntMax, Alpha));
	}
	if (!FloatName.IsNone()) {
		Snd->SetFloatParameter(FloatName, FMath::Lerp(FloatMin, FloatMax, Alpha));
	}
	if (!TriggerName.IsNone()) {
		Snd->SetTriggerParameter(TriggerName);
	}
}
