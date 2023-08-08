// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#include "CQuickMesh.h"

UCQuickMesh::UCQuickMesh(): Super() {
	Super::SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Super::SetGenerateOverlapEvents(false);
	Super::SetCanEverAffectNavigation(false);
	Super::CanCharacterStepUpOn = ECB_No;
	// todo disable physics
	PrimaryComponentTick.SetTickFunctionEnable(false);
	PrimaryComponentTick.bStartWithTickEnabled = false;
	Super::SetComponentTickEnabled(false);
}
