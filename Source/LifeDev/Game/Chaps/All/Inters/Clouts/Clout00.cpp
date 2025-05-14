// Copyright Jerónimo Barraco-Mármol

#include "Clout00.h"

#include "Interact/CInteract.h"
#include "JUtils/Actors/CQuickMesh.h"

AClout00::AClout00():Super() {
	RewardItem = TEXT("C1C00"); // chapter 1 clout 0

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		ObjMesh(TEXT("/Game/LifeDev/Game/Inters/Clouts/Shirt00.Shirt00"));
	if (ObjMesh.Succeeded())
		Mesh->SetStaticMesh(ObjMesh.Object);
	Mesh->SetRelativeLocation(FVector(-17.500000,17.500000,-0.000000));
	Interact->SetRelativeLocation(FVector(17.500000,-17.500000,2.500000));
	Interact->SetBoxExtent(FVector(18.000000,18.000000,3.000000));
}
