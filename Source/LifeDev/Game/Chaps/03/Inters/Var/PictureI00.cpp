// Copyright Jerónimo Barraco-Mármol

#include "PictureI00.h"

#include "CQuickMesh.h"

APictureI00::APictureI00():Super() {
	Texts = { FText::FromString(TEXT("Picture"))};
	TriggerDlg = "PIC00_T";
	IsOneShot = true;
	RewardFlash = .09;
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		CMat (TEXT("/Game/LifeDev/Game/Var/Mats/Voxel/Palettes/Palette00_DMI"));
	if (CMat.Succeeded()) Mesh->SetMaterial(0, CMat.Object.Get());
	APictureI00::SetAutoActivate(true);
	// TODO, maybe i could add some card interaction to this?
}
