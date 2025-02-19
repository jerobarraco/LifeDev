// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#include "LStepC1S004.h"

#include "LifeDev/Core/Consts/ConstItems.h"

ALStepC1S004::ALStepC1S004():Super() {
	Name = FName("C1S4");
	InputEnabled = true;
	// uses pawn camera
	CamTarget = nullptr;
	UsePawnCam = true;
	UseFadeTime = false;
	// wait for the player to talk with the npc and get the card
	ItemsFinish = {
		LDConsts::Items::Card1,
		LDConsts::Items::Poem1,
	};

	// the clothes are transient items that don't need to be carried
	// the rest of the items are going to be used or required
	ItemsRem = {"C1C00", "C1C01", "C1C02", "C1C03", "C1C04"}; 
}
