// Copyright (C) 2023 - Jeronimo Barraco-Marmol. All rights reserved.
// SPDX-License-Identifier: MIT

#include "QuickMesh.h"

#include "CQuickMesh.h"

AQuickMesh::AQuickMesh() {
	Mesh = CreateDefaultSubobject<UCQuickMesh>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetCastContactShadow(true);

	Super::SetActorTickEnabled(false);
	PrimaryActorTick.SetTickFunctionEnable(false);
	PrimaryActorTick.bStartWithTickEnabled = false;
}
