// Copyright Jerónimo Barraco-Mármol

#include "HeaterI01.h"

AHeaterI01::AHeaterI01():Super() {
	UseAnim = false;
	UseFade = false;
	UseRewardDestroy = false;
	UseAutoActivate = true;
	IsLocked = true;

	Texts = { NSLOCTEXT("HeaterI01", "Turn On", "Turn on")};
}
