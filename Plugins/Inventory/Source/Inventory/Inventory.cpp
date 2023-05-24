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

	// clamp values
	// for non-consumables use always -1, for consumables clamp at 0
	if (Item.Consumable) {
		// calculate the difference. are clamped to the produce (0, MaxCount)
		const int32 Max = Item.MaxCount <= 0 ? Current + Diff: Item.MaxCount; 
		OutDiff = FMath::Clamp(Diff, -Current, Max - Current);
		Current = FMath::Max(0, Current+OutDiff);
	} else {
		// calculate the difference. non-consumable are always 0. the rest are clamped to the produce (0, MaxCount)
		OutDiff = Diff;
		Current = -1;
	}

	FName NewSelected;
	// remove empty consumables
	if (Item.Consumable && Current == 0) {
		int32 Index = -1;
		// this code sucks i don't like it. todo improve.
		if (Name == Selected) {
			TArray<FName> Keys;
			Items.GetKeys(Keys);
			Index = Keys.Find(Name);
			const int32 Count = Keys.Num();
			// todo check this is correct.
			Index = Index < Count -1 ? Index : (Index >0 ? Index-1: -1);
			NewSelected = Keys[Index];
		}

		Items.Remove(Name);
	}else{
		Items.Add(Name, Current);
		if (Selected.IsNone()) {
			NewSelected = Name;
		}
	}

	if (!NewSelected.IsNone()) {
		OnSelected.Broadcast(Selected);
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
