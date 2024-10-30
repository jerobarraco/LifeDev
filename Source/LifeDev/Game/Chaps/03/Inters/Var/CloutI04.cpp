// Copyright Jerónimo Barraco-Mármol

#include "CloutI04.h"

#include "CQuickMesh.h"
#include "Interact/Animator/CAnimatorFade.h"
#include "JUtils/Misc/JMiscConsts.h"

ACloutI04::ACloutI04():Super() {
	RewardItem = "CloutI04";
	LockedDlg = "CL03_L";
	Locked = true;
	UseAnim = false;
	UseFade = false;

	const ConstructorHelpers::FObjectFinder<UMaterialInterface>
		CMat(TEXT("/Game/LifeDev/Game/Var/Mats/Voxel/Palettes/Palette00_DMI"));
	IFL(CMat.Succeeded()) Mesh->SetMaterial(0, CMat.Object.Get());
	AnimFade->MatBase = nullptr;
}
