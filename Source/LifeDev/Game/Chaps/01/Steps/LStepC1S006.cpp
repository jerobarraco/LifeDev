// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#include "LStepC1S006.h"

#include "LifeDev/Core/Consts/ConstItems.h"

ALStepC1S006::ALStepC1S006():Super() {
	Name = FName("C1S6");
	InputEnabled = true; // does it matters? it will show dialogs
	// uses pawn camera
	CamTarget = nullptr;
	UsePawnCam = true;
	UseFadeTime = false;
	DlgId = "N01.1";
	FBDlgAutoTo = .5;
	UseRain = true;

	// wait for the player to talk with the npc and get the card
	ItemsFinish = {
		LDConsts::Items::Card1,
				LDConsts::Items::Poem1,
}
;

	// the clothes are transient items that don't need to be carried
	// the rest of the items are going to be used or required
	ItemsRem = {"C1C00", "C1C01", "C1C02", "C1C03", "C1C04"}; 
}
