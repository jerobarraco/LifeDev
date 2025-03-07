// Copyright Jerónimo Barraco-Mármol

#include "Bottle00.h"

#include "CQuickMesh.h"

ABottle00::ABottle00() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Bath/Bottle01_P"));
	Mesh->SetStaticMesh(CMesh.Object);
	StateNum = 1;
	Texts = {
		NSLOCTEXT("Bottle00", "Text0", "Shampoo bottle")
	};
}
