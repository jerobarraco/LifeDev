// Copyright Jerónimo Barraco-Mármol

#include "BooksI01.h"

ABooksI01::ABooksI01():Super(1, 0) {
	Texts = { FText::FromString(TEXT("Book"))};
	TriggerDlg = "BK01_T";
	UseRewardDestroy = false;
	IsOneShot = true;
	Locked = false;
	RewardStep = false; // rewards a card. set in the editor

	Super::SetAutoActivate(true);
}
