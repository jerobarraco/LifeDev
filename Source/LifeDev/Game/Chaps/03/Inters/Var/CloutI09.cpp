// Copyright Jerónimo Barraco-Mármol

#include "CloutI09.h"

#include "CQuickMesh.h"
#include "Interact/Animator/CAnimatorFade.h"

ACloutI09::ACloutI09():Super() {
	// RewardItem = "CloutI04";
	// LockedDlg = "CL04_L";
	IsLocked = true;
	UseAnim = false;
	UseFade = true;

	const ConstructorHelpers::FObjectFinder<UMaterialInterface>
		CMat(TEXT("/Game/LifeDev/Game/Var/Mats/Voxel/Palettes/Palette00_DMI"));
	if(LIKELY(CMat.Succeeded())) Mesh->SetMaterial(0, CMat.Object.Get());
	AnimFade->MatBase = nullptr;
}
