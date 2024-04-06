// Copyright Jerónimo Barraco-Mármol

#include "SpongeI00.h"

ASpongeI00::ASpongeI00():Super() {
	LockedDlg = "Sponge00_L";
	RewardItem = "Plates";
	Locked = false;
	UseRewardFade = false;
	IsOneShot = true;
	// TODO if i have time, on trigger, fade out, then fade in and do reward
	// might be easier to do with an extra step
}
