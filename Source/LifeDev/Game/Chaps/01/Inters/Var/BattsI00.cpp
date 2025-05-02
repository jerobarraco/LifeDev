// Copyright Jerónimo Barraco-Mármol

#include "BattsI00.h"

#include "LifeDev/Core/Consts/ConstItems.h"

ABattsI00::ABattsI00():Super() {
	Texts = { FText::FromString(TEXT("Pick up batteries")) };
	RewardFlash = .1;
	RewardItem = LDConsts::Items::Batts;
	TriggerDlg = "BattsPick";
	UseAutoActivate = true;
	UseHint = true;
	HintCondition = "{Inter.TriggerL.WalkmanI00}";

	// needs to move to be able to be attached to the drawer
	Super::SetMobility(EComponentMobility::Movable);
}
