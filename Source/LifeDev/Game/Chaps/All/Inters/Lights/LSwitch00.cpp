// Copyright (c) 2023 Jeronimo Barraco-Marmol. All rights reserved.
#include "LSwitch00.h"

#include "CQuickMesh.h"

#include "Interact/Animator/CAnimatorFade.h"
#include "Interact/Animator/CAnimatorMix.h"

ALSwitch00::ALSwitch00(): Super() {
	UseAnim = true;
	Anim->IsAdditive = true; // important specially since the setstatenow will set the transform and TEnd might be wrong
	Anim->Duration = .25;
	Anim->TStart = FTransform::Identity;
	Anim->TEnd.SetLocation(FVector(-2.5,0,0));

	Base = CreateDefaultSubobject<UCQuickMesh>(TEXT("Base"));
	Base->SetupAttachment(Root);
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CBase(TEXT("/Game/LifeDev/Game/Inters/Lights/Switch00/base"));
	Base->SetStaticMesh(CBase.Object);
	Base->SetRelativeLocation(FVector(0,0,0));
	AnimFade->Meshes.Add(Base); // unused but...

	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Generic/Wall_Light_Double_Switch_Off-004"));
	SFXs = {CSnd.Object, CSnd.Object};

	SetStateNow(1); // start on. most lights start on.
}
