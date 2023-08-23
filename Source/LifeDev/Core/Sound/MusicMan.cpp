// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "MusicMan.h"

#include "Components/AudioComponent.h"
#include "LifeDev/Game/Flashback/Flashback.h"


AMusicMan::AMusicMan():Super() {
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.SetTickFunctionEnable(false);

	Player = CreateDefaultSubobject<UAudioComponent>(TEXT("Player"));
	Player->bAutoManageAttachment=true;
	Player->SetAutoActivate(false);
}

void AMusicMan::BeginPlay() {
	Super::BeginPlay();
	if (!Enabled) return;

	UFlashback* const Flashback = UFlashback::Get(GetWorld());
	if (!Flashback) return;
	
	SetIntensity(0);
	Flashback->OnChange.AddUniqueDynamic(this, &AMusicMan::SetIntensity);
	Player->Activate(true);
}

void AMusicMan::SetIntensity(float V) {
	static FName NInt ="Intensity";
	Player->SetFloatParameter(NInt, V);
}

