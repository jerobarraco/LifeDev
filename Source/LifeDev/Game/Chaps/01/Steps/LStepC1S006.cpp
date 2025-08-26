// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#include "LStepC1S006.h"

ALStepC1S006::ALStepC1S006():Super() {
	Label = FName("C1S6");
	InputEnabled = true; // does it matter? it will show dialogs
	// uses pawn camera
	CamTarget = this;
	UsePawnCam = false;
	// DlgId = "N01.1";
	FBDlgAutoTo = .5;
	UseFBDlgAuto = true;
	UseFBAnim = true;
	UseRain = true;
	TeleportChar=true;
	ActorsHide = {
		TSoftObjectPtr<AActor>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.LNPC01_UAID_D8BBC116E501119001_1693349215"))
	};
}
