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

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));

	Player = CreateDefaultSubobject<UCSounder>(TEXT("Player"));
	Player->SetAutoActivate(false);
	Player->bAutoManageAttachment = true;
	Player->TimeFadeIn = 1.0;
}

void AMusicMan::Fade_Implementation(const bool In) {
	Player->Fade(In);
}

void AMusicMan::PlayMusic(USoundBase* Snd, bool FadeOut) {
	if (!IsValid(Snd)) {
		UE_LOG(LogSounds, Log, TEXT("%hs. Sound not valid! Stop"), __func__);
		return;
	}

	UE_LOG(LogSounds, Log, TEXT("%hs '%s'"), __func__, *Snd->GetName());
	
	NextMusic = Snd;
	if (FadeOut && Player->IsPlaying()) Fade(false);
	else SetNextMusic();
}

void AMusicMan::BeginPlay() {
	Super::BeginPlay();

	// used for fade from one music to the next
	Player->OnAudioFinished.AddUniqueDynamic(this, &AMusicMan::SetNextMusic);
	Player->Activate(true); // attempt to start playing if set.
	SetIntensity(0); // doesn't really work if it's not playing
}

void AMusicMan::AudioFinished() {
	if (!IsValid(NextMusic)) {
		UE_LOG(LogSounds, Log, TEXT("%hs. No NextMusic. Stop."), __func__);
		return;
	}
	UE_LOG(LogSounds, Log, TEXT("%hs. NextMusic='%s'"), __func__, *GetNameSafe(NextMusic));

	// schedule a change in music in the next ms.
	// in the hope that would fix the issue on the builds where it doesn't really want to start.
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &AMusicMan::SetNextMusic, .05);
}

void AMusicMan::SetIntensity_Implementation(float V) {
	Intensity = V;
	static FName NInt ="Intensity";
	Player->SetSafeParamFloat(NInt, V);
}

void AMusicMan::SetNextMusic() {
	if (!IsValid(NextMusic)) {
		UE_LOG(LogSounds, Log, TEXT("%hs. No NextMusic. Stop."), __func__);
		return;
	}
	
	UE_LOG(LogSounds, Log, TEXT("%hs '%s'"), __func__, *NextMusic->GetName());

	Player->SetSound(NextMusic);
	Fade(true);
	// reset intensity so it's coherent. and also since we can't apply it before it's playing.
	SetIntensity(Intensity);

	// buddhist say no to attachment (unnecessarily at least). This is important for the above check.
	NextMusic = nullptr;
}

