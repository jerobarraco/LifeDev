// Copyright Jerónimo Barraco-Mármol

#include "PencilI00.h"

#include "Interact/Animator/CAnimatorFade.h"
#include "JUtils/Actors/CQuickMesh.h"

APencilI00::APencilI00():Super() {
	UseRewardDestroy = false;
	UseFade = true;
	SetActorHiddenInGame(true);
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		CMat(TEXT("/Game/LifeDev/Game/Var/Mats/Voxel/Palettes/Palette01_DMI"));
	Mesh->SetMaterial(0, CMat.Object);
	AnimFade->MatBase = CMat.Object;
}
