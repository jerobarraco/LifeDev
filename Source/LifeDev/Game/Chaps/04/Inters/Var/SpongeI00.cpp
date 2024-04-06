// Copyright Jerónimo Barraco-Mármol

#include "SpongeI00.h"

ASpongeI00::ASpongeI00():Super() {
	TriggerDlg = "Sponge00_T";
	RewardItem = "Plates";
	// RewardStep = true;
	Locked = false;
	UseRewardFade = false;
	IsOneShot = true;
	// TODO if i have time, on trigger, fade out, then fade in and do reward
	// might be easier to do with an extra step
}

void ASpongeI00::DoTrigger_Implementation() {
	Super::DoTrigger_Implementation();
	for (ALInteract* const I: Plates) {
		if (!IsValid(I)) continue;
		I->Fade(false);
		I->SetEnabled(false);
	}
}
