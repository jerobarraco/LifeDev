// Copyright Jerónimo Barraco-Mármol

#include "Card04.h"

#include "LifeDev/Core/Consts/ConstItems.h"

ACard04::ACard04():Super() {
	// overriden description for this chapter. will show on trigger.
	TriggerDlg = "C4_Look*"; // TODO
	RewardItem = LDConsts::Items::Card4;
	// the step will finish on this, set on the step.

	ACard04::SetMobility(EComponentMobility::Static);
	ACard04::SetAutoActivate(false);
}
