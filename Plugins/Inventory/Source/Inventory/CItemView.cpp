// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only
#include "CItemView.h"

#include "ItemViewUI.h"

// TODO do i even need this? maybe i can just... not use it. and use the InventoryManager.
// maybe i will keep it just to be able to separate the code
UCItemView::UCItemView():Super() {
	static ConstructorHelpers::FClassFinder<UUserWidget> DefaultUI(TEXT("/Game/LifeDev/Game/Inventory/UI/W_ItemViewUI"));
	UIClass = DefaultUI.Succeeded() ? DefaultUI.Class.Get() : UItemViewUI::StaticClass();
}
