// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: GPL-2.0
#include "Step.h"

#include "Camera/CameraActor.h"

void AStep::Start_Implementation() {
	if (IsValid(Camera)) {
		GetWorld()->GetFirstPlayerController()->SetViewTargetWithBlend(Camera, 1, VTBlend_EaseInOut);
	}
}

void AStep::Stop_Implementation() {}
