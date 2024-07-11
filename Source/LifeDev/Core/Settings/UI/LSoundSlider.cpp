// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LSoundSlider.h"

#include "Inventory/Flags.h"
#include "Sound/SoundSubmix.h"

void ULSoundSlider::Apply() {
	const float Value = GetValue();
	UE_LOG(LogTemp, Log, TEXT("%hs val=%.5f"),
		__func__, Value);

	if (!IsValid(Submix)) {
		UE_LOG(LogTemp, Warning, TEXT("%hs Submix not set. Key=%s"),
			__func__, *Key.ToString());
		return;
	}

	Submix->SetSubmixOutputVolume(this, Value);
}

void ULSoundSlider::Save() {
	UFlags* const Flags = UFlags::Instance(this);
	if (!IsValid(Flags)) return;

	const float Value = GetValue();
	// logging before calling apply just so that logs are in order
	UE_LOG(LogTemp, Log, TEXT("%hs val=%.5f"),
		__func__, Value);

	Apply(); // unnecessary but just in case.
	
	if (Key.IsNone()) {
		UE_LOG(LogTemp, Warning, TEXT("%hs Key not set."),
		__func__);
		return;
	}

	Flags->Set(Key, Value);
}

void ULSoundSlider::Load() {
	const UFlags* const Flags = UFlags::Instance(this);
	if (!IsValid(Flags)) return;

	if (Key.IsNone()) {
		UE_LOG(LogTemp, Warning, TEXT("%hs Key not set. Stop"),
		__func__);
		return;
	}

	// note default to 1, so if not set, it's full value.
	const float Val = Flags->Get(Key, 1);
	SetValue(Val);

	// this is a patch to ensure the
	// submix is set to the loaded value on load
	// notice this will get called when the dialog is shown.
	// the 1st time it will re-set the values to their current value.
	// which is ok, and it's good if it's different from the savegame.
	Apply();
}
