// Copyright Jerónimo Barraco-Mármol

#include "DoorI16.h"

ADoorI16::ADoorI16():Super() {
	// TODO potentially reuse doori13 for this
	UseAnim = false;
	// LockedDlg = "D16_L"; // todo dialog 
	IsLocked = true;
}
