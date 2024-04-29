// Copyright Jerónimo Barraco-Mármol

#include "DoorI17.h"

ADoorI17::ADoorI17():Super() {
	UseAnim = true;
	LockedDlg = "D17_L";
	Locked = true;
	SetEnabled(false); // todo re-enable once the dialog is set
}
