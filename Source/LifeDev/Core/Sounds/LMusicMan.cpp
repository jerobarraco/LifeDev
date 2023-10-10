// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LMusicMan.h"

#include "Story/Step.h"
#include "Story/Story.h"

#include "LifeDev/Game/Flashback/Flashback.h"

void ALMusicMan::BeginPlay() {
	Super::BeginPlay();
	if (!Enabled) return;

	UWorld* const W = GetWorld();
	UFlashback* const Flashback = UFlashback::Get(W);
	if (Flashback) {
		Flashback->OnChange.AddUniqueDynamic(this, &ALMusicMan::SetIntensity);
	}

	UStory* const Story = UStory::Get(W);
	if (Story) {
		Story->OnStart.AddUniqueDynamic(this, &ALMusicMan::SetStep);
	}
}

void ALMusicMan::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	UWorld* const W = GetWorld();
	UFlashback* const Flashback = UFlashback::Get(W);
	if (Flashback) {
		Flashback->OnChange.RemoveAll(this);
	}

	UStory* const Story = UStory::Get(W);
	if (Story) {
		Story->OnStart.RemoveAll(this);
	}

	Super::EndPlay(EndPlayReason);
}

void ALMusicMan::SetStep(AStep* Step) {
	if (!IsValid(Step)) return;
	if (Step->Music.IsNull()) return;
	
	USoundBase* const Sound = Step->Music.LoadSynchronous();
	if (!IsValid(Sound)) return;
	PlayMusic(Sound);
}
