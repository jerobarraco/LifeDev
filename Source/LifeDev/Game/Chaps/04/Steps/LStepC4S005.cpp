// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC4S005.h"

ALStepC4S005::ALStepC4S005():Super() {
	Label = FName("C4S5");
	// DlgId = Name; // this chapter finishes after the dialog
	InputEnabled = false;
	UsePawnCam = false;
	TeleportChar = true;
	
	UseFBDlgAuto = true;
	FBDlgAutoTo = 1.2;

	ActorsHide = {TSoftObjectPtr<AActor>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.LNPC03_UAID_D8BBC116E501A1E401_1174907286"))};
}
