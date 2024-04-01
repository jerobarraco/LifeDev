// Copyright Jerónimo Barraco-Mármol

#include "SpongeI00.h"

ASpongeI00::ASpongeI00():Super() {
	
	LockedDlg = "Sponge00_L";
	RewardItem = "Plates";
	Locked = true;
	UseRewardFade = false;
	IsOneShot = true;
	// TODO if i have time, on trigger, fade out, then fade in and do reward
}
