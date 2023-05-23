// Copyright (C) 2023 - Jeronimo Barraco-Marmol

#include "Inventory.h"

#include "Engine/DataTable.h"

#pragma optimize("", off)

bool UInventory::Mod(const FName& Name, int32 Diff, int32& OutDiff) {
	FItem Item;
	int32 Current;
	const bool Found = Get(Name, Item, Current);
	// the item does not exists, can't progress.
	if (!Found) return false;

	// calculate the difference. non-consumable are always 0. the rest are clamped to the produce (0, MaxCount)
	OutDiff = Item.Consumable ? FMath::Clamp(Diff, -Current, Item.MaxCount - Current) : 0;
	// for non-consumables use always -1, for consumables clamp at 0
	Current = Item.Consumable ? FMath::Max(0, Current+OutDiff) : -1;

	if (Item.Consumable && (Current <0 || Current > Item.MaxCount)) {
		UE_LOG(LogTemp, Error, TEXT(" DONT LET THE RACOON DO MATH!! "));
		return false;
	}

	// remove empty consumables
	if (Item.Consumable && Current == 0) {
		Items.Remove(Name);
	}else{
		Items.Add(Name, Current);
	}

	OnMod.Broadcast(Name, Item, Current);
	return true;
}

bool UInventory::Get(const FName& Name, FItem& OutItem, int32& OutCount) {
	OutCount = 0;
	if (!IsValid(DT)) return false;

	const FItem* const Item = DT->FindRow<FItem>(Name, TEXT(""));
	if (!Item) return false;

	// set the item anyway even if not found
	OutItem = *Item;

	const int32* pCount = Items.Find(Name);
	if (pCount) {
		OutCount = *pCount;
	}

	return true;
}

void UInventory::Init(UDataTable* DataTable) {
	if (IsValid(DataTable)) {
		DT = DataTable;
	}
}

void UInventory::DeInit() {
	DT = nullptr;
	Items.Empty();
}

TMap<FName, int32> UInventory::GetItems() {
	return Items; // purposely creating a copy
}

void UInventory::SetItems(const TMap<FName, int32>& NewItems) {
	Items = NewItems;
}

#pragma optimize("", on)
