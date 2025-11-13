// Copyright Jerónimo Barraco-Mármol

#include "ScrewerI00.h"

#include "CQuickMesh.h"
#include "Interact/Animator/CAnimatorFade.h"

AScrewerI00::AScrewerI00():Super() {
	RewardItem = "Screwer00";
	UseAutoActivate = true;
	UseHint = true;
	HintCondition = "{Inter.Locked.DoorI10}"; // hint after triggered
	UseFade = true;
	AnimFade->MatBase = nullptr; // the mesh uses a palette. force using the mesh's material.

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		ObjMat(TEXT("/Game/LifeDev/Game/Var/Mats/Voxel/Palettes/Mask/Palette01_MMI"));
	if(LIKELY(ObjMat.Succeeded())) Mesh->SetMaterial(0, ObjMat.Object);
	
	AScrewerI00::SetMobility(EComponentMobility::Type::Static);
}
