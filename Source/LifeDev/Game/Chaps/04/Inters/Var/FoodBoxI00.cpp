// Copyright Jerónimo Barraco-Mármol

#include "FoodBoxI00.h"

#include "JUtils/Actors/CQuickMesh.h"

AFoodBoxI00::AFoodBoxI00():Super() {
	Texts = { FText::FromString(TEXT("Pick rice box"))};
	RewardItem = "Food00";
	RewardFlash = .1;
	UseRewardFade = true;
	Locked = true;
	LockedDlg = "Food00_L";
	
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		CMesh(TEXT("/Game/LifeDev/Game/Var/Mats/Voxel/Palettes/Palette06_DMI"));
	Mesh->SetMaterial(0, CMesh.Object);
}
