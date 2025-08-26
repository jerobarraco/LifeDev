// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#include "LStepC4S009.h"

#include "LifeDev/Game/Flashback/Flashback.h"

ALStepC4S009::ALStepC4S009():Super() {
	Label = FName("C4S9");
	// DlgId = Name; // this chapter finishes after the dialog
	InputEnabled = false;
	UsePawnCam = false;
	TeleportChar = false;
	CamBlendTime = 2; // waittime will be forced to this
	UseFBDlgAuto = true;
	FBDlgAutoTo = -.05;
	ActorsShow = {
		TSoftObjectPtr<AActor>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.PaperI00_UAID_D8BBC116E50101EF01_1328482739")),
		TSoftObjectPtr<AActor>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.Pencil_UAID_D8BBC116E5014CEF01_1096293939"))};
	IntersTrigger = {TSoftObjectPtr<AInteract>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.CharNRG_UAID_D8BBC116E50193F001_1481028493"))};
}

void ALStepC4S009::Start_Implementation() {
	Super::Start_Implementation();
	// for this we want to really reach 0 since it's the very end (for dramatic purposes, not realism). 
	if (LIKELY(FB)) FB->SetMin(0);
}
