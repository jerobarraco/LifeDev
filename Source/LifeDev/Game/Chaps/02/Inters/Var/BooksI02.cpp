// Copyright Jerónimo Barraco-Mármol

#include "BooksI02.h"

ABooksI02::ABooksI02():Super(1, 0) {
	Texts = { NSLOCTEXT("BooksI02", "State", "A book") };
	TriggerDlg = "BK02_T";
	UseRewardDestroy = false;
	IsOneShot = true;
	IsLocked = false;
	UseAutoActivate = true;
}
