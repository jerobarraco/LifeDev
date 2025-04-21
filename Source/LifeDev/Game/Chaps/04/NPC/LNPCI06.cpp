// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LNPCI06.h"

#include "LifeDev/Game/Chaps/04/Inters/Var/MatchBoxI00.h"
#include "LifeDev/Game/Chaps/04/Inters/Var/StoveI00.h"

ALNPCI06::ALNPCI06():Super() {
	Texts = { NSLOCTEXT("LNPCI06", "State0", "Talk")};

	TriggerDlg = "NPCI06_T";
	// RewardStep = false; // just enable the stove
	UseRewardDestroy = false;
	IsLocked = false;
	IsOneShot = true;
	UseFade = true;
	UseAnim = false;
	RewardIntersActiveClass = { AStoveI00::StaticClass(), AMatchBoxI00::StaticClass() };
	// UseAutoActivate = false; // disabled by default. does not matter. get enabled by c4s2. // redundant
}
