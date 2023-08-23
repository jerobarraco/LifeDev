// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "MusicMan.h"

#include "Components/AudioComponent.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "Sounds/CSounder.h"

AMusicMan::AMusicMan():Super() {
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.SetTickFunctionEnable(false);

	Player = CreateDefaultSubobject<UCSounder>(TEXT("Player"));
	Player->bAutoManageAttachment=true;
	Player->SetAutoActivate(false);
}

void AMusicMan::Fade(bool In) {
	Player->SetPlaying(In);
}

void AMusicMan::PlayMusic(USoundBase* Snd, bool FadeOut) {
	if (!IsValid(Snd)) return;

	NextMusic = Snd;
	if (FadeOut && Player->IsPlaying()) {
		Player->OnAudioFinished.AddUniqueDynamic(this, &AMusicMan::SetNextMusic);
		Player->SetPlaying(false);
	} else {
		SetNextMusic();
	}
}

void AMusicMan::BeginPlay() {
	Super::BeginPlay();
	if (!Enabled) return;

	UFlashback* const Flashback = UFlashback::Get(GetWorld());
	if (!Flashback) return;
	
	SetIntensity(0);
	Flashback->OnChange.AddUniqueDynamic(this, &AMusicMan::SetIntensity);
	Player->Activate(true);
}

void AMusicMan::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	UFlashback* const Flashback = UFlashback::Get(GetWorld());
	if (Flashback) {
		Flashback->OnChange.RemoveAll(this);
	}

	Super::EndPlay(EndPlayReason);
}

void AMusicMan::SetIntensity(float V) {
	if (!IsValid(Player) || ! Player->IsPlaying()) return;
	static FName NInt ="Intensity";
	Player->SetFloatParameter(NInt, V);
}

void AMusicMan::SetNextMusic() {
	Player->OnAudioFinished.RemoveAll(this);
	Player->SetSound(NextMusic);
	Fade(true);
	// buddhist say no to attachment (unnecessarily at least)
	NextMusic = nullptr;
}

