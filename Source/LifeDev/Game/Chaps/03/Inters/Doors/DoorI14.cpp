// Copyright Jerónimo Barraco-Mármol

#include "DoorI14.h"

#include "LifeDev/Core/Consts/ConstItems.h"

ADoorI14::ADoorI14():Super() {
	LockedDlg = "D14_L"; // unnecessary since it uses the card1. but in case i re-organize the chapter.
	LockedItemDlg = "D14_LI";
	Locked = true;
	ULockDlg = "D14_U";
	ULockItem = LDConsts::Items::Card1;
	DisableWhileAnim = false; // avoid getting re-enabled due to state change
	// ULockItem = LDConsts::Items::Card3;
	
	UseItemDlgs = {
		{LDConsts::Items::Card0, "D14xC00"},
		// {LDConsts::Items::Card1, "D14xC01"}, // done with the ULockDlg
		{LDConsts::Items::Card2, "D14xC02"},
	};
	
	// this door is quite confusing. and while it seems interesting it rather make it better for the player.
	SetStateNow(1);
}
