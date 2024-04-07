// Copyright Jerónimo Barraco-Mármol

#include "StoveI00.h"

AStoveI00::AStoveI00():Super() {
	Locked = false;
	TriggerDlg = "Stove00_T";
	RewardStep = true;
	SetEnabled(false);
}
