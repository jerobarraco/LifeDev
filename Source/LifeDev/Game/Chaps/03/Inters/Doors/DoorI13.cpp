// Copyright Jerónimo Barraco-Mármol

#include "DoorI13.h"

ADoorI13::ADoorI13():Super() {
	UseAnim = false;
	// LockedDlg = "D13_L";
	IsLocked = true;
	UseAutoActivate = false;
	// todo this instance is not useful. recycle, potentially by doori16
#if WITH_EDITORONLY_DATA
	Comment = "TODO replace with a stub door. this one is not accessible anymore."; // todo
#endif
}
