// Copyright Jerónimo Barraco-Mármol

#include "FoodBoxI00.h"

#include "Interact/Animator/CAnimatorFade.h"
#include "JUtils/Actors/CQuickMesh.h"

AFoodBoxI00::AFoodBoxI00():Super() {
	Texts = { FText::FromString(TEXT("Rice box"))};
	RewardItem = "Food00";
	// RewardFlash = .1;
	UseRewardDestroy = true;
	UseFade = true;
	// Locked = true;
	// LockedDlg = "Food00_L";
	TriggerDlg = "Food00_T";

	static ConstructorHelpers::FObjectFinder<USoundBase>
		CSnd(TEXT("/Game/LifeDev/Game/Inters/Generic/Grab_C"));
	SFX_Trigger = CSnd.Object;
	
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		CMat(TEXT("/Game/LifeDev/Game/Var/Mats/Voxel/Palettes/Palette06_DMI"));
	Mesh->SetMaterial(0, CMat.Object);
	AnimFade->MatBase = CMat.Object;
	
	SetEnabled(false);
}
