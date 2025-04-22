// Copyright Jerónimo Barraco-Mármol

#include "DoorI14.h"

#include "LifeDev/Core/Consts/ConstItems.h"

ADoorI14::ADoorI14():Super() {
	// commented to help with refactoring later
	// LockedDlg = "D14_L"; // unnecessary since it uses the card1. but in case i re-organize the chapter.
	// LockedItemDlg = "D14_LI";
	// ULockDlg = "D14_U";
	IsLocked = true;
	ULockItemReq = ULockItem = LDConsts::Items::Card1; // TODO don't use the card! use something else! (screwdriver?)
	// ULockItem = LDConsts::Items::Card3;

	// this door is quite confusing. and while it seems interesting it rather make it better for the player.
	// so disable and start open
	UseAutoActivate = false;
	SetStateNow(1);
}
