// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC0S000.h"

ALStepC0S000::ALStepC0S000():Super() {
	Name = FName("C0S0");
	DisablePlayer = true;
}

void ALStepC0S000::Start_Implementation() {
	Super::Start_Implementation();
}

void ALStepC0S000::Stop_Implementation() {
	Super::Stop_Implementation();
	// TODO trigger next story step
}
