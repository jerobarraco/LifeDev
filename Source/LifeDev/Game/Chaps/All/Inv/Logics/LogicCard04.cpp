// Copyright Jerónimo Barraco-Mármol

#include "LogicCard04.h"

#include "LifeDev/Core/Settings/LSettings.h"

ULogicCard04::ULogicCard04() {}

void ULogicCard04::Use_Implementation() {
	if (UNLIKELY(!ULSettings::GetFeatS(this, EFeat::G_CARD4))) return;

	Super::Use_Implementation();
}
