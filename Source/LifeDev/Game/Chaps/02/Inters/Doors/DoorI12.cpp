// Copyright Jerónimo Barraco-Mármol

#include "DoorI12.h"

ADoorI12::ADoorI12():Super() {
	IsLocked = true;
	// LockedDlg = "D12_L";
	UseHint = true; // this door goes nowhere, but hint the player in the story flow.
	// todo hint condition door10 is not triggered
}
