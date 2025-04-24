// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "LearnMan.h"

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
