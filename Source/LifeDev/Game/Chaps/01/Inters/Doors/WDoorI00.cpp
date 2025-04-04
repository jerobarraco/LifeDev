// Copyright Jerónimo Barraco-Mármol

#include "WDoorI00.h"

AWDoorI00::AWDoorI00():Super() {
	IsLocked = true;
	ULockItem = "KW0";
	ULockDlg = "W0_ULCK";
	LockedItemDlg = "W0_LCKIT";
	LockedDlg = "W0_LCK";
}
