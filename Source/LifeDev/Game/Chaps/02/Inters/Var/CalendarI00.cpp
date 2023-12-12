// Copyright Jerónimo Barraco-Mármol

#include "CalendarI00.h"

ACalendarI00::ACalendarI00():Super() {
	UseAnim = false;
	UseRewardFade = false;
	RewardFlash = .1;
	TriggerDlg = "Cal00_T";
	SetEnabled(true);
}
