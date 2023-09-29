// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC0S001.h"

#include "Interact/Interact.h"
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

	UFlashback* const Flashback = UFlashback::Get(GetWorld());
	if (!Flashback) return;
	Flashback->SetMax(1);
	Flashback->SetVal(1, 10);
	// TODO add timer here and teleport the player
}

void ALStepC0S001::Stop_Implementation() {
	Super::Stop_Implementation();
	if (IsValid(FakeChar)) {
		// FakeChar->SetActorHiddenInGame(true);
		FakeChar->Destroy();
	}
	UFlashback::SetValS(GetWorld(), 0, .5);
}

void ALStepC0S001::BeginPlay() {
	Super::BeginPlay();
	AInteract* const FakeInter = Cast<AInteract> (FakeChar);
	if (FakeInter) {
		FakeInter->SetEnabled(false);
	}
}
