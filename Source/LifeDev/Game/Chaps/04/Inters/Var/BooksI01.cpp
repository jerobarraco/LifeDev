// Copyright Jerónimo Barraco-Mármol

#include "BooksI01.h"

ABooksI01::ABooksI01():Super(1, 0) {
	Texts = { NSLOCTEXT("BooksI01", "State", "A book") };
	TriggerDlg = "BK01_T";
	UseRewardDestroy = false;
	IsOneShot = true;
	IsLocked = false;
	// RewardStep = false; // rewards a card. set in the editor
	UseAutoActivate = true;
}
