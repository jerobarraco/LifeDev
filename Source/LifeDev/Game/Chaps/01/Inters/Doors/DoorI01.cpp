// Copyright Jerónimo Barraco-Mármol

#include "DoorI01.h"

ADoorI01::ADoorI01():Super() {
	Locked = true;
	ULockItemReq = "WM";
	ULockDlg = "D01_ULCK";
	LockedDlg = "D01_LCK";
	
	RootComponent->SetWorldLocation(FVector(-290,316,0));
	RootComponent->SetWorldRotation(FRotator(0,180,0));
}
