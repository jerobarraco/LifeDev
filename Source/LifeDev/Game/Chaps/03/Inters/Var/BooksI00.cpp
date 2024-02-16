// Copyright Jerónimo Barraco-Mármol

#include "BooksI00.h"

ABooksI00::ABooksI00():Super(3, -1) {
	SetEnabled(true);
	Texts = { FText::FromString(TEXT("Check"))};
	TriggerDlg = "BKI00_T";
	UseRewardFade = false;
}
