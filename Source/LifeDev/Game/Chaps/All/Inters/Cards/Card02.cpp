// Copyright Jerónimo Barraco-Mármol

#include "Card02.h"

ACard02::ACard02():Super() {
	UseAnim = false;
	RewardItem = "C2";
	Texts = { FText::FromString(TEXT("Pick Up")), FText::FromString(TEXT("Pick Up")) } ;
}
