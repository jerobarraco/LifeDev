// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC4S003.h"

ALStepC4S003::ALStepC4S003():Super() {
	Name = FName("C4S3");
	DlgId = Name; // this chapter finishes after the dialog
	UseFade = false;
	InputEnabled = false;
	UsePawnCam = false;
	WaitTime = CamBlendTime = 2;
	// set wait to blend so that FIRST you turn and THEN it fades the chars (on DoStart (after wait))
	FinishPostWait = false;
	TeleportChar = false;
}
