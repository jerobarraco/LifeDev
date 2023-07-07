// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "CSounder.h"

#include "Components/AudioComponent.h"

UCSounder::UCSounder():Super() {
	PrimaryComponentTick.bCanEverTick = false;
	Super::SetAutoActivate(false);
	SetHiddenInGame(true);
	//
	// static ConstructorHelpers::FObjectFinder<USoundBase>
	// 	CSnd(TEXT("/Game/LifeDev/Game/Chaps/All/Env/Rain/Rain01.Rain01"));
	// SFX->SetSound(CSnd.Object);
}

void UCSounder::SetPlaying(bool InPlaying) {
	if (InPlaying) {
		const float Time = TimeEnd <0 ? TimeStart :
		FMath::RandRange(TimeStart, TimeEnd);
		FadeIn(TimeFadeIn, 1, Time);
	} else {
        FadeOut(TimeFadeOut, 0);
        StopDelayed(TimeFadeOut);
	}
}
