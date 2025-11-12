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
	bCanEverAffectNavigation = false;
	// https://forums.unrealengine.com/t/nanite-pixel-programmable-distance-for-ism-component-huge-performance-wins/2278017/8
	// 600 is about the size of a room
	NanitePixelProgrammableDistance = 600;
	SetQuickCollisionEnabled(false);

	
	// i personally don't really like how contact shadows behave and are set. so i'm disabling them by default.
	// https://dev.epicgames.com/documentation/en-us/unreal-engine/contact-shadows-in-unreal-engine
	// https://www.youtube.com/live/nm1slxtF_qA?t=1867
	bCastContactShadow = false;
	SetUseDynShadow(false);
	// disable distance field shadows. as they break fading. but also they have to be enabled per light.
	// if the light is distance fields, it _only_ uses distance fields, otherwise it _only_ uses dynamic shadows. so i can NOT mix objects.
	// thereby, i can only rely on dynamic shadows. 
	// by default use distance fields for lighting. if cast shadows is off, it won't be used.
	// this is necessary to be true, or it won't show the shadow on distance field mode
	bAffectDistanceFieldLighting = false;
	bCastDistanceFieldIndirectShadow = false; // depends on mesh being movable, having distance fields generated, and project supporting it.
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

void UCQuickMesh::SetUseDynShadow(const bool Cast) {
	SetCastShadow(Cast);
	bCastDynamicShadow = Cast;
	bCastContactShadow = !Cast; // TODO what's the impact of casting both.
}

void UCQuickMesh::SetUseContactShadow(const bool Contact) {
	bCastContactShadow = Contact;
	bCastDynamicShadow = !Contact;
}

void UCQuickMesh::SetCustomDepth(const bool Enable, const int32 Val) {
	// use alongside JUtils/Mats/Post/Hidden
	CustomDepthStencilValue = Val;
	bRenderCustomDepth = Enable;
	SetRenderInDepthPass(!Enable);
	SetRenderInMainPass(!Enable);
}
