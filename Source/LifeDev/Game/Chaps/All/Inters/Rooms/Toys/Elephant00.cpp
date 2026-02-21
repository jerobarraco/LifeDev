// Copyright (C) 2023-2025 Jeronimo Barraco-Marmol

#include "Elephant00.h"

#include "CQuickMesh.h"
#include "Interact/CInteract.h"

AElephant00::AElephant00(): Super() {
	Texts = {
		NSLOCTEXT("Ppl", "State0", "A toy elephant"), // closed
	};
	RewardFlash = 0;
	UseRewardDestroy = false;
	StateNum = 1;
	UseAnim = false;
	UseFade = false; // by default skip it
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Rooms/Toys/Elephant00"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetUseDynShadow(true);
	Mesh->SetRelativeLocation(FVector(-4.,7.,0.));

	Interact->SetRelativeLocation(FVector(4.,-7., 6.));
	Interact->SetBoxExtent(FVector(3.,8.,6.));
	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSFX(TEXT("/Game/LifeDev/Game/Inters/Rooms/Toy/Toy_C"));
	SFXs = { CSFX.Object, CSFX.Object};

	Super::SetMobility(EComponentMobility::Static);
}

