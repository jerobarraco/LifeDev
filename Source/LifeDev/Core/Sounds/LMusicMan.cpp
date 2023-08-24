// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LMusicMan.h"

#include "LifeDev/Game/Flashback/Flashback.h"

void ALMusicMan::BeginPlay() {
	Super::BeginPlay();
	if (!Enabled) return;

	UFlashback* const Flashback = UFlashback::Get(GetWorld());
	if (!Flashback) return;
	Flashback->OnChange.AddUniqueDynamic(this, &ALMusicMan::SetIntensity);
}

void ALMusicMan::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	UFlashback* const Flashback = UFlashback::Get(GetWorld());
	if (Flashback) {
		Flashback->OnChange.RemoveAll(this);
	}

	Super::EndPlay(EndPlayReason);
}
