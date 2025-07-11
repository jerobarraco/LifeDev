// Copyright Jerónimo Barraco-Mármol

#include "DoorI12.h"

ADoorI12::ADoorI12():Super() {
	IsLocked = true;
	// LockedDlg = "D12_L";
	UseHint = true; // this door goes nowhere, but hint the player in the story flow.
	// disable hint once the user tried the doori10
	// checking for locked and trigger since the player could trigger the door without the locked
	HintCondition = "!({Inter.Locked.DoorI10}|{Inter.Trigger.DoorI10})";
}
