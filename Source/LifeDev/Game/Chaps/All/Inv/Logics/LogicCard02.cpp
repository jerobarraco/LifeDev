// Copyright Jerónimo Barraco-Mármol

#include "LogicCard02.h"

#include "LifeDev/Game/Flashback/Flashback.h"

ULogicCard02::ULogicCard02() {
}

void ULogicCard02::Use_Implementation() {
	Super::Use_Implementation();

	// lowers the fb
	if (UNLIKELY(!FB)) return;
	FB->ModVal(FMath::FRandRange(-.05, -.1));
}
