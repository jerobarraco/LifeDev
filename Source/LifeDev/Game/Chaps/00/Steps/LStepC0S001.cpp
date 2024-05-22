// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC0S001.h"

#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

#include "LifeDev/Core/Sounds/LMusicMan.h"
#include "LifeDev/Game/Char/LChar.h"
#include "LifeDev/Game/Flashback/Flashback.h"

ALStepC0S001::ALStepC0S001():Super() {
	Name = FName("C0S1");
	DlgId = FName("Intro");
	InputEnabled = false;
	UseFadeTime = true;
	UseFade = true; // important since the door, also due to the load
	FinishPostWait = false;
	TeleportChar = true;
	ItemsRem = {"T01"};
	Cam->SetConstraintAspectRatio(true);
	Cam->SetAspectRatio(2);
	CamBlendTime = 0; // doesn't get along with fade
	GhostPos = FVector(210,-42,-65);
	UseGhosts = true;
	UseRain = true;
}

void ALStepC0S001::Start_Implementation() {
	Super::Start_Implementation();
	FB->SetMax(1);
	FB->SetVal(1, 10);
}

void ALStepC0S001::Stop_Implementation() {
	FB->SetVal(0);
	Super::Stop_Implementation();
}
