// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC0S000.h"

#include "Dialogs/Dialogs.h"
#include "Interact/CInteract.h"
#include "Interact/Interact.h"

ALStepC0S000::ALStepC0S000():Super() {
	Name = FName("C0S0");
	SeqId = FName("Intro");
	InputEnabled = false;
	UseCam = true;
}

void ALStepC0S000::Start_Implementation() {
	Super::Start_Implementation();
	if (IsValid(FakeChar)) {
		FakeChar->SetActorHiddenInGame(false);
	}
}

void ALStepC0S000::Stop_Implementation() {
	Super::Stop_Implementation();
	if (IsValid(FakeChar)) {
		// FakeChar->SetActorHiddenInGame(true);
		FakeChar->Destroy();
	}
}

void ALStepC0S000::BeginPlay() {
	Super::BeginPlay();
	AInteract* const FakeInter = Cast<AInteract> (FakeChar);
	if (FakeInter) {
		FakeInter->SetEnabled(false);
	}
}
