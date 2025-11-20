// Copyright Jerónimo Barraco-Mármol

#include "BottleI01.h"

#include "CQuickMesh.h"
#include "Interact/Animator/CAnimatorFade.h"
#include "JUtils/Misc/JMiscConsts.h"

ABottleI01::ABottleI01():Super() {
	Texts = { NSLOCTEXT("Bottle00", "Text0", "Conditioner") };
	RewardItem = "Bottle01";
	UseRewardDestroy = true;
	UseFade = true;
	AnimFade->MatBase = nullptr; // use the mesh's mat
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		CMat(TEXT("/Game/LifeDev/Game/Var/Mats/Voxel/Palettes/Mask/Palette07_MMI"));
	if (LIKELY(CMat.Object)) Mesh->SetMaterial(0, CMat.Object);
	
	RewardIntersHint = {
		SoftOP(AInteract, "/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.SpotI04_UAID_D8BBC116E501525D02_1512082135"),
	};
}
