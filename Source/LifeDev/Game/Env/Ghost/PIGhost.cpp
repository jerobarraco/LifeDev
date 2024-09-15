// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "PIGhost.h"

#include "CGhostAxis.h"
#include "CQuickMesh.h"

APIGhost::APIGhost():Super() {
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<UCQuickMesh>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CMesh(
		TEXT("/Game/LifeDev/Game/Env/Ghost/Ghost-03"));
	Mesh->SetStaticMesh(CMesh.Object);
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> CMat(
		TEXT("/Game/LifeDev/Game/Env/Ghost/Ghost_PDMI.Ghost_PDMI"));
	Mesh->SetMaterial(0, CMat.Object);
	
	AxisX = CreateDefaultSubobject<UCGhostAxis>(TEXT("AxisX"));
	AxisY = CreateDefaultSubobject<UCGhostAxis>(TEXT("AxisY"));
	AxisZ = CreateDefaultSubobject<UCGhostAxis>(TEXT("AxisZ"));
}

void APIGhost::PostDuplicate(bool Pie) {
	Super::PostDuplicate(Pie);
	AxisX->RenameComp("X");
	AxisY->RenameComp("Y");
	AxisZ->RenameComp("Z");
}
