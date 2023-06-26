// Copyright Jerónimo Barraco-Mármol

#include "Card01.h"

ACard01::ACard01():Super() {
	AnimEnabled = false;
	ItemReward = "C1";
	Texts = { FText::FromString(TEXT("Pick Up")), FText::FromString(TEXT("Pick Up")) } ;
}
