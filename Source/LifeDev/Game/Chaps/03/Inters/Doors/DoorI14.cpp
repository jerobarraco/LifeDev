// Copyright Jerónimo Barraco-Mármol

#include "DoorI14.h"

#include "LifeDev/Game/Sys/Consts/ConstItems.h"

ADoorI14::ADoorI14():Super() {
	LockedDlg = "D14_L";
	LockedItemDlg = "D14_LI";
	Locked = true;
	ULockDlg = "D14_U";
	ULockItem = LDConsts::Items::Card3;
	
	UseItemDlgs = {
		{LDConsts::Items::Card0, "D14xC00"},
		{LDConsts::Items::Card1, "D14xC01"},
		{LDConsts::Items::Card2, "D14xC02"},
	};
}
