// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#include "QuickMesh.h"

#include "CQuickMesh.h"

AQuickMesh::AQuickMesh(): Super() {
	Mesh = CreateDefaultSubobject<UCQuickMesh>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	
	// TODO why did i disabled this? can i re-enable it?
	// TODO re-enable next time i do the "optimization" pass
	//		TODO need to re-ensure every object that needs to move sets it to movable
	//		TODO Make a function SetCollision (bool)
	// Super::SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// Super::SetCollisionProfileName("NoCollision");
	// Super::SetGenerateOverlapEvents(false);
	// Super::SetCanEverAffectNavigation(false);
	// Super::CanCharacterStepUpOn = ECB_No;
	// bNavigationRelevant = false;
	// bCanEverAffectNavigation = false;

	Super::SetActorTickEnabled(false);
	PrimaryActorTick.SetTickFunctionEnable(false);
	PrimaryActorTick.bStartWithTickEnabled = false;
}
