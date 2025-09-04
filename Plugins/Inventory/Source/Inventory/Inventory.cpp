// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT
#include "Inventory.h"

#include "Eval.h"
#include "Engine/DataTable.h"
#include "Engine/AssetManager.h"

#include "ItemLogic.h" //needed for ManType.

#define _IsCold(I) ((I.ActiveCoolDown<=0) | FMath::IsNearlyZero(I.ActiveCoolDown))
#define _IsNotCold(I) ((I.ActiveCoolDown>0) & !FMath::IsNearlyZero(I.ActiveCoolDown))

UInventory* UInventory::Instance(const UObject* const O) {
	if (UNLIKELY(!IsValid(O))) return nullptr;

	const UWorld* const W = O->GetWorld();
	if (UNLIKELY(!IsValid(W))) return nullptr;

	UInventory* const I = W->GetSubsystem<UInventory>();
	return IsValid(I) ? I : nullptr;
}

bool UInventory::Mod(const FName& Name, const int32 Diff, const bool OnlyConsume) {
	if (UNLIKELY(Name.IsNone())) return false;

	UE_LOG(LogInventory, Log, TEXT("%hs name=%s, diff=%i"),
		__func__, *Name.ToString(), Diff);

	FName NewSel = NAME_None; // name for the new selection, none if not changed
	// need another flag because we might want to set the selected to none
	bool SetSelect = false;

	// get or create the item
	FItem* Item = Items.Find(Name);
	if (!Item) {
		if (UNLIKELY(Diff<=0)) {
			UE_LOG(LogInventory, Warning, TEXT("%hs Attempt to substract from an item i don't have. Name=%s diff=%i"),
				__func__, *Name.ToString(), Diff);
			return false;
		}

		Item = AddNew(Name);
		if (UNLIKELY(!Item)) {
			UE_LOG(LogInventory, Warning, TEXT("%hs Failed to create item object. Name=%s"),
				__func__, *Name.ToString());
			return false;
		}
		
		// select the new one if nothing was selected
		if (UNLIKELY(Selected.IsNone())) {
			NewSel = Name;
			SetSelect = true;
		}
	}

	// if it's just a consume operation, and the item is not consumable, just skip and return true (the item exists).
	// if it's consumable continue and follow the rest of the checks. if Diff>0 and consume, it makes no sense. but let's treat it the same way for now.
	// if it's not OnlyConsume lets continue normally
	if (UNLIKELY(OnlyConsume & !Item->Consumable)) return true;

	/// update the item count
	
	// calculate the difference. are clamped to the produce (0, MaxCount)
	const int32 Current = Item->Count;
	// get the max positive possible. Current+Diff to allow to grow (clamped below).
	const int32 Max = Item->MaxCount <= 0 ? (Current + Diff): Item->MaxCount;
	UE_CLOG(Diff < -Current, LogInventory, Warning,
		TEXT("%hs Attempt to decrease beyond owned quantity. It will get clamped! Current=%i Diff=%i"),
		__func__, Current, Diff);
	// clamp the diff to the max. and min. -Current means it can't reach <0, but stay on 0
	const int32 CurDiff = FMath::Clamp(Diff, -Current, Max - Current);

	// notify the caller that we haven't changed anything. also avoid triggering an onMod and selection.
	if (UNLIKELY(CurDiff == 0)) {
		UE_LOG(LogInventory, Warning, TEXT("%hs Item unchanged. Diff is 0."
			"Maybe it has reached the maximum. or tried to go negative. Name=%s"),
			__func__, *Name.ToString());
		return false;
	}

	UE_LOG(LogInventory, Log, TEXT("%hs Changing item Name=%s from=%i by=%i"),
			__func__, *Name.ToString(), Current, CurDiff);
	Item->Count = Current + CurDiff; // apply change

	// intentionally copying the item, to avoid issues. the item might have been removed, or might 
	FItem ItemCopy = *Item;
	// remove empty consumables
	// important to remove items with quantity 0. used for "Has()"
	if (Item->Count <= 0) {
		// preemptively select the next one
		// this code sucks, i don't like it. todo improve.
		if (Name == Selected) {
			NewSel = GetNextKey();
			SetSelect = true;
		}
		Items.Remove(Name);
	}

	// finally notify. using move since curDiff is cached.
	OnMod.Broadcast(Name, CurDiff, MoveTemp(ItemCopy));

	// Set selected only after removing.
	// there's something fishy going on. otherwise it will remove the wrong object!
	// also triggering it at the end works better with the ui
	if (UNLIKELY(SetSelect)) SetSelected(NewSel);

	return true;
}

