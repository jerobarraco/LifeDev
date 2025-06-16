// Copyright Jerónimo Barraco-Mármol

#include "LogicCard03.h"

#include "Inventory/Inventory.h"
#include "JUtils/Misc/JUtilsMisc.h"

ULogicCard03::ULogicCard03() {
}

namespace LD::Card03 {
	static const char* Names[] = {
		"C0", "C1", "C2", "C4" // not c3, it would spam 
	};
}

void ULogicCard03::Use_Implementation() {
	Super::Use_Implementation();
	
	UInventory* const Inventory = UInventory::Instance(this);
	if (UNLIKELY(!Inventory)) return;
	
	constexpr size_t Size = UJUtilsMisc::ArraySize(LD::Card03::Names);
	for (size_t i = 0; i<Size; i++) {
		Inventory->SetCool(LD::Card03::Names[i]);
	}
}
