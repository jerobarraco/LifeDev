// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC0S001.h"

ALStepC0S001::ALStepC0S001():Super() {
	Name = FName("C0S1");
	InputEnabled = false;
	UsePawnCam = false;
	UseCam = false;
}

void ALStepC0S001::Start_Implementation() {
	Super::Start_Implementation();
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ALStepC0S001::Finish, 2);
}
