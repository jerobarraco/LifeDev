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
}

void APotI00::BeginPlay() {
	Super::BeginPlay();
	SetState(1); // start open
}

void APotI00::DoTrigger_Implementation() {
	Super::DoTrigger_Implementation();
	if (Stove) Stove->SetEnabled(true);
	Locked = true;
}
