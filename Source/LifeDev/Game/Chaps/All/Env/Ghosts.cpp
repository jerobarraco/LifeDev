// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "Ghosts.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Components/AudioComponent.h"
#include "Sounds/CSounder.h"

AGhosts::AGhosts():Super() {
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

	Parts = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Parts"));
	Parts->SetupAttachment(RootComponent);
	Parts->SetAutoActivate(false);
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>
		CNiagara(TEXT("/Game/LifeDev/Game/Chaps/All/NPCs/Ghost/Ghost_NS.Ghost_NS"));
	Parts->SetAsset(CNiagara.Object);
}

void AGhosts::SetPlaying(bool IsPlaying) {
	SFX->Fade(IsPlaying);
	Parts->SetActive(true);
}
