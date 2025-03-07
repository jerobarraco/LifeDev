// Copyright Jerónimo Barraco-Mármol

#include "Bottle01.h"

#include "CQuickMesh.h"
#include "Interact/Animator/CAnimatorFade.h"

ABottle01::ABottle01():Super() {
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		CMat(TEXT("/Game/LifeDev/Game/Var/Mats/Voxel/Palettes/Palette07_DMI"));
	Mesh->SetMaterial(0, CMat.Object);
	
	AnimFade->MatBase = nullptr;
}
