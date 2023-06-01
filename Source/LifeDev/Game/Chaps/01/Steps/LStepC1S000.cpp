// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC1S000.h"

ALStepC1S000::ALStepC1S000():Super() {
	Name = FName("C1S0");
	InputEnabled = true;
	IsPawnTarget = true;
}

void ALStepC1S000::Start_Implementation() {
	Super::Start_Implementation();
	// FTimerHandle TimerHandle;
	// GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ALStepC1S000::Finish, 2);
}
