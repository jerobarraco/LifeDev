// Copyright Jerónimo Barraco-Mármol

#include "KeysI00.h"

AKeysI00::AKeysI00():Super() {
	Texts = { FText::FromString(TEXT("Keys")) } ;
	RewardFlash = .1;
	RewardItem = "KW0";
	UseFade = true; // needed to fade on reward
	UseAnim = false;
	StateNum = 1;
	Super::SetAutoActivate(true);
}