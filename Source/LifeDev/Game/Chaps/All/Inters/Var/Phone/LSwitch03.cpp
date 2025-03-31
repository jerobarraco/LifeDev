// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LSwitch03.h"

#include "CQuickMesh.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"

ALSwitch03::ALSwitch03(): Super() {
	StateNum = 1;
	Anim->IsAdditive = true;
	
	const FVector Loc(5,-7.500000,12.500000);
	
	// Anim->TStart = FTransform(State0Rot, Loc, FVector(1));
	Anim->TStart.SetLocation(Loc);
	Anim->TEnd.SetLocation(FVector(0, 0, -10));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh (TEXT("/Game/LifeDev/Game/Inters/Lights/Switch00/btn2"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(-7.5,7.5,-15));

	Interact->SetRelativeLocation(FVector(7.500000,-7.5,15));
	Interact->SetBoxExtent(FVector(5)); // luckily with this button size it doesn't protrude that much

	SetStateNow(0);
}
