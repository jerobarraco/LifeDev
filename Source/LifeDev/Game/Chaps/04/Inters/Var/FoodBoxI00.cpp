// Copyright Jerónimo Barraco-Mármol

#include "FoodBoxI00.h"

#include "Interact/Animator/CAnimatorFade.h"
#include "JUtils/Actors/CQuickMesh.h"

AFoodBoxI00::AFoodBoxI00():Super() {
	Texts = { NSLOCTEXT("FoodBoxI00", "State0", "Rice box") };
	StateNum = 1;
	UseAutoActivate = true;
	UseFade = true;
	UseHint = true;
	RewardItem = "Food00";
	UseRewardDestroy = true;
	HintCondition = "{Inter.Locked.PotI00}";
	// TriggerDlg = "Food00_T";
	// RewardFlash = .1; // story works better this way
	// Locked = true;
	// LockedDlg = "Food00_L";

	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		CMat(TEXT("/Game/LifeDev/Game/Var/Mats/Voxel/Palettes/Mask/Palette06_MMI"));
	Mesh->SetMaterial(0, CMat.Object);
	AnimFade->MatBase = CMat.Object;
}
