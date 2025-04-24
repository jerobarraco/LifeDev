// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "LearnMan.h"

#include "LearnTypes.h"

DEFINE_LOG_CATEGORY_STATIC(LogLearnMan, Log, Log)

ALearnMan::ALearnMan():Super() {
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
}

void ALearnMan::Init(UDataTable* const Data) {
	DT = Data;
	UE_LOG(LogLearnMan, Log, TEXT("%hs DT=%s"), __func__, *GetNameSafe(Data));
}

void ALearnMan::DeInit() {
	DT = nullptr;
}

bool ALearnMan::Show(const FName& Id) {
	if (UNLIKELY(!DT)) {
		UE_LOG(LogLearnMan, Warning, TEXT("%hs DT is not ok. DT=%s"), __func__, *GetNameSafe(DT));
		return false;
	}

	const FLearnRow* const R = DT->FindRow<FLearnRow>(Id, "");
	if (UNLIKELY(!R)) {
		UE_LOG(LogLearnMan, Warning, TEXT("%hs Row not found. Row=%s"), __func__, *Id.ToString());
		return false;
	}
// TODO check

	return true;
}
