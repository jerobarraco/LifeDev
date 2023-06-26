// Copyright Jerónimo Barraco-Mármol

#include "Card.h"

#include "Interact/CInteract.h"

ACard::ACard():Super() {
	Interact->SetRelativeLocation(FVector(5.000000,-2.500000,0.000000));
	Interact->SetBoxExtent(FVector(10.000000,5.000000,5.000000));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Card00/Card00.Card00"));
	Mesh->SetStaticMesh(CMesh.Object);
}
