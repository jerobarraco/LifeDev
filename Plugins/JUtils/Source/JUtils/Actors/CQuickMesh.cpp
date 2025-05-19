// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "CQuickMesh.h"

UCQuickMesh::UCQuickMesh(): Super() {
	Super::SetComponentTickEnabled(false);
	PrimaryComponentTick.SetTickFunctionEnable(false);
	PrimaryComponentTick.bStartWithTickEnabled = false;

	// attempt to optimize nanite shader complexity
	// works only with r.OptimizeWPO 1
	bEvaluateWorldPositionOffset = false;
	bEvaluateWorldPositionOffsetInRayTracing = false;
	WorldPositionOffsetDisableDistance = 1;
	bWorldPositionOffsetWritesVelocity = false;
	bCastDistanceFieldIndirectShadow = true; // TODO is this a perf improv or not?

	SetQuickCollisionEnabled(false);
	SetCastAllShadows(false);
	// ShadowCacheInvalidationBehavior = // needs to be set on a case by case basis :/ 
}

void UCQuickMesh::SetQuickCollisionEnabled(const bool Enable) {
	Super::SetCanEverAffectNavigation(Enable);
	bNavigationRelevant = Enable;
	if (Enable) {
		Super::SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Super::SetCollisionProfileName("BlockAllDynamic");
	} else {
		Super::SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Super::SetCollisionProfileName("NoCollision");
		Super::SetGenerateOverlapEvents(false);
		Super::CanCharacterStepUpOn = ECB_No;
	}
}


void UCQuickMesh::SetCastAllShadows(const bool Cast) {
	SetCastShadow(Cast);
	bCastDynamicShadow = Cast;
	// by default this is false, i don't want to mess with it. so just don't.
	// bCastDistanceFieldIndirectShadow = Cast; //?? is this too much of a micro optimization?
}

void UCQuickMesh::SetCustomDepth(const bool Enable, const int32 Val) {
	// use alongside JUtils/Mats/Post/Hidden
	CustomDepthStencilValue = Val;
	bRenderCustomDepth = Enable;
	SetRenderInDepthPass(!Enable);
	SetRenderInMainPass(!Enable);
}
