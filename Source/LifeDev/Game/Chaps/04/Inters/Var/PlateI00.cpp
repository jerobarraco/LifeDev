// Copyright Jerónimo Barraco-Mármol

#include "PlateI00.h"

#include "CQuickMesh.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorFade.h"

APlateI00::APlateI00():Super() {
	UseFade = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Plate/Plate00_P02"));
	Mesh->SetStaticMesh(CMesh.Object);
	Interact->SetRelativeLocation(FVector(2.500000,-2.500000,10));
	Interact->SetBoxExtent(FVector(20,20,10));

	AnimFade->SetNewMat(); // won't fade without it, this mesh is not palette
}
