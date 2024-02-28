// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC3S002.h"

#include "LifeDev/Core/Sounds/LMusicMan.h"
#include "LifeDev/Game/Flashback/CRandomizerFB.h"
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
	
	// will decrease the fb. done in this step since this step could potentially last a while until they find the
	// required items, and it's hard to find them with a fb
	RndFB = CreateDefaultSubobject<UCRandomizerFB>(TEXT("RndFB"));
	RndFB->SetAutoActivate(false);
	RndFB->DelayMin = .5;
	RndFB->DelayMax = 2;
	RndFB->ValueMin = -.07;
	RndFB->ValueMax = .02; // you wouldn't think is so easy to get out of a flashback, do you?
}

void ALStepC3S002::Start_Implementation() {
	Super::Start_Implementation();
	ALMusicMan::SetRainS(GetWorld(), true);
	FB->SetMin(0);
	RndFB->Activate(true);
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

	RndFB->Deactivate();
	FB->SetMin(0);
	FB->SetMax(1);
	FB->SetVal(0);
	ALMusicMan::SetRainS(GetWorld(), false);
	Super::Stop_Implementation();
}
