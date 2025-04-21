// Copyright Jerónimo Barraco-Mármol

#include "SpotI01.h"

#include "LifeDev/Core/Consts/ConstItems.h"

ASpotI01::ASpotI01():Super() {
	TriggerDlg = "IS_C1T";
	LockedDlg = "IS_C1L"; // TODO
	DropDlg = "IS_C1D";
	Items = {LDConsts::Items::Bra};
	Texts = {
		FText(NSLOCTEXT("Chap01", "Spot01.DropHere", "Drop her clothes here")),
		FText(NSLOCTEXT("Chap01", "Spot01.Full", "All done")),
	};
}
