// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#include "LStepC1S003.h"

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

	CharClass = ALNPC01::StaticClass(); 	
}

void ALStepC1S003::Start_Implementation() {
	Super::Start_Implementation();
	NPCSpawn();
}

void ALStepC1S003::NPCDestroy() {
	if (IsValid(Char)) {
		Char->Destroy();
	}
	Char = nullptr;
}

void ALStepC1S003::Stop_Implementation() {
	Super::Stop_Implementation();
}

void ALStepC1S003::NPCSpawn() {
	if (!IsValid(CharClass)) {
		NPCDiagStart();
		return;
	}

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	UWorld* const World = GetWorld();
	Char = World->SpawnActor(CharClass, &CharT, Params);

	FTimerHandle Handle;
	World->GetTimerManager().SetTimer(Handle, this, &ALStepC1S003::NPCDiagStart, CamBlendTime);
}

void ALStepC1S003::NPCDiagStart() {
	Dialogs->OnDone.AddUniqueDynamic(this, &ALStepC1S003::NPCDiagStop);
	Dialogs->AddId("C1S3.0");
}

void ALStepC1S003::NPCDiagStop() {
	Dialogs->OnDone.RemoveAll(this);
	Finish();
}
