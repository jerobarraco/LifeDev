// Copyright (C) 2023 Jeronimo Barraco-Marmol


#include "LPController.h"

#include "Inventory/Teach/TPInput.h"


void ALPController::BeginPlay() {
	Super::BeginPlay();
	OverridePlayerInputClass = UTPInput::StaticClass();
}
