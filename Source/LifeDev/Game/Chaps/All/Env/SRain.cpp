// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "SRain.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sounds/CSounder.h"

// TODO move to the music manager (LMusicMan)

ASRain::ASRain():Super() {
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));

	SFX = CreateDefaultSubobject<UCSounder>(TEXT("SFX"));
	SFX->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Chaps/All/Env/Rain/Rain01_S.Rain01_S"));
	SFX->SetSound(CSnd.Object);
	SFX->TimeFadeIn = 2;
	SFX->TimeFadeOut = 2;
	SFX->TimeStart = 0;
	SFX->TimeEnd = 120;
}

void ASRain::SetPlaying(bool IsPlaying) {
	if (!IsValid(SFX)) return;
	SFX->Fade(IsPlaying);
}

void ASRain::SSetPlaying(UWorld* W, bool IsPlaying) {
	ASRain* const R = Cast<ASRain>(UGameplayStatics::GetActorOfClass(W, ASRain::StaticClass()));
	if (R) { R->SetPlaying(IsPlaying); }
}
