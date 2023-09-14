// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#include "QuickMesh.h"

#include "CQuickMesh.h"

AQuickMesh::AQuickMesh(): Super() {
	Mesh = CreateDefaultSubobject<UCQuickMesh>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	// Super::SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// Super::SetCollisionProfileName("NoCollision");
	// Super::SetGenerateOverlapEvents(false);
	// Super::SetCanEverAffectNavigation(false);
	// Super::CanCharacterStepUpOn = ECB_No;
	// bNavigationRelevant = false;
	// bCanEverAffectNavigation = false;
	//
	Super::SetActorTickEnabled(false);
	
	PrimaryActorTick.SetTickFunctionEnable(false);
	PrimaryActorTick.bStartWithTickEnabled = false;
}
