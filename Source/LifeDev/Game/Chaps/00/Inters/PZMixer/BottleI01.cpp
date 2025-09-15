// Copyright Jerónimo Barraco-Mármol

#include "BottleI01.h"

#include "Interact/Animator/CAnimatorFade.h"
#include "JUtils/Misc/JMiscConsts.h"

ABottleI01::ABottleI01():Super() {
	RewardItem = "Bottle01";
	UseRewardDestroy = true;
	UseFade = true;
	AnimFade->MatBase = nullptr;
	Texts = {
		NSLOCTEXT("Bottle00", "Text0", "Conditioner")
	};
	RewardIntersHint = {
		SoftOP(AInteract, "/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.SpotI04_UAID_D8BBC116E501525D02_1512082135"),
	};
}
