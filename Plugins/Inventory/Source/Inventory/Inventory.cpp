// Copyright (C) 2023 - Jeronimo Barraco-Marmol

#include "Inventory.h"

#include "Engine/DataTable.h"

#pragma optimize("", off)

void UInventory::Init(UDataTable* DataTable) {
	if (IsValid(DataTable)) {
		DT = DataTable;
	}
}

void UInventory::DeInit() {
	DT = nullptr;
	Items.Empty();
	Slots.Empty();
}

TMap<FName, int32> UInventory::GetItems() {
	return Items; // purposely creating a copy
}

void UInventory::GetItems(const TMap<FName, int32>& NewItems) {
	Items = NewItems;
}

#pragma optimize("", on)
