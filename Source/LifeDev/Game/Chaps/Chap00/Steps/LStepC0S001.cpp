// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStepC0S001.h"

ALStepC0S001::ALStepC0S001():Super() {
	Name = FName("C0S1");
	InputEnabled = true;
	IsPawnTarget = true;
}

void ALStepC0S001::Start_Implementation() {
	Super::Start_Implementation();
}
