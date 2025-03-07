// Copyright Jerónimo Barraco-Mármol

#include "Bottle01.h"

#include "CQuickMesh.h"

ABottle01::ABottle01() {
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		CMat(TEXT("/Game/LifeDev/Game/Var/Mats/Voxel/Palettes/Palette07_DMI"));
	Mesh->SetMaterial(0, CMat.Object);
}
