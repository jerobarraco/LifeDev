// Copyright (C) 2023 - Jeronimo Barraco-Marmol

#include "Inventory.h"

#include "Engine/DataTable.h"

#pragma optimize("", off)

bool UInventory::Mod(const FName& Name, int32 Diff, int32& OutDiff) {

	FName NewSel = FName();
	bool SetSelect = false;
	
	FItem* Item = Items.Find(Name);
	if (!Item) {
		if (Diff<=0) {
			UE_LOG(LogTemp, Warning, TEXT("Attempt to substract from an item i dont have. Name=%s"), *Name.ToString());
			return false;
		} else {
			FItem NewItem;
			const bool FoundRaw = GetRaw(Name, NewItem);
			if (!FoundRaw) {
				UE_LOG(LogTemp, Warning, TEXT("Attempted to add an item that doesnt exists. Name=%s"), *Name.ToString());
				return false;
			}

			Item = &Items.Add(Name, NewItem);
			if (Selected.IsNone()) {
				NewSel = Name;
				SetSelect = true;
			}
		}
	}

	int32 Current = Item->Count;

	// clamp values
	// for non-consumables use always -1, for consumables clamp at 0
	if (Item->Consumable) {
		// calculate the difference. are clamped to the produce (0, MaxCount)
		// get the max we can go. Current+diff to allow to grow.
		const int32 Max = Item->MaxCount <= 0 ? Current + Diff: Item->MaxCount;
		// clamp the diff to the max. and min.
		OutDiff = FMath::Clamp(Diff, -Current, Max - Current);
		// apply diff
		Current = FMath::Max(0, Current+OutDiff);
	} else {
		// calculate the difference. non-consumable are always 0. the rest are clamped to the produce (0, MaxCount)
		OutDiff = Diff;
		Current = -1;
	}

	Item->Count = Current;
	// remove empty consumables
	if (Item->Consumable && Item->Count <= 0) {
		// this code sucks i don't like it. todo improve.
		if (Name == Selected) {
			NewSel = GetNextKey();
			SetSelect = true;
		}
		Items.Remove(Name);
	}

	OnMod.Broadcast(Name, *Item, Current);

	// Set selected only after removing.
	// there's something fishy going on. otherwise it will remove the wrong object!
	// also triggering it at the end works better with the ui
	if (SetSelect) {
		SetSelected(NewSel);
	}

	return true;
}

bool UInventory::GetRaw(const FName& Name, FItem& OutItem) const {
	if (!IsValid(DT)) return false;

	const FItem* const Item = DT->FindRow<FItem>(Name, TEXT(""));
	if (!Item) return false;

	// set the item anyway even if not found
	OutItem = *Item;
	return true;
}

bool UInventory::Get(const FName& Name, FItem& OutItem) const {
	const FItem* pItem = Items.Find(Name);
	if (!pItem) {
		return false;
	}

	OutItem = *pItem; // purposely return a copy
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

const TMap<FName, FItem>& UInventory::GetItems() const {
	return Items;
}

void UInventory::SetItems(const TMap<FName, FItem>& NewItems) {
	Items = NewItems;
}

const FName& UInventory::GetSelected() {
	return Selected;
}

FName UInventory::GetNextKey(bool Forward, FName From) const {
	static const FName Empty;
	if (From.IsNone()) {
		if (Selected.IsNone()) return Empty;
		From = Selected;
	}
	
	TArray<FName> Keys;
	Items.GetKeys(Keys);
	// <2 because one will get removed. and we need to tell this situation apart to clear the selected
	if (Keys.Num()<2) return Empty;

	const int32 Num = Keys.Num();
	int32 Index = Keys.Find(From);
	Index += Forward ? 1 : -1; // shift
	Index += Num; // pad in case it goes into negatives
	Index = Index % Num; // wrap
	return Keys[Index];
}

bool UInventory::SetSelected(const FName& Name) {
	if (!Name.IsNone() && !Items.Contains(Name)) return false;
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
