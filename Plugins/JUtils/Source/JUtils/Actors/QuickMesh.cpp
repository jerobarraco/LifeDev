// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: LGPL-3.0-only

#include "QuickMesh.h"

#include "CQuickMesh.h"

AQuickMesh::AQuickMesh(): Super() {
	Mesh = CreateDefaultSubobject<UCQuickMesh>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	
	Super::SetActorTickEnabled(false);
	PrimaryActorTick.SetTickFunctionEnable(false);
	PrimaryActorTick.bStartWithTickEnabled = false;
}
