// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#include "LStepC1S003.h"

#include "LifeDev/Game/Chaps/All/NPCs/LNPC01.h"

ALStepC1S003::ALStepC1S003():Super() {
	Name = FName("C1S3");
	DlgId = "C1S3";
	InputEnabled = false;
	// uses own camera
	UsePawnCam = false;
	CamBlendTime = 3;
	Root->SetWorldLocation(FVector(-91.291630,813.867677,156.699670));
	Root->SetWorldRotation(FRotator(-29.498704,91.508393,-5.725105));
	// TODo specify the intersfadein here
}

void ALStepC1S003::BeginPlay() {
	Super::BeginPlay();

	DoIntersFade(IntersFadeIn, false); // force npc faded
	
	if (UNLIKELY(IntersFadeIn.Num()<=0)) {
		UE_LOG(LogTemp, Warning, TEXT("NPC not assigned to ALStep c1s3"));
		return;
	}

	const ALNPC01* const N = Cast<ALNPC01>(IntersFadeIn[0].Get());
	if (LIKELY(N)) N->SetPoseSit();
	else UE_LOG(LogTemp, Warning, TEXT("NPC not assigned to ALStep c1s3"));
}
