// Copyright Jerónimo Barraco-Mármol

#include "CalendarI00.h"

ACalendarI00::ACalendarI00():Super() {
	UseAnim = false;
	UseRewardDestroy = false;
	RewardFlash = .1;
	TriggerDlg = "Cal00_T";
	Super::SetAutoActivate(true);
}

void ACalendarI00::Rewarded_Implementation() {
	Super::Rewarded_Implementation();
	RewardActor = nullptr;
	SetActive(false);
}
