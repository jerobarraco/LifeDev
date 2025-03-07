// Copyright Jerónimo Barraco-Mármol

#include "SpotI04.h"

ASpotI04::ASpotI04() {
	UseOrder = false;
	UseStateInc = false;
	Items = {"Bottle00", "Bottle01"};
	Texts = {
		NSLOCTEXT("SpotI04", "Text0", "Drop bottle")
	};
}

// rewards actor is set on editor