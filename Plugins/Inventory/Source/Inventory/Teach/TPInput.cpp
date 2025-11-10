// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol

#include "TPInput.h"

bool UTPInput::InputKey(const FInputKeyEventArgs& Params) {
	// https://forums.unrealengine.com/t/enhanced-input-detect-gamepad-vs-keyboard-input/1231533/15?u=nande
	const EInputType NewTgt = UJUtilsInput::GetKeyType(Params.Key);
	if (LastType != NewTgt) {
		UE_LOG(LogTemp, VeryVerbose, TEXT("UTPlayerInput::%hs Changed Was=%s Is=%s"), __func__,
			*UEnum::GetValueAsString(LastType), *UEnum::GetValueAsString(NewTgt));
		LastType = NewTgt;
		OnChange.Broadcast(LastType);
	}

	return Super::InputKey(Params);
}
