// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LNPCI06.h"

ALNPCI06::ALNPCI06():Super() {
	SetEnabled(true);
	TriggerDlg = "NPCI06_T";
	RewardStep = true;
	UseRewardFade = false;
	Locked = false;
}
