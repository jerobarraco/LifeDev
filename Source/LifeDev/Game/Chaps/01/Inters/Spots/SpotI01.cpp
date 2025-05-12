// Copyright Jerónimo Barraco-Mármol

#include "SpotI01.h"

#include "LifeDev/Core/Consts/ConstItems.h"

ASpotI01::ASpotI01():Super() {
	DropDlg = "IS_C1D";
	Items = {LDConsts::Items::Bra};
	Texts = {
		FText(NSLOCTEXT("Chap01", "Spot01.DropHere", "Drop her clothes here")),
		FText(NSLOCTEXT("Chap01", "Spot01.Full", "All done")),
	};
	RewardIntersHint = {
		TSoftObjectPtr<AInteract>(FSoftObjectPath("/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.Clout02_UAID_D8BBC116E501FB8B01_1495993119"))
	};
}
