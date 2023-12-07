// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepEnd.h"

#include "Kismet/GameplayStatics.h"

ALStepEnd::ALStepEnd():Super() {
	Name = FName("End");
	static FText ST = FText::FromString("~ To be continued ... ~");
	Title = ST;
	InputEnabled = false;
	UseFadeTime = true;
	FinishPostWait = false; // avoid crash
	UseFade = true;
	// don't change the camera
	UsePawnCam = true;
	CamTarget = nullptr;
}

void ALStepEnd::Start_Implementation() {
	Super::Start_Implementation();
	UGameplayStatics::OpenLevel(GetWorld(), FName(*NextLevel), true);
	
}

void ALStepEnd::Stop_Implementation() {
	Super::Stop_Implementation();
	
}

void ALStepEnd::PostWait_Implementation() {
	Super::PostWait_Implementation();
}
