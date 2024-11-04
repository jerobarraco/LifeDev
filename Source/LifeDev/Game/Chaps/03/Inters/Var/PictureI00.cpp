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
	
	// APictureI00::SetAutoActivate(true);
}