bool UInventory::Ensure(const FName& Name) {
	if (LIKELY(Has(Name))) return true;

	UE_LOG(LogInventory, Log, TEXT("%hs Ensured item. name=%s"),
		__func__, *Name.ToString());
	return Mod(Name, 1);
}

bool UInventory::Rem(const FName& Name) {
	// this is a bit faster than using mod. But needs to ensure it triggers all the correct delegates.
	FItem Item;
	// Important that Get returns a copy, since we need to return this on Mod
	const bool Has = Get(Name, Item);
	// No item with 0 or negative should be stored. but check anyway.
	if (UNLIKELY(!Has)) return false;
	
	const FName NextKey = GetNextKey();
	UE_LOG(LogInventory, Log, TEXT("%hs Removing item. name=%s, nextSelect=%s"),
		__func__, *Name.ToString(), *NextKey.ToString());
	
	Items.Remove(Name);
	const int32 Diff = -Item.Count;
	Item.Count = 0; // it needs to be zeroed out, or people listening to this will get confused. (LInventoryUI)
	OnMod.Broadcast(Name, Diff, MoveTemp(Item)); // i use movetemp here only because i cache "Diff" and i don't use it anymore.

	SetSelected(NextKey);
	return true;
}

bool UInventory::Clear(const int32 NumReserve) {
	UE_LOG(LogInventory, Log, TEXT("%hs Clearing all items. Reserve=%i"), __func__, NumReserve);

	TArray<FName> Keys;
	Items.GetKeys(Keys);
	for (const FName K: Keys) {
		Rem(K);
	}

	Items.Reserve(NumReserve);
	return true;
}

bool UInventory::GetRaw(const FName& Name, FItem& OutItem) const {
	if (UNLIKELY(!IsValid(DT))) return false;

	const FItem* const Item = DT->FindRow<FItem>(Name, TEXT(""));
	if (UNLIKELY(!Item)) return false;

	// set the item anyway even if not found
	OutItem = *Item; // note this is a copy
	return true;
}

bool UInventory::Get(const FName& Name, FItem& OutItem) const {
	if (UNLIKELY(Name.IsNone())) return false;

	const FItem* const pItem = Items.Find(Name);
	if (UNLIKELY(!pItem)) return false;

	OutItem = *pItem; // purposely return a copy
	return true;
}

int32 UInventory::Count(const FName& Name) const {
	FItem Item;
	const bool Ok = Get(Name, Item);
	return LIKELY(Ok) ? Item.Count : 0;
}

void UInventory::Init(UDataTable* const DataTable) {
	if (LIKELY(IsValid(DataTable))) DT = DataTable;
}

void UInventory::DeInit() {
	DT = nullptr;
	Items.Empty();
}

const TMap<FName, FItem>& UInventory::GetAll() const {
	return Items;
}

void UInventory::SetItems(const TMap<FName, FItem>& NewItems) {
	Items = NewItems;
}

bool UInventory::GetSelectedItem(FItem& Item) const {
	if (UNLIKELY(Selected.IsNone())) {
		UE_LOG(LogInventory, Warning, TEXT("%hs No item is selected."), __func__);
		return false;
	}
	
	if (LIKELY(Get(Selected, Item))) return true;
	
	UE_LOG(LogInventory, Warning, TEXT("%hs Item does not exists? But that shouldn't happen here."),
		__func__);
	return false;
}

FName UInventory::GetNextKey(const bool Forward, FName From) const {
	if (UNLIKELY(From.IsNone())) {
		if (UNLIKELY(Selected.IsNone())) return NAME_None;
		From = Selected;
	}

	TArray<FName> Keys;
	Items.GetKeys(Keys);
	// <2 because one will get removed. and we need to tell this situation apart to clear the selected
	if (UNLIKELY(Keys.Num()<2)) return NAME_None;

	const int32 Num = Keys.Num();
	int32 Index = Keys.Find(From);
	Index += Forward ? 1 : -1; // shift
	Index += Num; // pad in case it goes into negatives
	Index = Index % Num; // wrap
	return Keys[Index];
}

