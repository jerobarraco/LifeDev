// Copyright Jerónimo Barraco-Mármol

#include "SpotI04.h"

#include "CQuickMesh.h"
#include "Interact/CInteract.h"

ASpotI04::ASpotI04() {
	UseAutoActivate = true;
	UnlockItems = {"Bottle00", "Bottle01"};
	Texts = { NSLOCTEXT("SpotI04", "Text0", "Drop bottle") };
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Plants/Pot00"));
	Mesh->SetStaticMesh(CMesh.Object);
	Mesh->SetRelativeScale3D(FVector(.2));
	Mesh->SetRelativeLocation(FVector(-6.500000,6.000000,-2.500000));
	Interact->SetRelativeScale3D(FVector(5));
	Interact->SetRelativeLocation(FVector(32.500000,-30.000000,30));
	Interact->SetBoxExtent(FVector(10));
}

// rewards actor is set on editor