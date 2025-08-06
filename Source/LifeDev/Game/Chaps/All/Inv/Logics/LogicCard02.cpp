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

	static TArray<FName> Cards = {
		// notice card02 (this) is not listed
		LDConsts::Items::Card0, LDConsts::Items::Card1,
		LDConsts::Items::Card3, LDConsts::Items::Card4
	};
	UJUtilsMisc::ArrayShuffle(Cards);

	const int32 Num = Cards.Num();
	for (uint8 i =0; i<Num; ++i) {
		const FName& n = Cards[i];
		if (Inv->SetCool(n)) return; // notice this is a return
	}

	if (UNLIKELY(!FB)) return;
	FB->ModVal(FMath::FRandRange(.05, 1));
}
