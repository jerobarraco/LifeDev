// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "Ghosts.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

#include "LifeDev/Core/Sounds/CLSounder.h"
#include "LifeDev/Core/Consts/ConstSettings.h"

AGhosts::AGhosts():Super() {
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("Root")));

	SFX = CreateDefaultSubobject<UCLSounder>(TEXT("SFX"));
	SFX->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Env/Ghost/Ghosts_S.Ghosts_S"));
	SFX->SetSound(CSnd.Object);
	SFX->TimeFadeIn = 1;
	SFX->TimeFadeOut = 2.0;
	SFX->TimeStartMin = 0;
	SFX->TimeStartMax = 120;
	static ConstructorHelpers::FObjectFinder<USoundAttenuation>
		CAttn(LDConsts::Audio::Attns::Env);
	SFX->AttenuationSettings = CAttn.Object; // they need to play loudly and clear.
	
	Parts = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Parts"));
	Parts->SetupAttachment(RootComponent);
	Parts->SetAutoActivate(false);
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem>
		CNiagara(TEXT("/Game/LifeDev/Game/Env/Ghost/Ghost_NS.Ghost_NS"));
	Parts->SetAsset(CNiagara.Object);
}

void AGhosts::SetPlaying(const bool IsPlaying) {
	SFX->Fade(IsPlaying);
	Parts->SetActive(true);
}
