// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LNPC.h"

ALNPC::ALNPC():Super() {
	PrimaryActorTick.bStartWithTickEnabled = false;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->SetComponentTickEnabled(false);
}
