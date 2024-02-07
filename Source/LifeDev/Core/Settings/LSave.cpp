// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "LifeDev/Core/Settings/LSave.h"

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
}

void ULSave::WriteSubsystems(UWorld* const W) {
	UE_LOG(LogLSave, Log, TEXT("%hs"), __func__);

	if (!W) return;

	UFlags* const Flags = UFlags::Instance(W);
	if (Flags) {
		UE_LOG(LogLSave, Log, TEXT("%hs: Writing flags"), __func__);
		Flags->SetAllFlags(SFlags);
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
			
			UE_LOG(LogLSave, Log, TEXT("%hs: Name=%s count=%i"), __func__, *N.ToString(), I);
			Inventory->Mod(N, I);
		}
	}
}

void ULSave::ReadSubsystems(UWorld* const W) {
	if (!W) return;

	UFlags* const Flags = UFlags::Instance(W);
	if (Flags) {
		UE_LOG(LogLSave, Log, TEXT("%hs: Reading Flags"), __func__);
		SFlags = Flags->GetAllFlags();
	}

	UInventory* const Inventory = UInventory::Instance(W);
	if (Inventory) {
		UE_LOG(LogLSave, Log, TEXT("%hs: Reading Inventory"), __func__);
		const TMap<FName, FItem>& Items = Inventory->GetItems();
		SInventory.Empty(Items.Num());

		TArray<FName> Keys;
		Items.GetKeys(Keys);
		for (const FName& N: Keys) {
			const FItem* const pI = Items.Find(N);
			if (!pI) continue;

			const FItem I = *pI;
			UE_LOG(LogLSave, Log, TEXT("%hs: Name=%s count=%i"), __func__, *N.ToString(), I.Count);
			// only saving the count. this is lame. but it's enough for now. good enough > good > perfect.
			SInventory.Add(N, I.Count);
		}
	}
}
