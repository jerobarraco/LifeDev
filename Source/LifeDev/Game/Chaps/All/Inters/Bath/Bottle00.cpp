// Copyright Jerónimo Barraco-Mármol

#include "Bottle00.h"

#include "CQuickMesh.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorFade.h"

ABottle00::ABottle00() {
	StateNum = 1;
	// TODO set position
	Texts = {
		NSLOCTEXT("Bottle00", "Text0", "Shampoo bottle")
	};
	UseFade = false;
	UseAnim = false;
	UseRewardDestroy = false;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Bath/Bottle01_P"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeLocation(FVector(77.500000,-75.000000,0.000000));
	Interact->SetRelativeLocation(FVector(-77.500000,75.000000,15.000000));
	Interact->SetBoxExtent(FVector(10.000000,10.000000,20.000000));
	AnimFade->MatBase = nullptr; // force using the current material
}
