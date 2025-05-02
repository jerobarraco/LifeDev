// Copyright Jerónimo Barraco-Mármol

#include "DoorI01.h"

ADoorI01::ADoorI01():Super() {
	IsLocked = true;
	// ULockDlg = "D01_ULCK";
	// LockedDlg = "D01_LCK";
	// for the future
	ULockItemReq = "WM"; // the wm depends on the batts so no need to go so hard on this
	// ULockCondition = "{V.Item.Count.Batts} & {V.Item.Count.WM}";
	UseHint = true;
	HintCondition = "{Story.Step.Start.C1S0}&!{Story.Step.Start.C2S0}"; // only on chapter 1
	RootComponent->SetWorldLocation(FVector(-290,316,0));
	RootComponent->SetWorldRotation(FRotator(0,180,0));
}
