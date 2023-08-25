// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#include "MusicMan.h"

#include "SoundsModule.h"
#include "Components/AudioComponent.h"
#include "Sounds/CSounder.h"

AMusicMan::AMusicMan():Super() {
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.SetTickFunctionEnable(false);

	Player = CreateDefaultSubobject<UCSounder>(TEXT("Player"));
	Player->bAutoManageAttachment=true;
	Player->SetAutoActivate(false);
	Player->TimeFadeIn = 1.0;
}

void AMusicMan::Fade(bool In) {
	Player->SetPlaying(In);
}

void AMusicMan::PlayMusic(USoundBase* Snd, bool FadeOut) {
	if (!IsValid(Snd)) return;

	UE_LOG(LogSounds, Log, TEXT("MusicMan PlayMusic '%s'"), *Snd->GetName());
	
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

	Player->Activate(true);
	SetIntensity(0); // doesn't really work if it's not playing
}


void AMusicMan::UpdateState(EAudioComponentPlayState PlayState) {
	// not checking the other states. to not mangle the logic.
	if (PlayState == EAudioComponentPlayState::Playing) {
		// reset intensity so it's coherent. and also since we can't apply it before it's playing.
		SetIntensity(Intensity);
	} 
}

void AMusicMan::SetIntensity(float V) {
	Intensity = V;
	const bool CanSet = !IsValid(Player) || ! Player->IsPlaying();
	UE_LOG(LogTemp, Log, TEXT("MusicMan SetIntensity %.5f %i"), V, CanSet);
	
	// avoid crashing
	if (CanSet) return;
	static FName NInt ="Intensity";
	Player->SetFloatParameter(NInt, V);
}

void AMusicMan::SetNextMusic() {
	Player->OnAudioFinished.RemoveAll(this);
	if (!IsValid(NextMusic)) return;

	UE_LOG(LogSounds, Log, TEXT("MusicMan SetNextMusic '%s'"), *NextMusic->GetName());

	Player->SetSound(NextMusic);
	Fade(true);

	// buddhist say no to attachment (unnecessarily at least). This is important for the above check.
	NextMusic = nullptr;
}

