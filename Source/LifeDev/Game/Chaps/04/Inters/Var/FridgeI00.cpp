// Copyright Jerónimo Barraco-Mármol

#include "FridgeI00.h"

AFridgeI00::AFridgeI00():Super() {
	Locked=true;
	IsOneShot=true;
	LockedDlg="Fridge_L";
	SetEnabled(true);
	UseRewardFade=false;
}

