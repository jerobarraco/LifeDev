// Copyright Jerónimo Barraco-Mármol

#include "Clout03.h"

#include "JUtils/Actors/CQuickMesh.h"

#include "Interact/CInteract.h"

AClout03::AClout03():Super() {
	Texts = { NSLOCTEXT("Clout03", "State0", "A bra") }; // used on the bra in ch01, on both.
	// one of them is not meant to be picked

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		ObjMesh(TEXT("/Game/LifeDev/Game/Inters/Clouts/Bra00.Bra00"));
	if (LIKELY(ObjMesh.Succeeded())) Mesh->SetStaticMesh(ObjMesh.Object);

	Mesh->SetRelativeLocation(FVector(67.500000,-37.500000,0.000000));
	Interact->SetRelativeLocation(FVector(-67.500000,37.500000,2.500000));
	Interact->SetBoxExtent(FVector(15.000000,45.000000,3.000000));
}
