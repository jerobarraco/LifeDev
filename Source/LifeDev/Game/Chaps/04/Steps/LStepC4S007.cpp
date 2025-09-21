// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC4S007.h"

ALStepC4S007::ALStepC4S007():Super() {
	Label = FName("C4S7");
	// DlgId = Name; // this chapter finishes after the dialog
	InputEnabled = false;
	UsePawnCam = false;
	TeleportChar = false;
	UseFBDlgAuto = true;
	FBDlgAutoTo = 1.2;
	ActorsHide = {TSoftObjectPtr<AActor>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.LNPC04_UAID_D8BBC116E501A1E401_1207918287"))};
}
