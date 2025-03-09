// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LSwitch01.h"

#include "CQuickMesh.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"

ALSwitch01::ALSwitch01(): Super() {
	
	Anim->IsAdditive = true;
	Anim->TEnd.SetLocation(FVector(-2.5,0,0));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh (TEXT("/Game/LifeDev/Game/Inters/Lights/Switch00/btn3"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(0,0,0));

	Interact->SetRelativeLocation(FVector(7.500000,-12.500000,17.500000));
	Interact->SetBoxExtent(FVector(5));
}
