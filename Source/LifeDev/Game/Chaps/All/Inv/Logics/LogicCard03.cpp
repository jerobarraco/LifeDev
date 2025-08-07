// Copyright Jerónimo Barraco-Mármol

#include "LogicCard03.h"

#include "Inventory/Inventory.h"
#include "JUtils/Misc/JUtilsMisc.h"
#include "LifeDev/Core/Consts/ConstItems.h"

ULogicCard03::ULogicCard03() {
}

// namespace LD::Card03 {
	// static const char* Names[] = {
		// "C0", "C1", "C2", "C4" // not c3, it would spam 
	// };
// }

void ULogicCard03::Use_Implementation() {
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

	// constexpr size_t Size = UJUtilsMisc::ArraySize(LD::Card03::Names);
	// for (size_t i = 0; i<Size; i++) {
		// Inv->SetCold(LD::Card03::Names[i]);
	// }
}
