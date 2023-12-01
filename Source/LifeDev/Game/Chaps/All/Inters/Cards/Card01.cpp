// Copyright Jerónimo Barraco-Mármol

#include "Card01.h"

ACard01::ACard01():Super() {
	UseAnim = false;
	RewardItem = "C1";
	TriggerDlg = "C1_PICK";
	Texts = { FText::FromString(TEXT("Pick Up")), FText::FromString(TEXT("Pick Up")) } ;
}
