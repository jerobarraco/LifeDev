// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LSwitch03.h"

#include "CQuickMesh.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"

ALSwitch03::ALSwitch03(): Super() {
	StateNum = 1;
	
	const FVector Loc(-7.500000,32.500000,-77.500000);
	
	// Anim->TStart = FTransform(State0Rot, Loc, FVector(1));
	Anim->TStart.SetLocation(Loc);
	Anim->TEnd.SetLocation(FVector(0, 0, 2.5));
	// Anim->IsBouncing = true;
	Anim->IsAdditive = true;
	Anim->Duration = .4;
	static ConstructorHelpers::FObjectFinder<UCurveFloat>
		CCF(TEXT("/JUtils/Curves/PulseOut"));
	Anim->Curve = CCF.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Bath/Toilet02/Btn2")); // already grey
	Mesh->SetStaticMesh(CMesh.Object);
	// Mesh->SetRelativeLocation(FVector(-7.5,7.5,-15));

	Interact->SetRelativeLocation(FVector(7.500000,-32.500000,77.500000));
	Interact->SetBoxExtent(FVector(2.5)); // luckily with this button size it doesn't protrude that much

	SetStateNow(0);
}
