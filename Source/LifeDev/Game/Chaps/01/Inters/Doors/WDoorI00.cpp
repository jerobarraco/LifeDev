// Copyright Jerónimo Barraco-Mármol

#include "WDoorI00.h"

AWDoorI00::AWDoorI00():Super() {
	IsLocked = true;
	ULockItem = "KW0";
	// ULockDlg = "W0_ULCK";
	LockedItemDlg = "W0_LCKIT"; // todo have to set this up on the data table. with a group with "Pick"
	// LockedDlg = "W0_LCK"; // already set on the table
}
