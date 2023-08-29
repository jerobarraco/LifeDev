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
		Fade(false);
	} else {
		SetNextMusic();
	}
}

void AMusicMan::BeginPlay() {
	Super::BeginPlay();
	if (!Enabled) return;
	
	Player->OnAudioFinished.AddUniqueDynamic(this, &AMusicMan::AudioFinished);
	Player->Activate(true); // attempt to start playing if set.
	SetIntensity(0); // doesn't really work if it's not playing
}

void AMusicMan::AudioFinished() {
	if (!IsValid(NextMusic)) {
		UE_LOG(LogSounds, Log, TEXT("MusicMan AudioFinished. No NextMusic."));
		return;
	}
	UE_LOG(LogSounds, Log, TEXT("MusicMan AudioFinished. NextMusic='%s'"), *NextMusic->GetName());

	// schedule a change in music in the next 100 ms.
	// in the hope that would fix the issue on the builds where it doesn't really wanna start.
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &AMusicMan::SetNextMusic, .05);
}

void AMusicMan::SetIntensity(float V) {
	Intensity = V;
	const bool CanSet = !IsValid(Player) || ! Player->IsPlaying();
	
	// avoid crashing
	if (CanSet) return;
	static FName NInt ="Intensity";
	Player->SetFloatParameter(NInt, V);
}

void AMusicMan::SetNextMusic() {
	if (!IsValid(NextMusic)) {
		UE_LOG(LogSounds, Log, TEXT("MusicMan SetNextMusic. No NextMusic."));
		return;
	}
	
	UE_LOG(LogSounds, Log, TEXT("MusicMan SetNextMusic '%s'"), *NextMusic->GetName());

	Player->SetSound(NextMusic);
	Fade(true);
	// reset intensity so it's coherent. and also since we can't apply it before it's playing.
	SetIntensity(Intensity);

	// buddhist say no to attachment (unnecessarily at least). This is important for the above check.
	NextMusic = nullptr;
}