bool UInventory::SetSelected(const FName& Name) {
	if (UNLIKELY(!Name.IsNone() && !Items.Contains(Name))) return false;
	if (UNLIKELY(Name == Selected)) return false;
	
	Selected = Name;
	OnSelected.Broadcast(Selected);
	return true;
}

bool UInventory::Has(const FName& Name) const {
	if (UNLIKELY(Name.IsNone())) return false;
	// note that this depends on items being removed when quantity is <=0
	return Items.Contains(Name);
}

bool UInventory::Use(const FName& Name) {
	bool Found = false;
	FItem& Item = GetRef(Name, Found);
	if (UNLIKELY(!Found)) return false;

	if (UNLIKELY(!IsUsable(Item))) {
		UE_LOG(LogInventory, Warning, TEXT("%hs Item is unusable. Stop. '%s'"),
			__func__, *Name.ToString());
		return false;
	}

	UE_LOG(LogInventory, Log, TEXT("%hs item=%s"), __func__, *Name.ToString());

	// this works setting the value on the reference
	// at this point the item reference is ok, so keep it.
	// set before calling Mod, since mod will dispatch OnMod
	// if this is the last one, then it makes no difference. who cares.
	Item.ActiveCoolDown = Item.CoolDown;
	if (_IsNotCold(Item)) SetCoolTimerEnabled(true);

	// intentionally make a copy since when an object gets removed from the pool,
	// the fname automagically transforms to the next name. W T F
	// (maybe the TArray moves instead of copying)
	const FName OldName = Name;

	// intentionally calling mod so that OnMod is triggered (if needed)
	Mod(Name, -1, true); // consume if needed

	OnUsed.Broadcast(OldName);
	return true;

	// Not triggering the item->Logic here since that's only for self-usage.
	// and we need to support using with other items.
	// since a self-usable item can also be used with other items, only the caller really knows.
}

bool UInventory::SetLocked(const FName& Name, const bool NewBlocked) {
	bool Found = false;
	FItem& Item = GetRef(Name, Found);
	if (UNLIKELY(!Found)) return false;

	Item.Locked = NewBlocked;
	return true;
}

bool UInventory::SetCold(const FName& Name) {
	UE_LOG(LogInventory, Log, TEXT("%hs ='%s'"),
		__func__, *Name.ToString());
	bool Found = false;
	FItem& Item = GetRef(Name, Found);
	if (UNLIKELY(!Found)) return false;
	if (_IsCold(Item)) return false;

	Item.ActiveCoolDown = 0;
	OnCold.Broadcast(Name);
	return true;
}

bool UInventory::IsCold(const FItem& Item) {
	const bool Cold = _IsCold(Item);
	UE_LOG(LogInventory, Log, TEXT("%hs cold=%i wait=%.4f title='%s'"),
		__func__, Cold, Item.ActiveCoolDown, *Item.Title.ToString());
	return Cold;
}

bool UInventory::IsUsable(const FItem& Item) const {
	if (!Item.Usable) return false;

	if (Item.Locked) {
		UE_LOG(LogInventory, Log, TEXT("%hs Item is locked. title='%s'"),
			__func__, *Item.Title.ToString());
		return false;
	}

	if (_IsNotCold(Item)) {
		UE_LOG(LogInventory, Log, TEXT("%hs Item is not cold. title='%s' wait=%.4f"),
			__func__, *Item.Title.ToString(), Item.ActiveCoolDown);
		return false;
	}

	// TODO make static if unused (what?)
	if (!Item.UseCondition.IsEmpty()) { // todo remove if unused
		const UEval* const Eval = UEval::Instance(this);
		double Res = 0;
		if (LIKELY(Eval)) Eval->Eval(Item.UseCondition, Res);
		return Res > 0;
	}

	return true;
}

