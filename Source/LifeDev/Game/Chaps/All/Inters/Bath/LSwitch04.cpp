// Copyright Jerónimo Barraco-Mármol

#include "LSwitch04.h"

#include "CQuickMesh.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorMix.h"

ALSwitch04::ALSwitch04():Super() {
	StateNum = 1;
	
	const FVector Loc(-7.500000,32.500000,-77.500000);
	Mesh->SetRelativeLocation(Loc);
	Anim->TEnd.SetLocation(FVector(0, 0, -2.5));
	// Anim->IsBouncing = true; //uses the curve
	Anim->IsAdditive = true;
	Anim->Duration = .4;
	Anim->IsReversed = false; // curve is reversed
	static ConstructorHelpers::FObjectFinder<UCurveFloat>
		CCF(TEXT("/Niagara/DefaultAssets/Curves/Templates/RampUpDown"));
	Anim->Curve = CCF.Object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Bath/Toilet02/Btn")); // already grey
	Mesh->SetStaticMesh(CMesh.Object);

	Interact->SetRelativeLocation(FVector(7.500000,-32.500000,77.500000));
	Interact->SetBoxExtent(FVector(2.5)); // luckily with this button size it doesn't protrude that much

}
