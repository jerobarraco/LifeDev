// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "LifeDev/Core/Settings/LSave.h"

#include "Inventory/Flags.h"
#include "Inventory/Inventory.h"

#include "LifeDev/Core/Consts/ConstFlags.h"

#include "LSettings.h"
#include "LSysSettings.h"

DEFINE_LOG_CATEGORY_STATIC(LogLSave, Log, Log);

////////////
///// DO NOT CALL GET WORLD HERE. IT WON'T WORK!
///////////////

void ULSave::Reset(const UObject* const O) {
	UE_LOG(LogLSave, Log, TEXT("Savegame reset"));
	
	// ChapterID = ULSysSettings::IsDebugBuild() ? ULSysSettings::Get()->StartChap : 0;
	// not using the above, since i want to preserve the ability to skip chapters even on shipping builds
	Time = FDateTime::UtcNow().ToUnixTimestamp(); // using utc for portability reasons
	Version = VersionLast;
	SInventory.Empty();
	SFlags.Empty();
	SFeats.Empty();

	const ULSysSettings* const SysSettings = ULSysSettings::Get();
	if (UNLIKELY(!SysSettings)) {
		UE_LOG(LogLSave, Warning, TEXT("%hs Can't get the ULSysSettings. Stop"), __func__);
		return;
	}

	// actually using StartChap and not GetStartChap to allow changing the start chap on shipping builds
	ChapterID = LIKELY(SysSettings) ? SysSettings->StartChap : 0;

	// read the feats. load with default ones.
	UE_LOG(LogLSave, Log, TEXT("%hs.Feats"), __func__);
	const TSet<EFeat>& Feats = SysSettings->GetFeats();
	for (const EFeat F: WatchFeats) { // only affect the ones we watch.
		const bool Val = Feats.Contains(F);
		UE_LOG(LogLSave, Log, TEXT("%hs.Feats: Feat=%s Enabled=%i"), __func__,
			*UEnum::GetValueAsString(F), Val);

		if (!Val) continue; // only store if set.

		SFeats.Add(F);
	}

	// foxify the game
	SFlags.Add(LDConsts::Flags::Settings::Global::Foxy, FMath::FRand());

	// TODO this is a bit risky. keep an eye on it
	// WriteSubsystems(O);
}

void ULSave::WriteSubsystems(const UObject* const O) {
	UE_LOG(LogLSave, Log, TEXT("%hs"), __func__);

	const UWorld* const W = LIKELY(O) ? O->GetWorld(): nullptr;
	if (UNLIKELY(!W)) {
		UE_LOG(LogLSave, Warning, TEXT("%hs. The world is fake! Can't continue."), __func__);
		return;
	}

	UFlags* const Flags = UFlags::Instance(W);
	if (LIKELY(Flags)) {
		UE_LOG(LogLSave, Log, TEXT("%hs: Writing flags"), __func__);
		Flags->SetAll(SFlags);
	}

	UInventory* const Inventory = UInventory::Instance(W);
	if (LIKELY(Inventory)) {
		UE_LOG(LogLSave, Log, TEXT("%hs: Writing Inventory"), __func__);
		Inventory->Clear(SInventory.Num());

		for (const TTuple<FName, int32>& KV: SInventory) {
			UE_LOG(LogLSave, Log, TEXT("%hs.Inventory: Name=%s count=%i"), __func__, *KV.Key.ToString(), KV.Value);
			Inventory->Mod(KV.Key, KV.Value);
		}
	} else {
		UE_LOG(LogLSave, Warning, TEXT("%hs: Can't obtain the Inventory. Skipped!"), __func__);
	}

	ULSettings* const Settings = ULSettings::Instance(W);
	if (LIKELY(Settings)) {
		UE_LOG(LogLSave, Log, TEXT("%hs.Feats"), __func__);
		// only affect the ones we watch. important or this will remove all the other flags.
		for (const EFeat F: WatchFeats) {
			const bool Val = SFeats.Contains(F);
			UE_LOG(LogLSave, Log, TEXT("%hs.Feats: Feat=%s Enabled=%i"), __func__,
				*UEnum::GetValueAsString(F), Val);
			Settings->SetFeat(F, Val);
		}
	}
}

void ULSave::ReadSubsystems(const UObject* const O, const bool WithInventory) {
	const UWorld* const W = LIKELY(O) ? O->GetWorld(): nullptr;
	if (UNLIKELY(!W)) {
		UE_LOG(LogLSave, Warning, TEXT("%hs. The world is fake! Can't continue."), __func__);
		return;
	}

	// this is a bit lame, but it's the cheapest and safest at the moment
	Time = FDateTime::Now().ToUnixTimestamp();

	const UFlags* const Flags = UFlags::Instance(W);
	if (LIKELY(Flags)) {
		UE_LOG(LogLSave, Log, TEXT("%hs: Reading Flags"), __func__);
		SFlags = Flags->GetAll();
	}

	const UInventory* const Inventory = UInventory::Instance(W);
	if (LIKELY(Inventory && WithInventory)) {
		UE_LOG(LogLSave, Log, TEXT("%hs.Inventory"), __func__);
		const TMap<FName, FItem>& Items = Inventory->GetAll();
		SInventory.Empty(Items.Num());

		for (const TTuple<FName, FItem>& I: Items) {
			UE_LOG(LogLSave, Log, TEXT("%hs.Inventory: Name=%s count=%i"), __func__, *I.Key.ToString(), I.Value.Count);
			// only saving the count. this is lame. but it's enough for now. good enough > good > perfect.
			SInventory.Add(I.Key, I.Value.Count);
		}
	}

	const ULSettings* const Settings = ULSettings::Instance(W);
	if (LIKELY(Settings)) {
		UE_LOG(LogLSave, Log, TEXT("%hs.Feats"), __func__);
		SFeats.Empty(); // not reserving. i don't know how many are set, and we only store the set ones.
		for (const EFeat F: WatchFeats) { // only affect the ones we watch.
			const bool Val = Settings->GetFeat(F);
			UE_LOG(LogLSave, Log, TEXT("%hs.Feats: Feat=%s Enabled=%i"), __func__,
				*UEnum::GetValueAsString(F), Val);

			if (!Val) continue; // only store if set.

			SFeats.Add(F);
		}
	}
}
