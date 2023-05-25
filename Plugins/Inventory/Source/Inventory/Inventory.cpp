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

	// remove empty consumables
	FName NewSel = FName(); 
	if (Item.Consumable && Current == 0) {
		int32 Index = -1;
		// this code sucks i don't like it. todo improve.
		if (Name == Selected) {
			TArray<FName> Keys;
			Items.GetKeys(Keys);
			Index = Keys.Find(Name);
			const int32 NCount = Keys.Num() -1;
			// todo check this is correct.
			Index = Index < NCount ? Index+1 : (Index >0 ? Index-1: -1);
			NewSel = Index >= 0 ? Keys[Index] : FName();
		}
		Items.Remove(Name);
	}else{
		Items.Add(Name, Current);
		if (Selected.IsNone()) {
			NewSel = Name;
		}
	}

	// Set selected only after removing.
	// there's something fishy going on. otherwise it will remove the wrong object!
	// also triggering it at the end works better with the ui
	if (!NewSel.IsNone()) {
		SetSelected(Name);
	}

	OnMod.Broadcast(Name, Item, Current);
	return true;
}

bool UInventory::Get(const FName& Name, FItem& OutItem, int32& OutCount) const {
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

const TMap<FName, int32>& UInventory::GetItems() const {
	return Items;
}

void UInventory::SetItems(const TMap<FName, int32>& NewItems) {
	Items = NewItems;
}

const FName& UInventory::GetSelected() {
	return Selected;
}

bool UInventory::SetSelected(const FName& Name) {
	if (!Items.Contains(Name)) return false;
	if (Name == Selected) return false;
	
	Selected = Name;
	OnSelected.Broadcast(Selected);
	return true;
}

void UInventory::Use(const FName& Name) {
	const bool Exists = Items.Contains(Name);
	if (!Exists) {
		UE_LOG(LogTemp, Error, TEXT("Tried to use an item that i don't have!"));
		return;
	}
	int32 OutDiff;
	Mod(Name, -1, OutDiff);
	OnUsed.Broadcast(Name);
}

#pragma optimize("", on)
