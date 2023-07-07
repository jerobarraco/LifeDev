// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "SGhosts.h"

#include "Components/AudioComponent.h"
#include "Sounds/CSounder.h"

ASGhosts::ASGhosts():Super() {
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));

	SFX = CreateDefaultSubobject<UCSounder>(TEXT("SFX"));
	SFX->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/Ghost/Ghosts_S.Ghosts_S"));
	SFX->SetSound(CSnd.Object);
	SFX->TimeFadeIn = 1;
	SFX->TimeFadeOut = 2.0;
	SFX->TimeStart = 0;
	SFX->TimeEnd = 120;
}

void ASGhosts::SetPlaying(bool IsPlaying) {
	SFX->SetPlaying(IsPlaying);
}
