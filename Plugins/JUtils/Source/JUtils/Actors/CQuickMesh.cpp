// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#include "CQuickMesh.h"

void UCQuickMesh::SetQuickCollisionEnabled(bool Enable) {
	Super::SetCanEverAffectNavigation(Enable);
	bNavigationRelevant = Enable;
	if (Enable) {
		Super::SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Super::SetCollisionProfileName("BlockAllDynamic");
	} else {
		Super::SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Super::SetCollisionProfileName("NoCollision");
		Super::SetGenerateOverlapEvents(false);
		Super::CanCharacterStepUpOn = ECB_No;
	}
}

UCQuickMesh::UCQuickMesh(): Super() {
	SetQuickCollisionEnabled(false);

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
