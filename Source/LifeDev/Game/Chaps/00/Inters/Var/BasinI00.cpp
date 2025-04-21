// Copyright Jerónimo Barraco-Mármol

#include "BasinI00.h"

ABasinI00::ABasinI00():Super() {
	Texts = { NSLOCTEXT("BasinI00", "State0", "Close"), NSLOCTEXT("BasinI00", "State1", "Done") };
	UseAnim = false;
	IsLocked = false;
	RewardFlash = .1;
	DisableWhileAnim = false; // we will disable manually
	UseRewardDestroy = false; // one shot but won't disappear nor it will fade

	UseHint = true;
	// maybe move this to consts?
	HintCondition = "{Inter.TriggerL.DoorI05}"; // hint once he tried to open the door

	// Water->SetAutoActivate(true);
	// SND_Water->SetAutoActivate(true);
	IsOneShot = true;

	SetStateNow(0, true, true); // activate parts and audio
}

void ABasinI00::BeginPlay() {
	Super::BeginPlay();
	// SetState(0);
}

void ABasinI00::DoTrigger_Implementation() {
	// Water->Deactivate();
	// SND_Water->Fade(false);
	Super::DoTrigger_Implementation();
}
