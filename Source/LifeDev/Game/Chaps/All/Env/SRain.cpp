// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "SRain.h"

#include "Components/AudioComponent.h"

ASRain::ASRain():Super() {
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));

	SFX = CreateDefaultSubobject<UAudioComponent>(TEXT("SFX"));
	SFX->SetupAttachment(RootComponent);
	SFX->SetAutoActivate(false);
	SFX->SetHiddenInGame(true);

	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Chaps/All/Env/Rain/Rain01.Rain01"));
	SFX->SetSound(CSnd.Object);
}

void ASRain::Play() {
	if (!IsValid(SFX)) return;
	const int32 Time = FMath::RandRange(0, 120);
	SFX->FadeIn(FadeIn, 1, Time);
}

void ASRain::Stop() {
	if (!IsValid(SFX)) return;
	SFX->FadeOut(FadeOut, 0);
	SFX->StopDelayed(FadeOut);
}
