// Copyright Jerónimo Barraco-Mármol

#include "Clout04.h"

#include "Interact/CInteract.h"

#include "JUtils/Actors/CQuickMesh.h"

AClout04::AClout04():Super() {
	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		ObjMesh(TEXT("/Game/LifeDev/Game/Inters/Clouts/Sock00.Sock00"));
	if (LIKELY(ObjMesh.Succeeded()))
		Mesh->SetStaticMesh(ObjMesh.Object);
	Mesh->SetRelativeLocation(FVector(-10.000000,10.000000,-2.500000));
	Interact->SetRelativeLocation(FVector(10.000000,-10.000000,2.500000));
	Interact->SetBoxExtent(FVector(10.000000,10.000000,2.500000));
}
