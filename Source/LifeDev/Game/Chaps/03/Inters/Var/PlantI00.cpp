// Copyright Jerónimo Barraco-Mármol

#include "PlantI00.h"

APlantI00::APlantI00():Super() {
	Texts = { FText::FromString(TEXT("Plant"))};
	UseAnim = false;
	UseRewardDestroy = false;
	StateNum = 1;
	TriggerDlg = "PL00_T";
}
