// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC0S000.h"

ALStepC0S000::ALStepC0S000():Super() {
	Name = FName("C0S0");
	InputEnabled = true;
}

void ALStepC0S000::Start_Implementation() {
	Super::Start_Implementation();
	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(Handle, this, &ALStepC0S000::Finish, 4);
}

void ALStepC0S000::Stop_Implementation() {
	Super::Stop_Implementation();
	// TODO trigger next story step
}
