// Copyright Jerónimo Barraco-Mármol

#include "LogicCard03.h"

#include "Inventory/Inventory.h"
#include "JUtils/Misc/JUtilsMisc.h"
#include "LifeDev/Core/Consts/ConstItems.h"
#include "LifeDev/Core/Settings/LSettings.h"

ULogicCard03::ULogicCard03() {
}

void ULogicCard03::Use_Implementation() {
	if (UNLIKELY(!ULSettings::GetFeatS(this, EFeat::G_CARD3))) return;
	
	Super::Use_Implementation();
	if (UNLIKELY(!Inv)) return;
	
	static TArray<FName> Cards = {
		// notice card03 (this) is not listed to avoid spamming this card
		LDConsts::Items::Card0, LDConsts::Items::Card1,
		LDConsts::Items::Card2, LDConsts::Items::Card4
	};
	UJUtilsMisc::ArrayShuffle(Cards);

	const int32 Num = Cards.Num();
	for (uint8 i =0; i<Num; ++i) {
		const FName& n = Cards[i];
		if (Inv->SetCold(n)) return; // notice this is a return
	}
}
