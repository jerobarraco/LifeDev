// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.

#include "LVolumeSlider.h"

#include "Inventory/Flags.h"
#include "Sound/SoundSubmix.h"

ULVolumeSlider::ULVolumeSlider() {
	SetStepSize(.1);
	SetValue(1.0);// default to 1
}

void ULVolumeSlider::Preview() {
	const float Val = GetValue(); // "Value" is a private member
	UE_LOG(LogTemp, Verbose, TEXT("%hs val=%.5f"),
		__func__, Val);

	if (!IsValid(Submix)) {
		UE_LOG(LogTemp, Warning, TEXT("%hs Submix not set. Key=%s"),
			__func__, *Key.ToString());
		return;
	}

	Submix->SetSubmixOutputVolume(this, Val);
}

void ULVolumeSlider::Apply_Implementation() {
	Super::Apply_Implementation();
	UFlags* const Flags = UFlags::Instance(this);
	if (!IsValid(Flags)) return;

	const float Val = GetValue(); // "Value" is private
	// logging before calling apply just so that logs are in order
	UE_LOG(LogTemp, Log, TEXT("%hs val=%.5f"),
		__func__, Val);

	Preview(); // unnecessary but just in case.
	
	if (Key.IsNone()) {
		UE_LOG(LogTemp, Warning, TEXT("%hs Key not set."),
		__func__);
		return;
	}

	Flags->Set(Key, Val);
}

void ULVolumeSlider::Load_Implementation() {
	Super::Load_Implementation();
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
	Preview();
}

void ULVolumeSlider::PostInitProperties() {
	Super::PostInitProperties();
	OnValueChanged.AddUniqueDynamic(this, &ULVolumeSlider::ValChanged);
}
