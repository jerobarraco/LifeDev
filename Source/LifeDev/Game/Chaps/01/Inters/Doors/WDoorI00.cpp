// Copyright Jerónimo Barraco-Mármol

#include "WDoorI00.h"

AWDoorI00::AWDoorI00(): Super() {
	IsLocked = true;
	ULockItemReq = ULockItem = "KW0";
	// ULockDlg = "W0_ULCK";
	// LockedItemDlg = "W0_LCKIT"; // driven by datatable
	// LockedDlg = "W0_LCK"; // already set on the table
}
