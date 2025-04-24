// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "LearnMan.h"

#include "LearnTypes.h"
#include "Inventory/Flags.h"

DEFINE_LOG_CATEGORY_STATIC(LogLearnMan, Log, Log)

ALearnMan::ALearnMan():Super() {
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
}

void ALearnMan::Init(UDataTable* const Data) {
	UE_LOG(LogLearnMan, Log, TEXT("%hs DT=%s"), __func__, *GetNameSafe(Data));
	DT = Data;
	Flags = UFlags::Instance(this);
	UE_CLOG(!!Flags, LogLearnMan, Warning, TEXT("%hs Flag subsystem not found!"), __func__);
}

void ALearnMan::DeInit() {
	DT = nullptr;
	Flags = nullptr;
}

bool ALearnMan::Show(const FName& Id) {
	if (UNLIKELY(!DT | !Flags)) {
		UE_LOG(LogLearnMan, Warning, TEXT("%hs DT or Flags is not ok. DT=%s"), __func__, *GetNameSafe(DT));
		return false;
	}

	const FLearnRow* const pR = DT->FindRow<FLearnRow>(Id, "");
	if (UNLIKELY(!pR)) {
		UE_LOG(LogLearnMan, Warning, TEXT("%hs Row not found. Row=%s"), __func__, *Id.ToString());
		return false;
	}

	const FName FN("Learn." + Id.ToString());
	if (UNLIKELY(Flags->Has(FN))) {
		UE_LOG(LogLearnMan, Warning, TEXT("%hs User already saw this. Row=%s"), __func__, *Id.ToString());
		return true; // Todo false or true?
	}

	OnShow.Broadcast(Id, *pR);
	return true;
}
