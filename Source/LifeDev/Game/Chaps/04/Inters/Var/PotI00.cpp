// Copyright Jerónimo Barraco-Mármol

#include "PotI00.h"

APotI00::APotI00():Super() {
	// RewardFlash = 0.1;
	Texts = {FText::FromString(TEXT("Hot pot")), FText::FromString(TEXT("Empty pot")) };
	UseRewardFade = false;
	Locked = false;
	LockedDlg = "Pot00_L";
	TriggerDlg = "Pot00_T";
	SetEnabled(false);
	IsOneShot = true;
	// I'm using SetEnabled instead of Lock because these things will be changing during the chapter
	// and i think that the player will find easier to tell when something became enabled. 
	// than realizing something became unlocked.
	// (without any extra cue/feedback which i'm not going to add now)
}

void APotI00::BeginPlay() {
	Super::BeginPlay();
	UseAnim = false; // sucks but avoids disable while anim breaking the set enabled
	SetState(1); // start open
	UseAnim = true;
}

void APotI00::DoTrigger_Implementation() {
	Super::DoTrigger_Implementation();
	if (Stove) Stove->SetEnabled(true);
}

// on editor. enables the stove.
