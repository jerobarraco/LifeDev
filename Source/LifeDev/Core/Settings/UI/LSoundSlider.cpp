// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LSoundSlider.h"

#include "Inventory/Flags.h"
#include "Sound/SoundSubmix.h"

void ULSoundSlider::Apply() {
	const float Value = GetValue();
	if (!IsValid(Submix)) {
		UE_LOG(LogTemp, Warning, TEXT("%hs Submix not set. Key=%s"),
			__func__, *Key.ToString());
		return;
	}

	Submix->SetSubmixOutputVolume(this, Value);
}

void ULSoundSlider::Save() {
	Apply(); // unnecessary but just in case.
	
	UFlags* const Flags = UFlags::Instance(this);
	if (!IsValid(Flags)) return;

	const float Value = GetValue();
	UE_LOG(LogTemp, Log, TEXT("%hs val=%.5f"),
		__func__, Value);
	if (Key.IsNone()) {
		UE_LOG(LogTemp, Warning, TEXT("%hs Key not set."),
		__func__);
		return;
	}

	Flags->Set(Key, Value);
}
