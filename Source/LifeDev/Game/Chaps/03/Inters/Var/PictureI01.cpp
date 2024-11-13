// Copyright Jerónimo Barraco-Mármol

#include "PictureI01.h"

#include "CQuickMesh.h"

APictureI01::APictureI01():Super() {
	Texts = { FText::FromString(TEXT("Picture"))};
	TriggerDlg = "PIC01_T";
	IsOneShot = true;
	RewardFlash = .09;
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		CMat (TEXT("/Game/LifeDev/Game/Var/Mats/Voxel/Palettes/Palette00_DMI"));
	if (CMat.Succeeded()) Mesh->SetMaterial(0, CMat.Object.Get());
	APictureI01::SetAutoActivate(true);
	// TODO add the transform
	
	// TODO, maybe i could add some card interaction to this?
}
