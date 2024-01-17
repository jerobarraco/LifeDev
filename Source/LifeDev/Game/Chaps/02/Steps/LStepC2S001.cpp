// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC2S001.h"

#include "Camera/CameraComponent.h"
#include "Diags/Diags.h"
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

	Cam->SetConstraintAspectRatio(true);
	Cam->AspectRatio = 2;
}

void ALStepC2S001::BeginPlay() {
	Super::BeginPlay();
	if (IsValid(FakeChar)) {
		FakeChar->SetActorHiddenInGame(true);
		FakeChar->SetEnabled(false);
	}
}

void ALStepC2S001::Start_Implementation() {
	UWorld* const W = GetWorld();
	if (!W) return;

	// make the fb raise progressively with the dialogs
	FB->SetMax(1);
	// bind before the super since it will trigger the Diags
	Diags->OnShow.AddUniqueDynamic(this, &ALStepC2S001::ShowDlg);
	
	Super::Start_Implementation();
	
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
	if (Diags) {
		Diags->OnShow.RemoveAll(this);
	}

	if (FB) {
		// FB->SetVal(.05, 10);
		
		// clamp till the end of the story, important for the randomizer in c2s4
		// .33 is the min to hear the melody
		FB->SetMin(.33,1);
	}
	
	if (IsValid(Ghosts)) {
		Ghosts->SetPlaying(false);
	} // gets nullified on destroyactors

	if (IsValid(FakeChar)) {
		FakeChar->Fade(false);
	}
	
	Super::Stop_Implementation();

	UWorld* const W = GetWorld();
	if (!W) return;
	// Destroy them during the fade
	FTimerHandle H;
	W->GetTimerManager().SetTimer(H, this, &ALStepC2S001::DestroyActors, 2);
}

void ALStepC2S001::DestroyActors() {
	UE_LOG(LogTemp, Log, TEXT("Destroy actors called"));
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
	// there are 11 dialogs, and the fb should be on .4 at least.
	// with .08 we ensure that we reach 1. but with .1 is too much
	// .075 a couple of diags remain
	FB->ModVal(.07);
}
