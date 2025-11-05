// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol


#include "TPInput.h"

bool UTPInput::InputKey(const FInputKeyEventArgs& Params) {
	// https://forums.unrealengine.com/t/enhanced-input-detect-gamepad-vs-keyboard-input/1231533/15?u=nande
	const bool IsGP = Params.IsGamepad();
	// this is unfortunate but it's the only way that seems to work
	UE_LOG(LogTemp, VeryVerbose, TEXT("UTPlayerInput::%hs isGamePad=%i"), __func__, IsGP);
	if (UNLIKELY(IsGP != WasGP)) {
		OnChange.Broadcast(IsGP);
	}

	return Super::InputKey(Params);
}