void UInventory::SetCoolTimerEnabled(const bool Enable) {
	const UWorld* const World = GetWorld();
	if (LIKELY(!World)) return;

	// done this way since there could be many items hot at the same time.
	
	FTimerManager& Time = World->GetTimerManager();

	if (Enable) {
		// check if the timer is still valid
		if (CoolTimer.IsValid() && Time.TimerExists(CoolTimer)) return;
		Time.SetTimer(CoolTimer, this, &UInventory::CoolTimerTick, CoolTimerRate, true);
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
		if (_IsCold(Item)) continue;
		
		Item.ActiveCoolDown = FMath::Max(0, Item.ActiveCoolDown-CoolTimerRate); // update cooldown, make sure to clamp
		if (_IsNotCold(Item)) { // has to re-check again.
			AllCool = false;
			continue;
		}

		ColdItems.Add(Name);
	}

	if (UNLIKELY(AllCool))
		SetCoolTimerEnabled(false); // remove timer

	/// notify
	const int32 ColdNum = ColdItems.Num();
	for (int32 i=0; i<ColdNum; ++i) {
		OnCold.Broadcast(ColdItems[i]);
	}
}

FItem* UInventory::AddNew(const FName& Name) {
	if (UNLIKELY(Items.Contains(Name))) return nullptr;

	FItem OutItem;
	const bool FoundRaw = GetRaw(Name, OutItem);
	if (UNLIKELY(!FoundRaw)) {
		UE_LOG(LogInventory, Warning,
			TEXT("%hs Attempted to add an item that doesn't exists. Name=%s"), __func__, *Name.ToString());
		return nullptr;
	}

	// replace in case .Add changes it
	FItem* const pOutItem = &Items.Add(Name, OutItem);
	if (UNLIKELY(!pOutItem)) return nullptr;
	
	// reset transient variables to avoid issues with input.
	pOutItem->Count = 0;
	pOutItem->ActiveCoolDown = 0;
	if (IsValid(pOutItem->LogicType)) { // creates the logic if possible
		const UClass* const ManType = pOutItem->LogicType.Get();
		if (LIKELY(IsValid(ManType))) {
			// outer is important so that it triggers BeginPlay on the itemLogic, and its GetWorld works. 
			pOutItem->Logic = NewObject<UItemLogic>(this, ManType);
			pOutItem->Logic->Name = Name;
		}
	}

	TSoftObjectPtr<USoundBase>& Snd = pOutItem->Snd;
	// https://dev.epicgames.com/documentation/en-us/unreal-engine/asynchronous-asset-loading-in-unreal-engine\https://forums.unrealengine.com/t/would-it-be-possible-to-get-some-explanation-for-async-asset-loading/282540/4?u=nandehttps://forums.unrealengine.com/t/would-it-be-possible-to-get-some-explanation-for-async-asset-loading/282540/4?u=nande
	// https://forums.unrealengine.com/t/would-it-be-possible-to-get-some-explanation-for-async-asset-loading/282540/4?u=nande
	if (UseSndAutoLoad && Snd.ToSoftObjectPath().IsValid() && !Snd.IsValid()) {
		UE_LOG(LogInventory, Log, TEXT("%hs Loading sound. Name=%s Async=%i Snd=%s"),
			__func__, *Name.ToString(), UseSndAsyncLoad, *Snd.ToString());
		if (UseSndAsyncLoad) 
			UAssetManager::GetStreamableManager().RequestSyncLoad(Snd.ToSoftObjectPath());
		else
			Snd.LoadSynchronous();
	}

	return pOutItem;
}

FItem& UInventory::GetRef(const FName& Name, bool& OutFound) {
	static FItem FauxItem;
	if (UNLIKELY(Name.IsNone())) {
		OutFound = false;
		return FauxItem;
	}

	FItem* const pItem = Items.Find(Name);
	OutFound = bool(pItem);
	if (UNLIKELY(!OutFound)) {
		UE_LOG(LogInventory, Warning, TEXT("%hs Item does not exist. '%s'"), __func__, *Name.ToString());
		return FauxItem;
	}
	
	// needs to return a reference and not return by param since cpp seems to not be able to set the reference to the new object
	// and tries to copy it instead. maybe i should return a pointer by param.... but i prefer this.
	return *pItem;
}

const FItem& UInventory::GetRefC(const FName& Name, bool& OutFound) const {
	static FItem FauxItemConst;
	const FItem* const pItem = Items.Find(Name);
	OutFound = bool(pItem);
	if (UNLIKELY(!OutFound)) {
		UE_LOG(LogInventory, Warning, TEXT("%hs Item does not exist. '%s'"), __func__, *Name.ToString());
		return FauxItemConst;
	}

	return *pItem;
}
