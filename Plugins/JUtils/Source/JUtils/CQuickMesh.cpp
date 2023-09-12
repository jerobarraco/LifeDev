// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#include "CQuickMesh.h"

UCQuickMesh::UCQuickMesh(): Super() {
	Super::SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Super::SetCollisionProfileName("NoCollision");
	Super::SetGenerateOverlapEvents(false);
	Super::SetCanEverAffectNavigation(false);
	Super::CanCharacterStepUpOn = ECB_No;
	bNavigationRelevant = false;
	bCanEverAffectNavigation = false;

	Super::SetComponentTickEnabled(false);
	PrimaryComponentTick.SetTickFunctionEnable(false);
	PrimaryComponentTick.bStartWithTickEnabled = false;
	SetCastAllShadows(false);
}

void UCQuickMesh::SetCastAllShadows(bool Cast) {
	SetCastShadow(Cast);
	bCastDynamicShadow = Cast;
	// by default this is false, i don't wanna mess with it. so just don't.
	// bCastDistanceFieldIndirectShadow = Cast; //?? is this too much of a micro optimization?
}
