// Copyright Jerónimo Barraco-Mármol

#include "ScrewerI00.h"

AScrewerI00::AScrewerI00():Super() {
	RewardItem = "Screwer00";
	UseFade = true;
	UseAutoActivate = true;
	AScrewerI00::SetMobility(EComponentMobility::Type::Static);
}
