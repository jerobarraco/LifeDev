// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LStep0000.h"

ALStep0000::ALStep0000():Super() {
	Name = FName("C0S0");
	DisablePlayer = true;
}

void ALStep0000::Start_Implementation() {
	Super::Start_Implementation();
}

void ALStep0000::Stop_Implementation() {
	Super::Stop_Implementation();
	// TODO trigger next story step
}
