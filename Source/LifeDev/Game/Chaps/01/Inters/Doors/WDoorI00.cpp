// Copyright Jerónimo Barraco-Mármol

#include "WDoorI00.h"

AWDoorI00::AWDoorI00() {
	IsLocked = true;
	ULockItemReq = ULockItem = "KW0";
	UseHint = true;
	// this is less performant but this door is loaded in the room not chapter causing dependency issues
	// hint after the c1 started
	HintCondition = "{Inter.Locked.DoorI01}";

	// ULockDlg = "W0_ULCK";
	// LockedItemDlg = "W0_LCKIT"; // driven by datatable
	// LockedDlg = "W0_LCK"; // already set on the table
}
