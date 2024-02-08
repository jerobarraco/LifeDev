// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "LifeDev/Core/Settings/LSave.h"

#include "LSettings.h"
#include "LSysSettings.h"
#include "Inventory/Flags.h"
#include "Inventory/Inventory.h"

DEFINE_LOG_CATEGORY_STATIC(LogLSave, Log, Log);

void ULSave::Reset() {
	UE_LOG(LogLSave, Log, TEXT("Savegame reset"));
	
	// ChapterID = ULSysSettings::IsDebugBuild() ? ULSysSettings::Get()->StartChap : 0;
	// not using above since i want to preserve the ability to skip chapters even on shipping builds
	ChapterID = ULSysSettings::Get()->StartChap;
	SInventory.Empty();
	SFlags.Empty();
	Time = 0;
	SlotIndex = 0;
}

void ULSave::WriteSubsystems(UWorld* const W) {
	UE_LOG(LogLSave, Log, TEXT("%hs"), __func__);

	if (!W) return;

	UFlags* const Flags = UFlags::Instance(W);
	if (Flags) {
		UE_LOG(LogLSave, Log, TEXT("%hs: Writing flags"), __func__);
		Flags->SetAll(SFlags);
	}

	UInventory* const Inventory = UInventory::Instance(W);
	if (Inventory) {
		UE_LOG(LogLSave, Log, TEXT("%hs: Writing Inventory"), __func__);
		Inventory->Clear(SInventory.Num());

		TArray<FName> Keys;
		SInventory.GetKeys(Keys);
		for (const FName N: Keys) {
			const int32* const pI = SInventory.Find(N);
			if (!pI) continue;
			const int32 I = *pI;
			
			UE_LOG(LogLSave, Log, TEXT("%hs.Inventory: Name=%s count=%i"), __func__, *N.ToString(), I);
			Inventory->Mod(N, I);
		}
	}

	ULSettings* const Settings = ULSettings::Instance(W);
	if (Settings) {
		UE_LOG(LogLSave, Log, TEXT("%hs.Feats"), __func__);
		for (const EFeat F: WatchFeats) { // only affect the ones we watch.
			const bool Val = SFeats.Contains(F);
			UE_LOG(LogLSave, Log, TEXT("%hs.Feats: Feat=%s Enabled=%i"), __func__,
				*UEnum::GetValueAsString(F), Val);
			Settings->SetFeat(F, Val);
		}
	}
}

void ULSave::ReadSubsystems(UWorld* const W) {
	if (!W) return;

	// this is a bit lame, but it's the cheapest and safest at the moment
	Time = FDateTime::Now().ToUnixTimestamp();
	
	UFlags* const Flags = UFlags::Instance(W);
	if (Flags) {
		UE_LOG(LogLSave, Log, TEXT("%hs: Reading Flags"), __func__);
		SFlags = Flags->GetAll();
	}

	UInventory* const Inventory = UInventory::Instance(W);
	if (Inventory) {
		UE_LOG(LogLSave, Log, TEXT("%hs.Inventory"), __func__);
		const TMap<FName, FItem>& Items = Inventory->GetItems();
		SInventory.Empty(Items.Num());

		TArray<FName> Keys;
		Items.GetKeys(Keys);
		for (const FName& N: Keys) {
			const FItem* const pI = Items.Find(N);
			if (!pI) continue;

			const FItem I = *pI;
			UE_LOG(LogLSave, Log, TEXT("%hs.Inventory: Name=%s count=%i"), __func__, *N.ToString(), I.Count);
			// only saving the count. this is lame. but it's enough for now. good enough > good > perfect.
			SInventory.Add(N, I.Count);
		}
	}

	ULSettings* const Settings = ULSettings::Instance(W);
	if (Settings) {
		UE_LOG(LogLSave, Log, TEXT("%hs.Feats"), __func__);
		SFeats.Empty(); // not reserving. i don't know how many are set and we only store the set ones.
		for (const EFeat F: WatchFeats) { // only affect the ones we watch.
			const bool Val = Settings->GetFeat(F);
			UE_LOG(LogLSave, Log, TEXT("%hs.Feats: Feat=%s Enabled=%i"), __func__,
				*UEnum::GetValueAsString(F), Val);

			if (!Val) continue; // only store if set.

			SFeats.Add(F);
		}
	}
}
