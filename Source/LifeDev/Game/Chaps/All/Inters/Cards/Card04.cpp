// Copyright Jerónimo Barraco-Mármol

#include "Card04.h"

#include "LifeDev/Core/Consts/ConstItems.h"

ACard04::ACard04():Super() {
	// overriden description for this chapter. will show on trigger.
	TriggerDlg = "C3_Look*";
	RewardItem = LDConsts::Items::Card4;
	UseRewardDestroy = true;
	// the step will finish on this, set on the step.

	Super::SetMobility(EComponentMobility::Static);
	SetAutoActivate(false);
	SetActive(false);
}
