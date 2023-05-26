// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LInventoryManager.h"

#include "LInventoryUI.h"

ALInventoryManager::ALInventoryManager():Super() {
	static ConstructorHelpers::FClassFinder<ULInventoryUI> CUI(TEXT("/Game/LifeDev/Game/Inventory/UI/W_InventoryUI"));
	UIClass = CUI.Class;
}
