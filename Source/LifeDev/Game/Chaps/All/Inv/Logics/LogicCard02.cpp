// Copyright Jerónimo Barraco-Mármol

#include "LogicCard02.h"

#include "Inventory/Inventory.h"
#include "JUtils/Misc/JUtilsMisc.h"
#include "LifeDev/Core/Consts/ConstItems.h"
#include "LifeDev/Game/Flashback/Flashback.h"

ULogicCard02::ULogicCard02() {
}

void ULogicCard02::Use_Implementation() {
	Super::Use_Implementation();
	// reset the timer on one of the cards or lowers the fb

	if (UNLIKELY(!Inv)) return;

	if (UNLIKELY(!FB)) return;
	FB->ModVal(FMath::FRandRange(-.05, -.1));
}
