// Copyright (C) 2023 Jeronimo Barraco-Marmol

#include "LPController.h"

#include "JUtils/Input/JPInput.h"

ALPController::ALPController() {
	OverridePlayerInputClass = UJPInput::StaticClass();
}
