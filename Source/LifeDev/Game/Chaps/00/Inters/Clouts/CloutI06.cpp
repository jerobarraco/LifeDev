// Copyright Jerónimo Barraco-Mármol

#include "CloutI06.h"

ACloutI06::ACloutI06():Super() {
	TriggerDlg = "C06_T";
	static FText T = FText::FromString(TEXT("Dirty Clothes"));
	Texts = { T };
}
