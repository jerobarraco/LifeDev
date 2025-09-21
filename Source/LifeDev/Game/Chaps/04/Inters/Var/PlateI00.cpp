// Copyright Jerónimo Barraco-Mármol

#include "PlateI00.h"

#include "CQuickMesh.h"
#include "Interact/CInteract.h"
#include "Interact/Animator/CAnimatorFade.h"

APlateI00::APlateI00():Super() {
	UseFade = true;
	UseAutoActivate = false;
	SetActorHiddenInGame(true);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		CMesh(TEXT("/Game/LifeDev/Game/Inters/Kitchen/Plate/Plate00_P02"));
	Mesh->SetStaticMesh(CMesh.Object);
	Interact->SetRelativeLocation(FVector(2.500000,-2.500000,10));
	Interact->SetBoxExtent(FVector(20,20,10));

	// force reloading the material, since the base Plate removes it for the palettes.
	// but this mesh doesn't use palettes, and i don't want to set the material to voxel fade (in case i use it statically)
	// i can't change the base Plate since it's used for other plates that need to fade and use palettes.
	// remember kids, each exception/special case will multiply the amount of work.
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		CMatBaseNew(TEXT("/Game/LifeDev/Game/Var/Mats/Voxel/VoxelFade_DMI.VoxelFade_DMI"));
	if (LIKELY(CMatBaseNew.Succeeded()))
		AnimFade->MatBase = CMatBaseNew.Object;
}
