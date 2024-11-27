// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LInventoryMan.h"

#include "LInventoryUI.h"

ALInventoryMan::ALInventoryMan():Super() {
	static ConstructorHelpers::FClassFinder<ULInventoryUI>
		CUI(TEXT("/Game/LifeDev/Game/Inventory/UI/W_InventoryUI"));
	UIClass = CUI.Succeeded() ? CUI.Class.Get() : UInventoryUI::StaticClass();
}
