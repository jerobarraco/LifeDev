// Copyright Jerónimo Barraco-Mármol

#include "PictureI00.h"

#include "CQuickMesh.h"

APictureI00::APictureI00():Super() {
	Texts = {
		FText::FromString(TEXT("Picture A")),
		FText::FromString(TEXT("Picture B")),
		FText::FromString(TEXT("Picture C")),
		FText::FromString(TEXT("Picture D"))
	};
	StateNum = 4;
	UseRewardDestroy = false;
	UseFade = false;
	// TODO state transforms
	
	static ConstructorHelpers::FObjectFinder<UMaterialInterface>
		CMat (TEXT("/Game/LifeDev/Game/Var/Mats/Voxel/Palettes/Palette00_DMI"));
	if (CMat.Succeeded())
		Mesh->SetMaterial(0, CMat.Object.Get());

	APictureI00::SetAutoActivate(true);
}
