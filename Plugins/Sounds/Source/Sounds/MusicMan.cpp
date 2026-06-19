// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "MusicMan.h"

#include "SoundsModule.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"

#include "Sounds/CSounder.h"

AMusicMan::AMusicMan() {
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.SetTickFunctionEnable(false);

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));

	Player = CreateDefaultSubobject<UCSounder>(TEXT("Player"));
	Player->SetAutoActivate(false);
	Player->TimeFadeIn = 1.0;
	// csounder is automanaged by default
}

void AMusicMan::Fade_Implementation(const bool In) {
	Player->Fade(In);
}

void AMusicMan::Play(USoundBase* const Snd, const bool FadeOut) {
	if (UNLIKELY(!IsValid(Snd))) {
		UE_LOG(LogSounds, Log, TEXT("%hs. Sound not valid! Stop"), __func__);
		return;
	}

	UE_LOG(LogSounds, Log, TEXT("%hs '%s'"), __func__, *Snd->GetName());
	
	NextMusic = Snd;
	if (FadeOut & Player->IsPlaying()) Fade(false); // will trigger SetNextMusic on finish
	else SetNextMusic();
}

void AMusicMan::BeginPlay() {
	Super::BeginPlay();

	// used for fade from one music to the next
	Player->OnAudioFinished.AddUniqueDynamic(this, &AMusicMan::SetNextMusic);
	Player->Activate(true); // attempt to start playing if set.
}

void AMusicMan::SetNextMusic() {
	if (UNLIKELY(!IsValid(NextMusic))) {
		UE_LOG(LogSounds, Log, TEXT("%hs. No NextMusic. Stop."), __func__);
		return;
	}
	
	UE_LOG(LogSounds, Log, TEXT("%hs '%s'"), __func__, *NextMusic->GetName());

	Player->SetSound(NextMusic);
	Fade(true);

	// buddhist say no to attachment (unnecessarily at least). This is important for the above check.
	NextMusic = nullptr;
}

