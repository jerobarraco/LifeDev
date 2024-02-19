// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC3S001.h"

#include "Camera/CameraComponent.h"
#include "Diags/Diags.h"

#include "LifeDev/Game/Chaps/All/Env/Ghosts.h"
#include "LifeDev/Game/Flashback/Flashback.h"
#include "LifeDev/Game/Interact/LInteract.h"

ALStepC3S001::ALStepC3S001():Super() {
	Name = FName("C3S1");
	UseFade = false;
	DlgId = FName("C3S1"); // this chapter finishes after the dialog
	InputEnabled = false;
	UsePawnCam = false;
	FinishPostWait = false;

	
	Cam->SetConstraintAspectRatio(true);
	Cam->AspectRatio = 2;

	GhostPos = FVector(210,-42,-65);;
	UseGhosts = true;
	// TODO raise the fb with each dialog
}

void ALStepC3S001::BeginPlay() {
	Super::BeginPlay();
	
	if (IsValid(FakeChar)) {
		FakeChar->SetActorHiddenInGame(true);
		FakeChar->SetEnabled(false);
	}
}

void ALStepC3S001::Start_Implementation() {
	UWorld* const W = GetWorld();
	if (!W) return;

	// make the fb raise progressively with the dialogs
	FB->SetMax(1);
		
	constexpr int32 numDlgs = 4; //TODO??
	FbDiagMod = (1.0-FB->GetVal()) / (numDlgs-1);
	
	// bind before the super since it will trigger the Diags
	Diags->OnShow.AddUniqueDynamic(this, &ALStepC3S001::ShowDlg);
	
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

void ALStepC3S001::Stop_Implementation() {
	if (Diags) Diags->OnShow.RemoveAll(this);
	if (FB) {
		// FB->SetVal(.05, 10);
		FB->SetMin(.33,1); // TODO
	}
	
	if (IsValid(Ghosts)) Ghosts->SetPlaying(false);
	// gets nullified on destroyactors

	if (IsValid(FakeChar)) FakeChar->Fade(false);
	
	Super::Stop_Implementation();

	UWorld* const W = GetWorld();
	if (!W) return;
	// Destroy them during the fade
	FTimerHandle H;
	W->GetTimerManager().SetTimer(H, this, &ALStepC3S001::DestroyActors, 2);
}

void ALStepC3S001::ShowDlg(const FDialog& Diag) {
	// FB->ModVal(FbInc);
}
