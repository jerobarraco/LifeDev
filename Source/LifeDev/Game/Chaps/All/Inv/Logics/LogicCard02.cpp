// Copyright Jerónimo Barraco-Mármol

#include "LogicCard02.h"

#include "LifeDev/Core/Settings/LSettings.h"
#include "LifeDev/Game/Flashback/Flashback.h"

ULogicCard02::ULogicCard02() {
}

void ULogicCard02::Use_Implementation() {
	if (UNLIKELY(!ULSettings::GetFeatS(this, EFeat::G_CARD2))) return;

	Super::Use_Implementation();
	if (UNLIKELY(!FB)) return;

	// todo foxify
	FB->ModVal(FMath::FRandRange(ModMin, ModMax)); // lowers the fb by a random
}
