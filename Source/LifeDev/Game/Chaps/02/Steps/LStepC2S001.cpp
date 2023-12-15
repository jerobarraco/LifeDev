// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC2S001.h"

#include "Diags/Diags.h"
#include "LifeDev/Core/Sounds/LMusicMan.h"
#include "LifeDev/Game/Chaps/All/Env/Ghosts.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Game/Interact/LInteract.h"

ALStepC2S001::ALStepC2S001():Super() {
	static const FName N("C2S1");
	Name = N;
	DlgId = N; // this chapter finishes after the dialog
	UseFade = false;
	InputEnabled = false;
	UsePawnCam = false;
	UseFadeTime = false;
	FinishPostWait = false;
	TeleportChar = false;
}

void ALStepC2S001::BeginPlay() {
	Super::BeginPlay();
	if (IsValid(FakeChar)) {
		FakeChar->SetActorHiddenInGame(true);
	}
}

void ALStepC2S001::Start_Implementation() {
	UWorld* const W = GetWorld();
	if (!W) return;

	// make the fb raise progressively with the dialogs
	FB->SetMax(1);
	FB->SetMin(0);
	// Flashback->SetVal(1, 10);
	// bind before the super since it will trigger the dialogs probably
	Dialogs->OnShow.AddUniqueDynamic(this, &ALStepC2S001::ShowDlg);
	
	Super::Start_Implementation();
	
	ALMusicMan::SetRainS(W, true);

	Ghosts = Cast<AGhosts>(W->SpawnActor(AGhosts::StaticClass()));
	if (IsValid(Ghosts)) {
		Ghosts->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
		Ghosts->SetActorRelativeLocation(GhostPos);
		Ghosts->SetPlaying(true);
	}

	if (IsValid(FakeChar)) {
		FakeChar->SetActorHiddenInGame(false);
		FakeChar->Fade(true);
	}
}

void ALStepC2S001::Stop_Implementation() {
	UWorld* const W = GetWorld();
	if (!W) return;

	Dialogs->OnShow.RemoveAll(this);
	if (IsValid(Ghosts)) {
		Ghosts->SetPlaying(false);
		// Destroy them during the fade
		FTimerHandle H;
		W->GetTimerManager().SetTimer(H, this, &ALStepC2S001::DestroyActors, WaitTime);
	}

	if (IsValid(FakeChar)) {
		FakeChar->Fade(false);
	}

	ALMusicMan::SetRainS(W, false);
	FB->SetVal(.05, 10);

	Super::Stop_Implementation();
}

void ALStepC2S001::DestroyActors() {
	// this is a bit dangerous, we can't go back to chap 0 without reloading.
	// but also more performant.

	if (IsValid(FakeChar)) {
		FakeChar->Destroy();
	}
	FakeChar = nullptr;

	if (IsValid(Ghosts)) {
		Ghosts->Destroy();
	}
	Ghosts = nullptr;
	FB = nullptr;
}

void ALStepC2S001::ShowDlg(const FDialog& Diag) {
	FB->ModVal(.1);
}
