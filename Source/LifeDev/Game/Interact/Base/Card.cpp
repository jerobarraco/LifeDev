// Copyright Jerónimo Barraco-Mármol

#include "Card.h"

#include "Interact/CInteract.h"

ACard::ACard():Super() {

	Interact->SetRelativeLocation(FVector(-60.039127,-7.825052,100.782019));
	Interact->SetBoxExtent(FVector(60.000000,10.000000,103.743262));
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Chaps/All/Inters/Door00/Door00.Door00"));
	Mesh->SetStaticMesh(CMesh.Object);
}
