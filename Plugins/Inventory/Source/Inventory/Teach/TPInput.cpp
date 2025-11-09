// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol

#include "TPInput.h"

bool UTPInput::InputKey(const FInputKeyEventArgs& Params) {
	// https://forums.unrealengine.com/t/enhanced-input-detect-gamepad-vs-keyboard-input/1231533/15?u=nande
	const EInputType NewTgt = UJUtilsInput::GetKeyType(Params.Key);
	if (Tgt != NewTgt) {
		UE_LOG(LogTemp, VeryVerbose, TEXT("UTPlayerInput::%hs Changed Was=%s Is=%s"), __func__,
			*UEnum::GetValueAsString(Tgt), *UEnum::GetValueAsString(NewTgt));
		Tgt = NewTgt;
		OnChange.Broadcast(Tgt);
	}

	return Super::InputKey(Params);
}
