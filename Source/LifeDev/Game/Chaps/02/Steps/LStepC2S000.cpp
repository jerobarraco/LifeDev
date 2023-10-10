// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC2S000.h"

ALStepC2S000::ALStepC2S000():Super() {
	Name = FName("C2S0");
	static FText ST = FText::FromString("~ To be continued ... ~");
	Title = ST;
	UseFade = true;
	// DlgId = FName("C2S0"); // this chapter finishes after the dialog
	InputEnabled = true;
	UsePawnCam = true;
	UseFadeTime = true;
	FinishPostWait = false;
	// TODO un/load data layers 
}
