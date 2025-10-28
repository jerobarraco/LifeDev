// Copyright Jerónimo Barraco-Mármol

#include "ScrewerI00.h"

#include "Interact/Animator/CAnimatorFade.h"

AScrewerI00::AScrewerI00():Super() {
	RewardItem = "Screwer00";
	UseAutoActivate = true;
	UseHint = true;
	HintCondition = "{Inter.Locked.DoorI10}"; // hint after triggered
	UseFade = true;
	AnimFade->MatBase = nullptr; // the mesh uses a palette. force using the mesh's material.

	AScrewerI00::SetMobility(EComponentMobility::Type::Static);
}
