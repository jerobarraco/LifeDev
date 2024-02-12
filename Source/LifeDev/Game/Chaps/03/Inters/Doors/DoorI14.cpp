// Copyright Jerónimo Barraco-Mármol

#include "DoorI14.h"

ADoorI14::ADoorI14():Super() {
	LockedDlg = "D14_L";
	// UseItemDlgs = {
	// 	{LDConsts::Items::Card0, "D10xC00"},
	// 	{LDConsts::Items::Card2, "D10xC02"},
	// };
	Locked = true;
	ULockDlg = "D14_UL";
	ULockItem = "D14_ULI";
	ULockItemReq = "??";
}