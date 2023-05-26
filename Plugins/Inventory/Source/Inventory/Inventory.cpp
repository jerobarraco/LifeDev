// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "Inventory.h"

#include "Engine/DataTable.h"

#pragma optimize("", off)

bool UInventory::Mod(const FName& Name, int32 Diff) {
	FName NewSel = FName();
	bool SetSelect = false;

	// get or create the item
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
		const int32 CurDiff = FMath::Clamp(Diff, -Current, Max - Current);
		// apply diff
		Current = FMath::Max(0, Current+CurDiff);
	} else {
		// calculate the difference. non-consumable are always 0. the rest are clamped to the produce (0, MaxCount)
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
	// reset transient variables to avoid issues with input.
	OutItem.Count = 0;
	OutItem.ActiveCoolDown = 0;
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

bool UInventory::Use(const FName& Name) {
	const bool Exists = Items.Contains(Name);
	if (!Exists) {
		UE_LOG(LogTemp, Error, TEXT("Tried to use an item that i don't have. '%s'"), *Name.ToString());
		return false;
	}
	FItem& Item = Items[Name];

	// intentionally not calling iscold for performance. if i end up uisng IsCold then call IsCold for simplicity here.
	if (Item.ActiveCoolDown>0) {
		UE_LOG(LogTemp, Error, TEXT("Tried to use an item that haven't cooled down. '%s': wait=%i"), *Name.ToString(), Item.ActiveCoolDown);
		return false;
	}

	// intentionally make a copy since when an object gets removed from the pool, the fname automagically transforms to the next name. W T F
	FName OldName = Name;
	Mod(Name, -1);
	// item was the last one in the inventory.
	if (!Items.Contains(OldName)) return true;

	// at this point the item reference is ok, se keep it.
	Item.ActiveCoolDown = Item.CoolDown;
	if (Item.ActiveCoolDown>0) {
		SetCoolTimerEnabled(true);
	}
	
	OnUsed.Broadcast(OldName);
	return true;
}

// do i need this?
// returns cold if it doesn't need to cool down, whether it uses or not cooldowns
bool UInventory::IsCold(const FName& Name) const {
	const bool Exists = Items.Contains(Name);
	if (!Exists) {
		UE_LOG(LogTemp, Error, TEXT("attempt to check for cold an item i don't have. '%s'"), *Name.ToString());
		return false;
	}

	return Items[Name].ActiveCoolDown <= 0;
}

void UInventory::SetCoolTimerEnabled(bool Enable) {
	UWorld* const World = GetWorld();
	if (!World) return;
	
	FTimerManager& Time = World->GetTimerManager();

	if (Enable) {
		// check if the timer is still valid
		if (CoolTimer.IsValid() && Time.TimerExists(CoolTimer)) return;
		Time.SetTimer(CoolTimer, this, &UInventory::CoolTimerTick, 1, true);
	} else {
		CoolTimer.Invalidate();
		Time.ClearAllTimersForObject(this);
	}
}

void UInventory::CoolTimerTick() {
	TArray<FName> Keys;
	Items.GetKeys(Keys);
	TArray<FName> ColdItems;

	bool AllCool = true;
	const int32 ItemsNum = Keys.Num();
	for(int32 i=0; i<ItemsNum; ++i){
		const FName& Name = Keys[i];
		FItem& Item = Items[Name];
		if (Item.ActiveCoolDown<=0) continue;
		
		Item.ActiveCoolDown = FMath::Max(0, Item.ActiveCoolDown-1);
		if (Item.ActiveCoolDown > 0) {
			AllCool = false;
			continue;
		}
		ColdItems.Add(Name);
	}

	if (AllCool) {
		SetCoolTimerEnabled(false);
	}

	const int32 ColdNum = ColdItems.Num();
	for (int32 i=0; i<ColdNum; ++i) {
		OnItemCold.Broadcast(ColdItems[i]);
	}
}
#pragma optimize("", on)
