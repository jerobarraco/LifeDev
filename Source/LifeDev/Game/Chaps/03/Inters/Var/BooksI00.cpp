// Copyright Jerónimo Barraco-Mármol

#include "BooksI00.h"

ABooksI00::ABooksI00() {
	BookCount = 3;
	RndSeed = 5;
	Texts = { NSLOCTEXT("BooksI00", "State0", "Books")};
	// TriggerDlg = "BK00_T";
	UseRewardDestroy = false;
	IsOneShot = true;
	UseAutoActivate = true;
}
