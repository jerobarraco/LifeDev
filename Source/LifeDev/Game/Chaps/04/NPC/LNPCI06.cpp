// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LNPCI06.h"

ALNPCI06::ALNPCI06():Super() {
	SetEnabled(true); // does not matter. get enabled by c4s2.
	TriggerDlg = "NPCI06_T";
	RewardStep = false; // just enable the pot
	UseRewardDestroy = false;
	Locked = false;
	IsOneShot = true;
	UseFade = true;
}

// on editor. rewardinterenabled. enables the pot.