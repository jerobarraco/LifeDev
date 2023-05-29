// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: GPL-2.0
#include "Step.h"

#include "Camera/CameraActor.h"

void AStep::Start_Implementation() {
	UE_LOG(LogTemp, Log, TEXT("Starting step '%s'"), *Name.ToString());

	if (IsValid(Camera)) {
		GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(Camera, 1, VTBlend_EaseInOut);
	}
}

void AStep::Stop_Implementation() {
	UE_LOG(LogTemp, Log, TEXT("Stopping step '%s'"), *Name.ToString());
}
