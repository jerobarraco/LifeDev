// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LSwitch01.h"

#include "CQuickMesh.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"

ALSwitch01::ALSwitch01(): Super() {
	StateNum = 4; // 4 is the max. could be clamped on children
	Texts = {
		NSLOCTEXT("Switch01", "Turn0", "Turn"),
		NSLOCTEXT("Switch01", "Turn1", "Turn again"),
		NSLOCTEXT("Switch01", "Turn2", "Turn once more"),
		NSLOCTEXT("Switch01", "Turn3", "Turn yet again"),
	};
	const FVector Loc(0.000000,-7.500000,12.500000);
	const FRotator State0Rot(0,0.,0);
	const FRotator State1Rot(0,0, -90);
	const FRotator State2Rot(0,0,-180);
	const FRotator State3Rot(0,0,-270);
	Trans = {
		FTransform(State0Rot, Loc),
		FTransform(State1Rot, Loc),
		FTransform(State2Rot, Loc),
		FTransform(State3Rot, Loc),
	};
	// IRoot->SetRelativeTransform(Trans[0]);
	Anim->IsAdditive = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh (TEXT("/Game/LifeDev/Game/Inters/Lights/Switch00/btn3"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(0.000000,7.500000,-12.500000));

	Interact->SetRelativeLocation(FVector(7.500000,-10.000000,12.500000));
	Interact->SetBoxExtent(FVector(5)); // left like this to avoid issues due to overflow
	SetStateNow(1);
}
