// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol


#include "TPInput.h"

bool UTPInput::InputKey(const FInputKeyEventArgs& Params) {
	// https://forums.unrealengine.com/t/enhanced-input-detect-gamepad-vs-keyboard-input/1231533/15?u=nande
	// TODO use a new type instead of bool, for the device type. search in case ue has one already. and replace teachtarget

	const bool IsGP = Params.IsGamepad();
	// Params.bIsTouchEvent
	// Params.Key.IsGamepadKey() // the key also has a bunch of these. Params.IsGamePad returns the value on the key
	// this doesn't seem very optimal, but it's the only way that seems to work
	// ultimately it will do this under the hood.
	UE_LOG(LogTemp, VeryVerbose, TEXT("UTPlayerInput::%hs isGamePad=%i"), __func__, IsGP);
	if (UNLIKELY(WasGP != IsGP)) {
		UE_LOG(LogTemp, VeryVerbose, TEXT("UTPlayerInput::%hs Changed Was=%i Is=%i"), __func__, WasGP, IsGP);
		WasGP = IsGP;
		OnChange.Broadcast(IsGP);
	}

	return Super::InputKey(Params);
}
