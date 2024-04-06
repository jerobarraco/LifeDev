// Copyright Jerónimo Barraco-Mármol

#include "BooksI01.h"

ABooksI01::ABooksI01():Super(1, 0) {
	SetEnabled(true);
	Texts = { FText::FromString(TEXT("Book"))};
	TriggerDlg = "BK00_T";
	UseRewardFade = false;
	IsOneShot = true;
	Locked = true;
}
