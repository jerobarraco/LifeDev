// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC2S000.h"

#include "WorldPartition/DataLayer/DataLayerAsset.h"

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

	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDL1 (TEXT("/Game/LifeDev/Game/Chaps/All/DataLayers/Chap02_DL.Chap02_DL"));
	if (CDL1.Succeeded()) {
		DL_Load.Add(CDL1.Object);
	}
	
	static ConstructorHelpers::FObjectFinder<UDataLayerAsset>
		CDL2 (TEXT("/Game/LifeDev/Game/Chaps/All/DataLayers/Chap01_DL.Chap01_DL"));
	if (CDL2.Succeeded()) {
		DL_Unload.Add(CDL2.Object);
	}
}
