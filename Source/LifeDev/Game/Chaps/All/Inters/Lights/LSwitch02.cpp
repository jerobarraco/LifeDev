// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LSwitch02.h"

#include "CQuickMesh.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"

ALSwitch02::ALSwitch02(): Super() {
	StateNum = 2;
	const FVector Loc(5,-7.500000,12.500000);
	const FRotator State0Rot(10,0.,0);
	const FRotator State1Rot(-10,0, 0);
	Anim->IsAdditive = false;
	// only twa states no need to use Trans
	Anim->TStart = FTransform(State0Rot, Loc, FVector(1));
	Anim->TEnd = FTransform(State1Rot, Loc, FVector(1));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh (TEXT("/Game/LifeDev/Game/Inters/Lights/Switch00/btn2"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(-7.5,10,-15));

	Interact->SetRelativeLocation(FVector(7.500000,-7.5,15));
	Interact->SetBoxExtent(FVector(5)); // luckily with this button size it doesn't protrude that much

	SetStateNow(1);
}
