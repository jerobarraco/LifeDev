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
	if (Item.Consumable && Current == 0) {
		int32 Index = -1;
		// this code sucks i don't like it. todo improve.
		if (Name == Selected) {
			TArray<FName> Keys;
			Items.GetKeys(Keys);
			const int32 NCount = Keys.Num() -1;
			// todo check this is correct.
			Index = Keys.Find(Name);
			Index = Index < NCount -1 ? Index : (Index >0 ? Index-1: -1);
			FName NewSel = NCount > 0 ? Keys[Index] : FName();
			
			SetSelected(NewSel);
		}

		Items.Remove(Name);
	}else{
		Items.Add(Name, Current);
		
		if (Selected.IsNone()) {
			SetSelected(Name);
		}
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

const FName& UInventory::GetSelected() {
	return Selected;
}

bool UInventory::SetSelected(const FName& Name) {
	if (!Items.Contains(Name)) return false;

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
