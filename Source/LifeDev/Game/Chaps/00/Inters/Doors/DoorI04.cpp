// Copyright Jerónimo Barraco-Mármol

#include "DoorI04.h"

ADoorI04::ADoorI04():Super() {
	AnimEnabled = false;
	LockedDlg = "D04_LCK";
	ULockItemReq = "P02";
	ULockDlg = "D04_ULCK";
	
	Locked = true;
}
