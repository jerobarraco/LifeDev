// Copyright (C) 2023 Jeronimo Barraco-Marmol


#include "LPController.h"

#include "Inventory/Teach/TPlayerInput.h"


void ALPController::BeginPlay() {
	Super::BeginPlay();
	OverridePlayerInputClass = UTPlayerInput::StaticClass();
}
