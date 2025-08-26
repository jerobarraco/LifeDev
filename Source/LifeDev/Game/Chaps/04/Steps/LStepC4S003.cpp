// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC4S003.h"

ALStepC4S003::ALStepC4S003():Super() {
	Label = FName("C4S3");
	// DlgId = Name; // this chapter finishes after the dialog
	InputEnabled = false;
	UsePawnCam = false;
	// go a bit above to ensure we end up at 1 a couple of dialogs before
	FBDlgAutoTo = 1.2;
	UseFBDlgAuto = true;
	TeleportChar = true;
	IntersTrigger = {
		TSoftObjectPtr<AInteract>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.CharNRG_UAID_D8BBC116E50193F001_1688570496"))
	};
}
