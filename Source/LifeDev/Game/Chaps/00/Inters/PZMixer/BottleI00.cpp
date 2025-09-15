// Copyright Jerónimo Barraco-Mármol

#include "BottleI00.h"

#include "Interact/Animator/CAnimatorFade.h"
#include "JUtils/Misc/JMiscConsts.h"

ABottleI00::ABottleI00():Super() {
	RewardItem = "Bottle00";
	UseRewardDestroy = true;
	UseFade = true;
	AnimFade->MatBase = nullptr;
	RewardIntersHint = {
		SoftOP(AInteract, "/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.SpotI04_UAID_D8BBC116E501525D02_1512082135"),
	};
}
