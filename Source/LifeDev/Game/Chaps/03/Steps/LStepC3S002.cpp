// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC3S002.h"

#include "LifeDev/Core/Sounds/LMusicMan.h"
#include "LifeDev/Game/Sys/Consts/ConstItems.h"

ALStepC3S002::ALStepC3S002():Super() {
	Name = FName("C3S2");
	UseFade = false;
	InputEnabled = true;
	UsePawnCam = true;
	FinishPostWait = false;

	ItemsFinish = {
		LDConsts::Items::Card3,
		LDConsts::Items::Poem3,
	}; // TODO
}

void ALStepC3S002::Start_Implementation() {
	Super::Start_Implementation();
	ALMusicMan::SetRainS(GetWorld(), true);
}

void ALStepC3S002::Stop_Implementation() {
	Super::Stop_Implementation();
	ALMusicMan::SetRainS(GetWorld(), false);
}
