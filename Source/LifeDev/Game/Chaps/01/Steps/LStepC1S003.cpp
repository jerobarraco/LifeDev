// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#include "LStepC1S003.h"

#include "Diags/Diags.h"

#include "LifeDev/Game/Chaps/All/NPCs/LNPC01.h"

ALStepC1S003::ALStepC1S003():Super() {
	Name = FName("C1S3");
	InputEnabled = false;
	// uses own camera
	UsePawnCam = false;
	UseFadeTime = false;
	CamBlendTime = 3;

	Root->SetWorldLocation(FVector(-91.291630,813.867677,156.699670));
	Root->SetWorldRotation(FRotator(-29.498704,91.508393,-5.725105));
}

void ALStepC1S003::BeginPlay() {
	Super::BeginPlay();
	if (NPC) NPC->SetVisible(false);
}

void ALStepC1S003::Start_Implementation() {
	Super::Start_Implementation();
	NPCShow();
}

void ALStepC1S003::Stop_Implementation() {
	Super::Stop_Implementation();
}

void ALStepC1S003::NPCShow() {
	if (!IsValid(NPC)) {
		NPCDiagStart();
		return;
	}

	NPC->SetVisible(true);
	UWorld* const World = GetWorld();
	FTimerHandle Handle;
	World->GetTimerManager().SetTimer(Handle, this, &ALStepC1S003::NPCDiagStart, CamBlendTime);
}

void ALStepC1S003::NPCDiagStart() {
	Diags->OnDone.AddUniqueDynamic(this, &ALStepC1S003::NPCDiagStop);
	Diags->AddId("C1S3.0");
}

void ALStepC1S003::NPCDiagStop() {
	Diags->OnDone.RemoveAll(this);
	Finish();
}
