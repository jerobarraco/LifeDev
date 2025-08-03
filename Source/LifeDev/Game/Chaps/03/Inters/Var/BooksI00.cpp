// Copyright Jerónimo Barraco-Mármol

#include "BooksI00.h"

ABooksI00::ABooksI00():Super(3, 93) {
	Texts = { NSLOCTEXT("BooksI00", "State0", "Books")};
	// TriggerDlg = "BK00_T";
	UseRewardDestroy = false;
	IsOneShot = true;
	UseAutoActivate = true;
}
