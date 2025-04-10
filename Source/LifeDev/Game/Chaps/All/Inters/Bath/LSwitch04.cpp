// Copyright Jerónimo Barraco-Mármol

#include "LSwitch04.h"

#include "CQuickMesh.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"

ALSwitch04::ALSwitch04():Super() {
	StateNum = 1;
	Texts = { NSLOCTEXT("Toilet", "State0", "Push") };

	// SFXs = {}; // TODO need new audio
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Bath/Toilet02/Btn1")); // already grey
	Mesh->SetStaticMesh(CMesh.Object);
	const FVector Loc(-7.500000,25.0,-77.500000);
	Mesh->SetRelativeLocation(Loc);

	Anim->CopyTStart();
	Anim->TEnd.SetLocation(FVector(0, 0, -2.5));
	Anim->IsAdditive = true;
	Anim->Duration = .6;
	Anim->IsReversed = false; // curve is reversed
	static ConstructorHelpers::FObjectFinder<UCurveFloat>
		CCF(TEXT("/Niagara/DefaultAssets/Curves/Templates/RampUpDown"));
	Anim->Curve = CCF.Object;

	Interact->SetRelativeLocation(FVector(7.500000,-25,77.500000));
	Interact->SetBoxExtent(FVector(2.5, 5, 2.5)); // luckily with this button size it doesn't protrude that much
}
