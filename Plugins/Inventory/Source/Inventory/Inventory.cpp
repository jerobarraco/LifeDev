// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "Inventory.h"

#include "Engine/DataTable.h"

#include "ItemLogic.h" //needed for ManType.

bool UInventory::Mod(const FName& Name, int32 Diff) {
	if (Name.IsNone()) {
		UE_LOG(LogInventory, Warning, TEXT("Attempt to mod item None"));
		return false;
	}

	FName NewSel = NAME_None;
	bool SetSelect = false; // need another flag because we might wanna set the selected to none

	// get or create the item
	FItem* Item = Items.Find(Name);
	if (!Item) {
		if (Diff<=0) {
			UE_LOG(LogInventory, Warning, TEXT("Attempt to substract from an item i don't have. Name=%s"), *Name.ToString());
			return false;
		}

		Item = AddNew(Name);
		if (!Item) return false;
		
		// select the new one if nothing was selected
		if (Selected.IsNone()) {
			NewSel = Name;
			SetSelect = true;
		}
	}

	/// update the item count
	int32 Current = Item->Count;
	// used to broadcast even on non-consumable
	int32 CurDiff = Diff;
	// clamp values
	// for non-consumables use always -1, for consumables clamp at 0
	if (Item->Consumable) {
		// calculate the difference. are clamped to the produce (0, MaxCount)
		// get the max we can go. Current+diff to allow to grow.
		const int32 Max = Item->MaxCount <= 0 ? Current + Diff: Item->MaxCount;
		// clamp the diff to the max. and min.
		CurDiff = FMath::Clamp(Diff, -Current, Max - Current);
		// apply diff
		Current = FMath::Max(0, Current+CurDiff);
	} else {
		// calculate the difference. non-consumable are always 0. the rest are clamped to the produce (0, MaxCount)
		Current = -1;
	}

	Item->Count = Current;
	
	// intentionally copying the item, to avoid issues. the item might have been removed, or might 
	FItem ItemCopy = *Item;
	// remove empty consumables
	if (Item->Consumable && Item->Count <= 0) {
		// this code sucks i don't like it. todo improve.
		if (Name == Selected) {
			NewSel = GetNextKey();
			SetSelect = true;
		}
		Items.Remove(Name);
	}

	OnMod.Broadcast(Name, CurDiff, MoveTemp(ItemCopy));

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
	OutItem = *Item; // note this is a copy
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

bool UInventory::GetSelectedItem(FItem& Item) const {
	if (Selected.IsNone()) {
		UE_LOG(LogInventory, Warning, TEXT("No item is selected."));
		return false;
	}
	
	if (!Get(Selected, Item)) {
		UE_LOG(LogInventory, Warning, TEXT("Item does not exists? but here? this should NOT happen!!!!"));
		return false;
	}

	return true;
}

FName UInventory::GetNextKey(bool Forward, FName From) const {
	if (From.IsNone()) {
		if (Selected.IsNone()) return NAME_None;
		From = Selected;
	}
	
	TArray<FName> Keys;
	Items.GetKeys(Keys);
	// <2 because one will get removed. and we need to tell this situation apart to clear the selected
	if (Keys.Num()<2) return NAME_None;

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

bool UInventory::Has(const FName& Name) {
	if (Name.IsNone()) return false;
	return Items.Contains(Name);
}

bool UInventory::Use(const FName& Name) {
	bool Found = false;
	FItem& Item = GetRef(Name, Found);
	if (!Found)	return false; 

	if (!IsUsable(Item)) {
		UE_LOG(LogInventory, Error, TEXT("Item is unusable. '%s'"), *Name.ToString());
		return false;
	}

	// intentionally make a copy since when an object gets removed from the pool, the fname automagically transforms to the next name. W T F
	FName OldName = Name;
	// intentionally calling mod so that onMod is triggered
	Mod(Name, -1);
	// item was the last one in the inventory. we have no more of it.
	if (!Items.Contains(OldName)) return true;

	// TODO verify this works setting the value on the reference
	// at this point the item reference is ok, se keep it.
	Item.ActiveCoolDown = Item.CoolDown;
	if (Item.ActiveCoolDown>0) {
		SetCoolTimerEnabled(true);
	}

	OnUsed.Broadcast(OldName);
	return true;
}

bool UInventory::SetBlocked(const FName& Name, bool NewBlocked) {
	bool Found = false;
	FItem& Item = GetRef(Name, Found);
	if (!Found)	return false;

	Item.IsBlocked = NewBlocked;
	return true;
}

bool UInventory::IsUsable(const FItem& Item) const {
	if (!Item.Usable) return false;
	if (Item.IsBlocked) {
		UE_LOG(LogInventory, Log, TEXT("Item is blocked. title='%s'"), *Item.Title.ToString());
		return false;
	}
	if (!IsCold(Item)) {
		UE_LOG(LogInventory, Log, TEXT("Item is not cold. title='%s' wait=%i"), *Item.Title.ToString(), Item.ActiveCoolDown);
		return false;
	}
	return true;
}

// do i need this?
// returns cold if it doesn't need to cool down, whether it uses or not cooldowns
bool UInventory::IsCold(const FItem& Item) {
	const bool Cold = Item.ActiveCoolDown <= 0;
	UE_LOG(LogInventory, Log, TEXT("Item Is cold?. cold=%i wait=%i title='%s'"), Cold, Item.ActiveCoolDown, *Item.Title.ToString());
	return Cold;
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
		OnCold.Broadcast(ColdItems[i]);
	}
}

FItem* UInventory::AddNew(const FName& Name) {
	if (Items.Contains(Name)) return nullptr;

	FItem OutItem;
	const bool FoundRaw = GetRaw(Name, OutItem);
	if (!FoundRaw) {
		UE_LOG(LogInventory, Warning, TEXT("Attempted to add an item that doesn't exists. Name=%s"), *Name.ToString());
		return nullptr;
	}

	// replace in case .Add changes it
	FItem* pOutItem = &Items.Add(Name, OutItem);
	if (!pOutItem) return nullptr;
	
	// reset transient variables to avoid issues with input.
	pOutItem->Count = 0;
	pOutItem->ActiveCoolDown = 0;
	if (IsValid(pOutItem->LogicType)) { // creates the logic if possible
		UClass* const ManType = pOutItem->LogicType.Get();
		if (IsValid(ManType)) {
			pOutItem->Logic = NewObject<UItemLogic>(this, ManType);
			pOutItem->Logic->Name = Name;
		}
	}

	return pOutItem;
}

FItem& UInventory::GetRef(const FName& Name, bool& OutFound) {
	static FItem FauxItem;
	if (Name.IsNone()) {
		OutFound = false;
		return FauxItem;
	}

	FItem* const pItem = Items.Find(Name);
	OutFound = !!pItem;
	if (!OutFound) {
		UE_LOG(LogInventory, Error, TEXT("Can't get non existent item '%s'"), *Name.ToString());
		return FauxItem;
	}
	
	// needs to return a reference and not return by param since cpp seems to not be able to set the reference to the new object
	// and tries to copy it instead. maybe i should return a pointer by param.... but i prefer this.
	return *pItem;
}

const FItem& UInventory::GetRefC(const FName& Name, bool& OutFound) const {
	static FItem FauxItemConst;
	const FItem* const pItem = Items.Find(Name);
	OutFound = !!pItem;
	if (!OutFound) {
		UE_LOG(LogInventory, Error, TEXT("Can't get non existent item '%s'"), *Name.ToString());
		return FauxItemConst;
	}
	return *pItem;
}
