// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LNPCI06.h"

ALNPCI06::ALNPCI06():Super() {
	SetActive(false); // disabled by default. does not matter. get enabled by c4s2.

	Texts = {FText::FromString(FString("Talk"))};

	TriggerDlg = "NPCI06_T";
	RewardStep = false; // just enable the pot
	UseRewardDestroy = false;
	Locked = false;
	IsOneShot = true;
	UseFade = true;
	UseAnim = false;
}

// on editor. rewardinterenabled. enables the pot.