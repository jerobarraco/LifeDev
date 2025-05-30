// Copyright Jerónimo Barraco-Mármol

#include "SpotI00.h"

ASpotI00::ASpotI00():Super() {
	Texts = {
		FText(NSLOCTEXT("Chap01", "Spot00.DropHere", "Drop clothes here")), 
		FText(NSLOCTEXT("Chap01", "Spot00.Full", "All done")),
	};
	Items = {
		// two shirts and a boxer, and a sock
		FName("C1C00"), FName("C1C01"), FName("C1C02"), FName("C1C04")
	};
}
