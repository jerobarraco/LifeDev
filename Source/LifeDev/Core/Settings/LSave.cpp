// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "LifeDev/Core/Settings/LSave.h"

#include "LSysSettings.h"
#include "Inventory/Flags.h"
#include "Inventory/Inventory.h"

void ULSave::Reset() {
	// ChapterID = ULSysSettings::IsDebugBuild() ? ULSysSettings::Get()->StartChap : 0;
	// i want to preserve the ability to skip chapters even on shipping builds
	ChapterID = ULSysSettings::Get()->StartChap;
	SInventory.Empty();
	SFlags.Empty();
}

// TODO fix issue with cards not being restored (having count of -1)

void ULSave::WriteSubsystems(UWorld* const W) {
	if (!W) return;

	UFlags* const Flags = UFlags::Instance(W);
	if (Flags) {
		Flags->SetAllFlags(SFlags);
	}

	UInventory* const Inventory = UInventory::Instance(W);
	if (Inventory) {
		Inventory->Clear(SInventory.Num());

		TArray<FName> Keys;
		SInventory.GetKeys(Keys);
		for (const FName N: Keys) {
			const int32* const pI = SInventory.Find(N);
			if (!pI) continue;
			const int32 I = *pI;
			
			UE_LOG(LogTemp, Log, TEXT("ULSave::Writesubs. Name=%s count=%i"), *N.ToString(), I);
			Inventory->Mod(N, I);
		}
	}
}

void ULSave::ReadSubsystems(UWorld* const W) {
	if (!W) return;

	UFlags* const Flags = UFlags::Instance(W);
	if (Flags) {
		SFlags = Flags->GetAllFlags();
	}

	UInventory* const Inventory = UInventory::Instance(W);
	if (Inventory) {
		const TMap<FName, FItem>& Items = Inventory->GetItems();
		SInventory.Empty(Items.Num());
		TArray<FName> Keys;
		Items.GetKeys(Keys);
		for (const FName& N: Keys) {
			const FItem* const pI = Items.Find(N);
			if (!pI) continue;
			const FItem I = *pI;
			SInventory.Add(N, I.Count);
			UE_LOG(LogTemp, Log, TEXT("ULSave::ReadSubs. Item=%s count=%i"), *N.ToString(), I.Count);
		}
	}
}
