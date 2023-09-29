// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC0S001.h"

#include "Interact/Interact.h"
#include "LifeDev/Game/Chaps/All/Env/Ghosts.h"
#include "LifeDev/Game/Chaps/All/Env/SRain.h"
#include "LifeDev/Game/Flashback/Flashback.h"

ALStepC0S001::ALStepC0S001():Super() {
	Name = FName("C0S1");
	DlgId = FName("Intro");
	InputEnabled = false;
	UseFadeTime = true;
	UseFade = true;
	FinishPostWait = false;
}

void ALStepC0S001::Start_Implementation() {
	Super::Start_Implementation();
	if (IsValid(FakeChar)) {
		FakeChar->SetActorHiddenInGame(false);
	}

	UWorld* const W = GetWorld();
	UFlashback* const Flashback = UFlashback::Get(W);
	if (!Flashback) return;
	Flashback->SetMax(1);
	Flashback->SetVal(1, 10);

	ASRain::SSetPlaying(W, true);

	Ghosts = Cast<AGhosts>(W->SpawnActor(AGhosts::StaticClass()));
	if (IsValid(Ghosts)) {
		Ghosts->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
		Ghosts->SetActorRelativeLocation(GhostLocation);
		Ghosts->SetPlaying(true);
	}
	// TODO add timer here and teleport the player
}

void ALStepC0S001::Stop_Implementation() {
	Super::Stop_Implementation();
	if (IsValid(FakeChar)) {
		FakeChar->Destroy(); // FakeChar->SetActorHiddenInGame(true);
	}
	if (IsValid(Ghosts)) {
		Ghosts->SetPlaying(false);
		// not destroying here to allow ghosts to de-spawn naturally
	}

	UWorld* const W = GetWorld();
	ASRain::SSetPlaying(W, false);
	UFlashback::SetValS(W, 0);
}

void ALStepC0S001::BeginPlay() {
	Super::BeginPlay();
	AInteract* const FakeInter = Cast<AInteract>(FakeChar);
	if (FakeInter) {
		FakeInter->SetEnabled(false);
	}
}
