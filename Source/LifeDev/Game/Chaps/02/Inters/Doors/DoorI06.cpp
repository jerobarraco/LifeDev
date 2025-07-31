// Copyright Jerónimo Barraco-Mármol

#include "DoorI06.h"

ADoorI06::ADoorI06():Super() {
	UseAnim = false;
	// LockedDlg = "D06_L";
	IsLocked = true;
#if WITH_EDITORONLY_DATA
	Comment = "TODO on chap03 replace with a stub door as is not accessible"; // todo
#endif
}
