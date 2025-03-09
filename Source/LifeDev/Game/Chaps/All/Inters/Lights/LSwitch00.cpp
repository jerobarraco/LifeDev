// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LSwitch00.h"

#include "CQuickMesh.h"
#include "Interact/Animator/CAnimatorFade.h"

ALSwitch00::ALSwitch00(): Super() {
	Base = CreateDefaultSubobject<UCQuickMesh>(TEXT("Base"));
	Base->SetupAttachment(Root);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh (TEXT("/Game/LifeDev/Game/Inters/Lights/Switch00/base"));
	Base->SetStaticMesh(CMesh.Object);
	Base->SetRelativeLocation(FVector(0,0,0));
	AnimFade->Meshes.Add(Base); // unused but...
}
