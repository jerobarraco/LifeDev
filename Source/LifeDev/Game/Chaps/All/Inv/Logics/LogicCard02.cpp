// Copyright Jerónimo Barraco-Mármol

#include "LogicCard02.h"

#include "LifeDev/Game/Flashback/Flashback.h"

ULogicCard02::ULogicCard02() {
}

void ULogicCard02::Use_Implementation() {
	Super::Use_Implementation();
	if (UNLIKELY(!FB)) return;

	// todo foxify
	FB->ModVal(FMath::FRandRange(ModMin, ModMax)); // lowers the fb by a random
}
