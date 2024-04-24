// Copyright Jerónimo Barraco-Mármol

#include "BooksI00.h"

ABooksI00::ABooksI00():Super(3, 93) {
	SetEnabled(true);
	Texts = { FText::FromString(TEXT("Check"))};
	TriggerDlg = "BK00_T";
	UseRewardDestroy = false;
}
