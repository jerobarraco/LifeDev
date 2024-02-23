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
	};
}

void ALStepC3S002::Start_Implementation() {
	Super::Start_Implementation();
	ALMusicMan::SetRainS(GetWorld(), true);
}

void ALStepC3S002::Stop_Implementation() {
	// this is the card. using this so i don't have to code the fade in
    // setting to null to avoid getting it destroyed by the step
    // though technically unreal will nullify this for me,
    // and i always check for IsValid instead of null
    // once the player obtains the interact and gets destroyed;
    // there's a lot of chance of problems due to timing issues.
    // specially since this step finishes with Card3 obtained.
    Actor = nullptr;

	Super::Stop_Implementation();
	ALMusicMan::SetRainS(GetWorld(), false);
}
