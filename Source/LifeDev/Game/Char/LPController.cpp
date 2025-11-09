// Copyright (C) 2023 Jeronimo Barraco-Marmol


#include "LPController.h"

#include "Inventory/Teach/TPInput.h"

ALPController::ALPController() {
	OverridePlayerInputClass = UTPInput::StaticClass();
}
