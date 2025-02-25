// Copyright Jerónimo Barraco-Mármol

#include "DoorI01.h"

ADoorI01::ADoorI01():Super() {
	Locked = true;
	ULockDlg = "D01_ULCK";
	LockedDlg = "D01_LCK";
	// for the future
	ULockItemReq = "WM";
	// ULockCondition = "{V.Item.Count.Batts} & {V.Item.Count.WM}";

	RootComponent->SetWorldLocation(FVector(-290,316,0));
	RootComponent->SetWorldRotation(FRotator(0,180,0));
}
