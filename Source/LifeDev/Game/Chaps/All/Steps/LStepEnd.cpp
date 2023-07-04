// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepEnd.h"

#include "Kismet/GameplayStatics.h"

ALStepEnd::ALStepEnd():Super() {
	Name = FName("End");
	InputEnabled = false;
	// don't wait for fades
	UseFadeTime = false;
	// don't finish, we will change levels. and better not to mess with this.
	FinishPostWait = false;
	// don't change the camera
	UsePawnCam = false;
	CamTarget = nullptr;
}

void ALStepEnd::Start_Implementation() {
	Super::Start_Implementation();
	
	UGameplayStatics::OpenLevel(GetWorld(), FName(*NextLevel), true);
}
