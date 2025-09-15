// Copyright Jerónimo Barraco-Mármol

#include "SpotI02.h"

#include "JUtils/Misc/JMiscConsts.h"
#include "LifeDev/Core/Consts/ConstItems.h"

ASpotI02::ASpotI02():Super() {
	Texts = {
		NSLOCTEXT("SpotI02", "DropHere", "Drop clothes here"), 
		NSLOCTEXT("SpotI02", "Full", "All done"),
	};
	UnlockItems = {
		// a bra and a shirt a boxer and a sock
		FName("C1C01"), FName("C1C02"), LDConsts::Items::Bra, FName("C1C04")
	};

	// RewardActor set in editor to the puzzle that gives a card
	RewardActor = SoftOP(AActor, "/Game/LifeDev/Game/Sys/Game_L.Game_L:PersistentLevel.PuzzleI00_UAID_D8BBC116E5014BCE01_1680297915");
}
