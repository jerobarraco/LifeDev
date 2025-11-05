// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol

#include "Toy00.h"

#include "CQuickMesh.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorFade.h"

AToy00::AToy00() {
	Texts = {
		NSLOCTEXT("Ppl", "State0", "..."), // closed
	};
	RewardFlash = 0;
	UseRewardDestroy = false;
	StateNum = 1;
	UseAnim = false;
	UseFade = false; // by default skip it
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Var/Toy01_P"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetUseDynShadow(true);
	Mesh->SetRelativeLocation(FVector(-10,2.225,0));

	AnimFade->MatBase = nullptr; // the mesh uses a palette. force using the mesh's material.

	Interact->SetRelativeLocation(FVector(5.000000,-2.225000,1.250000));
	Interact->SetBoxExtent(FVector(5.000000,2.225000,1.250000));
}
