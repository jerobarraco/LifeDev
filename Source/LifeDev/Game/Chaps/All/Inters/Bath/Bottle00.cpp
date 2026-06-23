// Copyright Jerónimo Barraco-Mármol

#include "Bottle00.h"

#include "UObject/ConstructorHelpers.h"

#include "CQuickMesh.h"
#include "Interact/CInteract.h"

ABottle00::ABottle00() {
	StateNum = 1;
	Texts = {
		NSLOCTEXT("Bottle00", "Text0", "Shampoo")
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
}
