// Copyright Jerónimo Barraco-Mármol

#include "SpotI04.h"

#include "CQuickMesh.h"

ASpotI04::ASpotI04() {
	UseOrder = false;
	UseStateInc = false;
	Items = {"Bottle00", "Bottle01"};
	Texts = {
		NSLOCTEXT("SpotI04", "Text0", "Drop bottle")
	};
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Plants/Pot00"));
	Mesh->SetStaticMesh(CMesh.Object);
}

// rewards actor is set on editor