// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.

#include "LStepC1S004.h"

#include "LifeDev/Game/Flashback/Flashback.h"

ALStepC1S004::ALStepC1S004():Super() {
	Name = FName("C1S4");
	InputEnabled = true;
	// uses own camera
	UsePawnCam = true;
	UseFadeTime = false;
	// wait for the player to talk with the npc and get the card
	FinishItems = {"C1"};
}

void ALStepC1S004::Stop_Implementation() {
	Super::Stop_Implementation();
	GetWorld()->GetSubsystem<UFlashback>()->SetVal(0);
}

