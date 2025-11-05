// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol


#include "TPlayerInput.h"

bool UTPlayerInput::InputKey(const FInputKeyEventArgs& Params) {
	UE_LOG(LogTemp, Log, TEXT("UTPlayerInput::%hs isGamePad=%i"), __func__, Params.IsGamepad());
	return Super::InputKey(Params);
}
